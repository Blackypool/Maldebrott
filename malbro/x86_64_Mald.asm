"main":
        push    rbx                             ; save regs

        xor     r9d, r9d                        ; r9 = 0
        xor     eax, eax                        ; ax = 0

        ; r9 = 0
        ; ax = 0


.L2:    ; обработка y_vec + цикл y_0 !!! удалил мой цикл потому что одинокавые параметры в массиве y_vec  ---
        pxor    xmm12, xmm12                    ; xmm12 = 0
        xor     r10d, r10d                      ; r10 = 0

        ; xmm12 = ax = y_0
        cvtsi2ss        xmm12, eax              ; Convert Int 2 Single-precision(одинарная точность) float

    ;   y_vec[i] = (float)(y_0) / OY_OY * 1.74 * zoom + offset_OY;
        
        divss   xmm12, DWORD PTR .L_500[rip]    ; div scalar single-p   //   [rip] == [rel] for PIE   //   y_0 / 500

        cvtss2sd        xmm12, xmm12            ; doing double точность из single                   !!!!!! need opt

        mulsd   xmm12, QWORD PTR .L_1_74[rip]   ; y_0 * 1.74
        subsd   xmm12, QWORD PTR .L_0_9[rip]    ; y_0 - offset_OY(0.9)        LC6 = 0.9 != -0.9 !!!
        cvtsd2ss        xmm12, xmm12            ; double точноть -> single

        ; xmm12 = y_vec[0]  ---
        ; r10 = 0
        ; AX = i of x_0


        ; r10 = x_0
.L9:    ; цикл x_0
        pxor    xmm0, xmm0                          ; xmm0 = 0
        pxor    xmm7, xmm7                          ; xmm7 = 0

        lea     edx, [r10+1]                        ; dx = r10 + 1 = x_0 + 1

    xor     edi, edi                            ; di = 0

        cvtsi2ss        xmm0, r10d                  ; x_0 = float(r10d).0       // xmm0 = x_0
        divss   xmm0, DWORD PTR .L_800[rip]         ; x_0 / 800
        cvtss2sd        xmm0, xmm0                  ; (x_0).0 -> (x_0).00
    pxor    xmm2, xmm2                          ; xmm2 = 0
        mulsd   xmm0, QWORD PTR .L_2_2[rip]         ; x_0 * 2.2

        pxor    xmm4, xmm4                          ; xmm4 = 0
        pxor    xmm1, xmm1                          ; xmm1 = 0

    mov     esi, 1                              ; si = 1

        subsd   xmm0, QWORD PTR .L_1_7[rip]         ; x_0 - offset_OX

   ;  memcpy of y_vec                           
        movaps  xmm11, xmm12                        ; xmm11 = xmm12
        movaps  xmm10, xmm12                        ; xmm10 = xmm12

        mov     ecx, 1                              ; cx = 1

        movaps  xmm9, xmm12                         ; xmm9 = xmm12
        movaps  xmm8, xmm12                         ; xmm8 = xmm12
   ;

    mov     r8d, 1                              ; r8 = 1
        cvtsd2ss        xmm7, xmm0                  ; xmm0.d -> xmm7.s
        pxor    xmm0, xmm0                          ; xmm0 = 0
        movss   DWORD PTR [rsp-4], xmm7             ; x_vec[0] = xmm7


        cvtsi2ss        xmm0, edx                   ; xmm0 = (float)dx
        divss   xmm0, DWORD PTR .L_800[rip]         ; xmm0 / 800 
        cvtss2sd        xmm0, xmm0                  ; xmm0.0 -> xmm0.00
        mulsd   xmm0, QWORD PTR .L_2_2[rip]         ; xmm0 * 2.2
        subsd   xmm0, QWORD PTR .L_1_7[rip]         ; xmm0 - 1.7
    lea     edx, [r10+2]                        ; dx = r10+2
        cvtsd2ss        xmm2, xmm0                  ; xmm0.00 -> xmm2.0
        pxor    xmm0, xmm0                          ; xmm0 = 0
        movss   DWORD PTR [rsp-8], xmm2             ; x_vec[1] = xmm2


        cvtsi2ss        xmm0, edx                   ; xmm0 = (float)dx          // dx = r10 + i // r10 = x_0 = xmm0
        divss   xmm0, DWORD PTR .L_800[rip]         ; xmm0 / 800 
        cvtss2sd        xmm0, xmm0                  ; xmm0.0 -> xmm0.00
        mulsd   xmm0, QWORD PTR .L_2_2[rip]         ; xmm0 * 2.2
        subsd   xmm0, QWORD PTR .L_1_7[rip]         ; xmm0 - 1.7
    lea     edx, [r10+3]                        ; dx = r10+3 // fo future
        cvtsd2ss        xmm4, xmm0                  ; xmm0.00 -> xmm4.0
        pxor    xmm0, xmm0                          ; xmm0 = 0
        movss   DWORD PTR [rsp-12], xmm4            ; x_vec[2] = xmm4


        cvtsi2ss        xmm0, edx                   ; xmm0 = (float)dx
        divss   xmm0, DWORD PTR .L_800[rip]         ; xmm0 / 800 
        cvtss2sd        xmm0, xmm0                  ; xmm0.0 -> xmm0.00
        mulsd   xmm0, QWORD PTR .L_2_2[rip]         ; xmm0 * 2.2
        subsd   xmm0, QWORD PTR .L_1_7[rip]         ; xmm0 - 1.7
    mov     edx, 1                              ; dx = 1
        cvtsd2ss        xmm1, xmm0                  ; xmm0.00 -> xmm1.0
    movaps  xmm0, xmm7                          ; xmm0 = xmm7
        movss   DWORD PTR [rsp-16], xmm1            ; x_vec[3] = xmm1
        
        movaps  xmm6, xmm1                          ; xmm6 = xmm1

    ; x_vec[]:
            ; xmm0 = xmm7 = x_vec[0]
        ; xmm1 = x_vec[3]
        ; xmm2 = x_vec[1]. ??
        ; xmm4 = x_vec[2]. ??
            ; xmm6 = xmm1 = x_vec[3]
        ; xmm7 = x_vec[0]

        ; si = 1
        ; cx = 1
        ; r8 = 1

        ; dx = 1
        ; di = 0

    ; y_vec[]:
        ; xmm8  = xmm12
        ; xmm9  = xmm12
        ; xmm10 = xmm12
        ; xmm11 = xmm12

        jmp     .L7

