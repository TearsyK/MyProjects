; ============================================================================
; Projekt: Rozmycie Gaussa - Konwolucja POZIOMA (Faza 1)
; Plik: GaussianBlurASM_Horizontal.asm
; Autor: Kacper Kostanek
; Data: Semestr zimowy 2025/2026
; Wersja: 4.1
;
; Parametry:
;   RCX - inputImage (unsigned char*, READ ONLY)
;   RDX - tempBuffer (float*, WRITE ONLY)
;   R8 - width
;   R9 - stride
;   [RSP+40] - startY
;   [RSP+48] - endY
; ============================================================================

.data
.code

PUBLIC GaussianBlurASM_Horizontal
GaussianBlurASM_Horizontal PROC

    push rbx
    push rbp
    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15
    sub rsp, 96                     

    ; === SETUP REGISTERS ===
    mov rsi, rcx                           ; rsi = input pointer
    mov rdi, rdx                           ; rdi = output pointer
    
                                           ; r8 = width
                                           ; r9 = stride
    imul r8, r8, 3h
    mov r10, qword ptr [rsp+40+160]       ; r10 = startY
    mov r11, qword ptr [rsp+48+160]       ; r11 = endY
    
    ; === BUILD KERNEL VECTORS: [k0, k1, k2] ===
    
    mov eax, 10h 
    vmovq xmm0, rax 
    vpbroadcastw ymm3, xmm0
    mov eax, 60h
    vmovd xmm0, eax
    vpbroadcastw ymm4, xmm0
    mov eax, 20h
    vmovd xmm0, eax
    vpbroadcastw ymm6, xmm0
    mov eax, 00FFh
    vmovd xmm0, eax
    vpbroadcastq ymm7, xmm0

    ; === PETLA Y - od startY do endY ===
horz_y_loop:

    cmp r10, r11
    jge horz_y_complete

    ; === PRE-CALCULATE y * stride ===
    mov rax, r10                    ; rax = y (64-bit)
    imul rax, r9                    ; rax = y * stride
    mov r13, rax                    ; r13 = y*stride (tymczasowo)

    ; === PETLA X ===
    xor r14, r14                    ; r13d = x

    ; === FIRST PIXEL (p0) ===
    mov rax, r13
    lddqu xmm0, xmmword ptr [rsi+rax]     ; p0
    add rax, 3h
    lddqu xmm1, xmmword ptr [rsi+rax]     ; p1

    vpmovzxbw ymm0, xmm0
    vpmovzxbw ymm1, xmm1
    vpmullw ymm0, ymm0, ymm4
    vpmullw ymm1, ymm1, ymm6
    vpaddw ymm0, ymm0, ymm1
    vpsrlw ymm0, ymm0, 7


    vextracti128 xmm5, ymm0, 1
    vpackuswb xmm0, xmm0, xmm5
    

    ; Move transformed channels into memory
    movdqu xmmword ptr [rdi+rax-3h], xmm0


horz_x_loop:    
    
    mov  rax, r9                   ; eax = width
    sub  rax, 16h                  ; eax = width - 22
    cmp  rax, r14                  ; x < width - 16 ?
    jl horz_x_complete

    
    mov rax, r14
    add rax, r13

    ; Load data from memory to vector registers
    ; And unpack 8-bit channels to 16-bit channels
    vpmovzxbw ymm0, xmmword ptr [rsi+rax]
    vpmovzxbw ymm1, xmmword ptr [rsi+rax+3h]
    vpmovzxbw ymm2, xmmword ptr [rsi+rax+6h]
    
    ; Multiply unpacked 16-bit channels by kernel vectors
    vpmullw ymm0, ymm0, ymm3
    vpmullw ymm2, ymm2, ymm3
    vpmullw ymm1, ymm1, ymm4

    ; Add up all vectors
    vpaddw ymm0, ymm0, ymm1
    vpaddw ymm0, ymm0, ymm2


    ; Divide multiplied channels by 128 (Q7 * Q7 = Q14, we need to transform it back to Q7)
    vpsrlw ymm0, ymm0, 7

    ; Pack 16-bit channels to 8-bit channels
    vextracti128 xmm5, ymm0, 1
    vpackuswb xmm0, xmm0, xmm5

    ; Move transformed channels into memory
    movdqu xmmword ptr [rdi+rax+3h], xmm0

    ; Prepare for next set of data from memory
    add r14, 10h
    jmp horz_x_loop

horz_x_complete:
    
;--- Last loop

    mov rax, r13
    add rax, r8
    sub rax, 16h

    ; Load data from memory to vector registers
    ; And unpack 8-bit channels to 16-bit channels
    vpmovzxbw ymm0, xmmword ptr [rsi+rax]
    vpmovzxbw ymm1, xmmword ptr [rsi+rax+3h]
    vpmovzxbw ymm2, xmmword ptr [rsi+rax+6h]
    
    ; Multiply unpacked 16-bit channels by kernel vectors
    vpmullw ymm0, ymm0, ymm3
    vpmullw ymm2, ymm2, ymm3
    vpmullw ymm1, ymm1, ymm4

    ; Add up all vectors
    vpaddw ymm0, ymm0, ymm1
    vpaddw ymm0, ymm0, ymm2

    ; Divide multiplied channels by 128 (Q7 * Q7 = Q14, we need to transform it back to Q7)
    vpsrlw ymm0, ymm0, 7

    ; Pack 16-bit channels to 8-bit channels
    vextracti128 xmm5, ymm0, 1
    vpackuswb xmm0, xmm0, xmm5

    ; Move transformed channels into memory
    movdqu xmmword ptr [rdi+rax+3h], xmm0

;--- Last pixel ---
        
    mov rax, r8        ; eax = width 
    add rax, r13        ; rax = base
    
    movdqu xmm0, xmmword ptr [rsi+rax-10h]
    movdqu xmm1, xmmword ptr [rsi+rax-10h]
    vpslldq xmm1, xmm1, 3

    vpmovzxbw ymm0, xmm0
    vpmovzxbw ymm1, xmm1
    vpmullw ymm0, ymm0, ymm4
    vpmullw ymm1, ymm1, ymm6
    vpaddw ymm0, ymm0, ymm1
    vpsraw ymm0, ymm0, 7
    vpsraw ymm1, ymm1, 7
    

    vextracti128 xmm5, ymm0, 1
    vpackuswb xmm0, xmm0, xmm5

    ; Move transformed channels into memory
    mov rcx, r8
    add rcx, r13
    sub rcx, 3
    pextrd eax, xmm0, 3

    shr eax, 8
    mov byte ptr [rdi + rcx], al
    shr eax, 8
    add rcx, 1
    mov byte ptr [rdi + rcx], al
    shr eax, 8
    add rcx, 1
    mov byte ptr [rdi + rcx], al

    inc r10
    jmp horz_y_loop

horz_y_complete:

    add rsp, 96
    pop r15
    pop r14
    pop r13
    pop r12
    pop rsi
    pop rdi
    pop rbp
    pop rbx
    ret

GaussianBlurASM_Horizontal ENDP

END
