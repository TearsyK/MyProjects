#include <cstring>
#include <cstdint>
#include <algorithm>

// ============================================================================
// Projekt: Rozmycie Gaussa - Konwolucja PIONOWA (Faza 2)
// Plik: GaussianBlurCPP_Vertical.cpp
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

static void ProcessRowVert_First(
    const unsigned char* input,
    unsigned char* output,
    int width,
    int stride
) {
    for (int x = 0; x < width; ++x) {
        int pixelOffset = x * BYTES_PER_PIXEL;

        for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
            uint32_t pix0 = input[pixelOffset + ch];                    // y=0
            uint32_t pix1 = input[stride + pixelOffset + ch];           // y=1

            uint32_t total = (pix0 * K1) + (pix1 * K2);
            total = total >> Q7_SHIFT;

            output[pixelOffset + ch] = static_cast<uint8_t>(
                total > 255 ? 255 : total
                );
        }
    }
}

static void ProcessRowVert_Main(
    const unsigned char* input,
    unsigned char* output,
    int width,
    int stride,
    int y
) {
    int baseOffset = y * stride;

    for (int x = 0; x < width; ++x) {
        int pixelOffset = baseOffset + x * BYTES_PER_PIXEL;

        for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
            uint32_t pix0 = input[pixelOffset - stride + ch];         // y-1
            uint32_t pix1 = input[pixelOffset + ch];                  // y
            uint32_t pix2 = input[pixelOffset + stride + ch];         // y+1

            uint32_t total = (pix0 * K0) + (pix1 * K1) + (pix2 * K0);
            total = total >> Q7_SHIFT;

            output[pixelOffset + ch] = static_cast<uint8_t>(
                total > 255 ? 255 : total
                );
        }
    }
}

static void ProcessRowVert_Last(
    const unsigned char* input,
    unsigned char* output,
    int width,
    int stride,
    int height
) {
    int lastRowOffset = (height - 1) * stride;

    for (int x = 0; x < width; ++x) {
        int pixelOffset = lastRowOffset + x * BYTES_PER_PIXEL;

        for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch) {
            uint32_t pix0 = input[pixelOffset - stride + ch];         // y-2
            uint32_t pix1 = input[pixelOffset + ch];                  // y-1

            uint32_t total = (pix0 * K2) + (pix1 * K1);
            total = total >> Q7_SHIFT;

            output[pixelOffset + ch] = static_cast<uint8_t>(
                total > 255 ? 255 : total
                );
        }
    }
}

// ============================================================================
// GŁÓWNA FUNKCJA - VERTICAL BLUR
// ============================================================================

extern "C" __declspec(dllexport) void GaussianBlurCPP_Vertical(
    const unsigned char* inputImage,
    unsigned char* outputImage,
    int width,
    int stride,
    int startY,
    int endY,
    int height
) {
    startY = std::max(0, startY);
    endY = std::min(height, endY);

    // === FIRST ROW (y=0): row0*K1 + row1*K2 ===
    if (startY == 0 && height > 1) {
        ProcessRowVert_First(inputImage, outputImage, width, stride);
    }

    // === MAIN LOOP: row[y-1]*K0 + row[y]*K1 + row[y+1]*K0 ===
    for (int y = std::max(1, startY); y < std::min(endY, height - 1); ++y) {
        ProcessRowVert_Main(inputImage, outputImage, width, stride, y);
    }

    // === LAST ROW (y=height-1): row[y-1]*K2 + row[y]*K1 ===
    if (endY == height && height > 1) {
        ProcessRowVert_Last(inputImage, outputImage, width, stride, height);
    }
}