.L20:
        mov     r11d, r8d
        add     r9d, r8d
        xor     r11d, 1
        mov     ebx, r11d
.L3:
        xor     r11d, r11d
        comiss  xmm4, DWORD PTR .L_5[rip]
        subss   xmm3, xmm7
        movss   xmm6, DWORD PTR [rsp-16]
        subss   xmm1, DWORD PTR [rsp-20]
        subss   xmm5, xmm14
        movaps  xmm4, xmm15
        addss   xmm8, xmm8
        addss   xmm9, xmm9
        setbe   r11b
        addss   xmm10, xmm10
        addss   xmm11, xmm11
        and     edx, r11d
        subss   xmm4, xmm13
        addss   xmm6, xmm5
        xor     r11d, r11d
        comiss  xmm2, DWORD PTR .L_5[rip]
        movss   xmm2, DWORD PTR [rsp-8]
        addss   xmm8, xmm12
        addss   xmm9, xmm12
        addss   xmm4, DWORD PTR [rsp-12]
        addss   xmm10, xmm12
        addss   xmm2, xmm1
        addss   xmm11, xmm12
        setbe   r11b
        and     ecx, r11d
        xor     r11d, r11d
        comiss  xmm0, DWORD PTR .L_5[rip]
        movss   xmm0, DWORD PTR [rsp-4]
        addss   xmm0, xmm3
        setbe   r11b
        and     esi, r11d
        cmp     edx, 1
        adc     ebx, 0
        cmp     ecx, 1
        adc     ebx, 0
        cmp     esi, 1
        adc     ebx, 0
        add     edi, 1
        cmp     ebx, 4
        je      .L12
        cmp     edi, 256
        je      .L12

 ; from L.9 -> L.7
   ;{
    ; x_vec[]:
            ; xmm0 = xmm7 = x_vec[0]        = memcpy(x_vec[0])
        ; xmm1 = x_vec[3]
        ; xmm2 = x_vec[1]. ??
        ; xmm4 = x_vec[2]. ??
            ; xmm6 = xmm1 = x_vec[3]        = memcpy(x_vec[3])
        ; xmm7 = x_vec[0]

        ; si = 1
        ; cx = 1
        ; r8 = 1

        ; dx = 1
        ; di = 0

    ; y_vec[]:
        ; xmm8  = xmm12
        ; xmm9  = xmm12
        ; xmm10 = xmm12
        ; xmm11 = xmm12
  ;}

