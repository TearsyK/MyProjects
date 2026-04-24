#include <cstring>
#include <cstdint>
#include <algorithm>

// ============================================================================
// Projekt: Rozmycie Gaussa - Konwolucja POZIOMA (Faza 1)
// Plik: GaussianBlurCPP_Horizontal.cpp
//
// Parametry jądra Q7: [0x10, 0x60, 0x20] = [16, 96, 32] / 128
// Kernel: [0.125, 0.75, 0.125] dla środka, [0.75, 0.25] dla brzegów
// Format pikseli: RGB (3 bajty na piksel)
// ============================================================================

// Q7 kernel coefficients (identyczne jak w ASM)
static constexpr int K0 = 0x10;  // 16  → 0.125
static constexpr int K1 = 0x60;  // 96  → 0.75
static constexpr int K2 = 0x20;  // 32  → 0.25
static constexpr int Q7_SHIFT = 7;
static constexpr int BYTES_PER_PIXEL = 3;  // RGB

// ============================================================================
// Pomocnicze funkcje
// ============================================================================

static void ProcessPixelHorz_First(
    const unsigned char* input,
    unsigned char* output,
    int baseOffset
) {
    for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
        uint32_t pix0 = input[baseOffset + ch];
        uint32_t pix1 = input[baseOffset + BYTES_PER_PIXEL + ch];

        uint32_t total = (pix0 * K1) + (pix1 * K2);
        total = total >> Q7_SHIFT;

        output[baseOffset + ch] = static_cast<uint8_t>(
            total > 255 ? 255 : total
            );
    }
}

static void ProcessPixelHorz_Main(
    const unsigned char* input,
    unsigned char* output,
    int baseOffset,
    int x
) {
    int pixelOffset = baseOffset + x * BYTES_PER_PIXEL;

    for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
        uint32_t pix0 = input[pixelOffset - BYTES_PER_PIXEL + ch];
        uint32_t pix1 = input[pixelOffset + ch];
        uint32_t pix2 = input[pixelOffset + BYTES_PER_PIXEL + ch];


        uint32_t total = (pix0 * K0) + (pix1 * K1) + (pix2 * K0);
        total = total >> Q7_SHIFT;

        output[pixelOffset + ch] = static_cast<uint8_t>(
            total > 255 ? 255 : total
            );
    }
}

static void ProcessPixelHorz_Last(
    const unsigned char* input,
    unsigned char* output,
    int baseOffset,
    int width
) {
    int pixelOffset = baseOffset + (width - 1) * BYTES_PER_PIXEL;

    for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
        uint32_t pix0 = input[pixelOffset - BYTES_PER_PIXEL + ch];
        uint32_t pix1 = input[pixelOffset + ch];


        uint32_t total = (pix0 * K2) + (pix1 * K1);
        total = total >> Q7_SHIFT;

        output[pixelOffset + ch] = static_cast<uint8_t>(
            total > 255 ? 255 : total
            );
    }
}

static void CopyPixel(
    const unsigned char* input,
    unsigned char* output,
    int offset
) {
    std::memcpy(output + offset, input + offset, BYTES_PER_PIXEL);
}

// ============================================================================
// GŁÓWNA FUNKCJA - HORIZONTAL BLUR
// ============================================================================

extern "C" __declspec(dllexport) void GaussianBlurCPP_Horizontal(
    const unsigned char* inputImage,
    unsigned char* tempBuffer,
    int width,
    int stride,
    int startY,
    int endY
) {
    startY = std::max(0, startY);
    endY = std::max(startY, endY);

    for (int y = startY; y < endY; ++y) {
        int baseOffset = y * stride;

        // === FIRST PIXEL (x=0): p0*K1 + p1*K2 ===
        if (width > 1) {
            ProcessPixelHorz_First(inputImage, tempBuffer, baseOffset);
        }
        else {
            CopyPixel(inputImage, tempBuffer, baseOffset);
        }

        // === MAIN LOOP: p[x-1]*K0 + p[x]*K1 + p[x+1]*K0 ===
        for (int x = 1; x < width - 1; ++x) {
            ProcessPixelHorz_Main(inputImage, tempBuffer, baseOffset, x);
        }

        // === LAST PIXEL (x=width-1): p[x-1]*K2 + p[x]*K1 ===
        if (width > 1) {
            ProcessPixelHorz_Last(inputImage, tempBuffer, baseOffset, width);
        }
    }
}