.L7:

        movaps  xmm13, xmm9     ; xmm13 = y_vec[]       = memcpy(y_vec)     1 - mc_y
        movaps  xmm14, xmm10    ; xmm14 = y_vec[]       = memcpy(y_vec)     2 - mc_y    !!!???

        movaps  xmm3, xmm0      ; xmm3  = x_vec[0]

        mulss   xmm13, xmm9     ; xmm13 = y_vec^2   1 - y_2             ??????????????

        movaps  xmm7, xmm8      ; xmm7  = y_vec[]       = memcpy(y_vec)     3 - mc_y

        movaps  xmm15, xmm4     ; xmm15 = x_vec[2]      = memcpy(x_vec[2])

        mulss   xmm14, xmm10    ; xmm14 = y_vec^2   2 - y_2             ??????????????

        movaps  xmm5, xmm6      ; xmm5  = x_vec[3]      ?? уже было memcpy нужно для дальнейшего в квадрат
        movaps  xmm1, xmm2      ; xmm1  = x_vec[1]      = memcpy(x_vec[1])

        mulss   xmm3, xmm0      ; xmm3 = x_vec[0]^2         0 - x_2
        mulss   xmm7, xmm8      ; xmm7 = y_vec[]^2  3 - y_2

        mulss   xmm15, xmm4     ; xmm15 = x_vec[2]^2        2 - x_2
        mulss   xmm5, xmm6      ; xmm5  = x_vec[3]^2        3 - x_2

        movss   DWORD PTR [rsp-20], xmm13       ; y_vec^2 -> []

        mulss   xmm1, xmm2      ; xmm1 = x_vec[1]^2         1 - x_2

        movaps  xmm13, xmm14    ; xmm13 = xmm14 = y_vec^2               ??????????????
        movaps  xmm14, xmm11    ; xmm14 = y_vec[] !!!???

        mulss   xmm14, xmm11    ; xmm14 = y_vec^2
        mulss   xmm11, xmm6     ; xmm11 = x_vec[3] * y_vec

        movaps  xmm6, xmm3      ; xmm6 = x_vec[0]^2

        addss   xmm6, xmm7      ; xmm6 = x_vec[0]^2 + y_vec[]^2     ; r_2[0]

        comiss  xmm6, DWORD PTR .L_5[rip]       ; cmp r_2[0], r_max

        mulss   xmm10, xmm4                     ; xmm10 = x_vec[2] * y_vec[]
        movss   xmm4, DWORD PTR [rsp-20]        ; xmm4 = y_vec^2    from memory

        mulss   xmm8, xmm0      ; xmm8 = y_vec[] * x_vec[0]

        movaps  xmm0, xmm5      ; xmm0 = x_vec[3]^2

        mulss   xmm9, xmm2      ; xmm9 = y_vec[] * x_vec[1]

        movaps  xmm2, xmm15     ; xmm2 = x_vec[2]^2
        addss   xmm4, xmm1      ; xmm4 = x_vec[1]^2 + y_vec^2       ; r_2[1]

        addss   xmm2, xmm13     ; xmm2 = y_vec^2 + x_vec[2]^2       ; r_2[2]
        addss   xmm0, xmm14     ; xmm0 = y_vec^2 + x_vec[3]^2       ; r_2[3]

        jbe     .L20    ;????

        mov     ebx, 1          ; bx = 1
        xor     r8d, r8d        ; r8 = 0

        jmp     .L3

.L12:
        add     r10d, 4
        cmp     r10d, 800
        jne     .L9
        add     eax, 1
        cmp     eax, 500
        jne     .L2
        mov     eax, r9d

        pop     rbx
        ret



.L_800:
        .long   1145569280         
.L_2_2:
        .long   -1717986918
        .long   1073846681
.L_1_7:
        .long   1073741824
        .long   1073427251
.L_5:
        .long   1084227584                  
.L_500:
        .long   1140457472               
.L_1_74:
        .long   1030792151
        .long   1073469194
.L_0_9:
        .long   -1073741824
        .long   1072483532