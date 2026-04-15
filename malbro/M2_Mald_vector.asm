.LCPI0_0:
        .word   2
        .word   3
main:
        sub     sp, sp, #16                     ; int in stack = volatile no_skip_O3

        mov     w8, #0                          ; w8 = 0 = y_0 <=> add w8, wzr, #0

        mov     w9, #1140457472                 ; w9 = 500 = OX_OX
        fmov    s0, w9                          ; s0 = (float)w9
    
        mov     w9, #47186                      ; 0000 47186
        movk    w9, #16350, lsl #16             ; 16350 47186
        fmov    s1, w9                          ; s1 = 1.74

        mov     w9, #1145569280                 ; w9 = 800
        dup     v2.4s, w9                       ; v2s = {800.0f, 800.0f, 800.0f, 800.0f}
        ; V2 = vector register 
        ; 4S = 4 lines каждая по single word    ; v2s
        ; dup_licate

        mov     w9, #52429                                           
        movk    w9, #16396, lsl #16             ; w9 = 2.2f
        dup     v3.4s, w9                       ; v3s = {2.2f, 2.2f, 2.2f, 2.2f}

        mov     w9, #26214
        movk    w9, #48998, lsl #16             ; w9 = -0.9f
        fmov    s4, w9                          ; s4 = float(w9)

        adrp    x9, .LCPI0_0                    ; Address of Data Relative Page = {2, 3}
        ldr     d5, [x9, :lo12:.LCPI0_0]        ; Load Register // d5 = {2, 3}                  // v5 !!!

        mov     w9, #39322
        movk    w9, #49113, lsl #16
        dup     v6.4s, w9                       ; v6 = -1.7f

        str     wzr, [sp, #12]                  ; zero register -> int no_skip_O3 = 0; (stack need // 16)
        ; sp + 0
        ; sp + 4
        ; sp + 8
        ; sp + 12   = no_skip
        ; sp + 16

        fmov    v7.4s, #5.00000000              ; v7 = {5.0f, 5.0f, 5.0f, 5.0f} = r2_max

        b       .LBB0_2                         ; jmp in for(y_0)

.LBB0_1:
        add     w8, w8, #1                      ; y_0++
        cmp     w8, #500                        ; y_0 == 500?
        b.eq    .LBB0_7                         ; je end

.LBB0_2:
        mov     x9, #0                          ; x9 = 0 = x_0
        scvtf   s16, w8                         ; s16 = (float) y_0 

    ; делает один раз тк для всех 4 чисел одинаковые y_vec
        fdiv    s16, s16, s0                    ; y_0/500
        fmul    s16, s16, s1                    ; y_0/500 * 1.74
        fadd    s16, s16, s4                    ; y_0/500 * 1.74 - 0.9f = y_vec
                                                ;                 0 1 
        dup     v16.4s, v16.s[0]                ; v16 = {y_vec, y_vec, y_vec, y_vec}
        b       .LBB0_4

.LBB0_3:
        ldr     w11, [sp, #12]                  ; w11 = no_ski[] = pop w11 push w11 
        add     w10, w10, w11                   ; w10 = no_skip + color
        str     w10, [sp, #12]                  ; push no_skip

;???
        add     x10, x9, #4                     ; x10 = x_0 + 4

        cmp     x9, #796                        ; x_0 >< 796
        mov     x9, x10                         ; x_0 = x10

        b.hs    .LBB0_1

.LBB0_4:

        ; w9 = x_0

        mov     w10, #0                         ; w10 = 0 = Clr_vec[0] ???
        mov     w11, #0                         ; w11 = 0 = how_much_iter

        ; faster then add!!!
        ; 0  = ...0000
        ; 4  = ...0100
        ; 8  = ...1000
        ; 12 = ...1100                          ; кратно 4 => last 2 bit = 0

        orr     w12, w9, #0x1                   ; w12 = w9 | 1 = x_0 + 1
        dup     v17.2s, w9                      ; v17 = {x_0, x_0, trash, trash}

        ; d5 = {2, 3}
        orr     v17.8b, v17.8b, v5.8b           ; v17 = {x_0 + 2, x_0 + 3}
        fmov    s18, w9                         ; s18 = x_0

        mov     v18.s[1], w12                   ; v18 = {x_0, x_0 + 1}      s = 32b

        ; take low 64 of v17    --->> v18 64 hight <=> [1]
        mov     v18.d[1], v17.d[0]              ; v18 = {x_0, x_0 + 1, x_0 + 2, x_0 + 3}      d = 64bit = double
        scvtf   v17.4s, v18.4s                  ; Signed Convert to Float (float)int
        ;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; не домножает на 1)

        ; v2s = {800.0f, 800.0f, 800.0f, 800.0f}
        ; v3s = {2.2f, 2.2f, 2.2f, 2.2f}
        ; v6 = -1.7f
        fdiv    v17.4s, v17.4s, v2.4s           ; x_0 / 800
        fmul    v17.4s, v17.4s, v3.4s           ; x_0 / 800 * 2.2
        fadd    v17.4s, v17.4s, v6.4s           ; x_0 / 800 * 2.2 - 1.7 = x_vec

        movi    v20.4s, #1                      ; v20 = {1, 1, 1, 1} = inc_vec

        mov     v19.16b, v16.16b                ; v19 = v16 = {y_vec, y_vec, y_vec, y_vec} = memcpy
        mov     v18.16b, v17.16b                ; v18 = v17 = {x_vec0, x_vec1, x_vec2, x_vec3} = memcpy

.LBB0_5:
        fmul    v21.4s, v18.4s, v18.4s          ; v21 = {x_vec^2}
        fmul    v22.4s, v19.4s, v19.4s          ; v22 = {y_vec^2}

        fadd    v23.4s, v21.4s, v22.4s          ; v23 = {r_2_vec}

        fcmgt   v23.4s, v23.4s, v7.4s          ; == upgrate if_else !!!
        ; v23 = {7, 2, 9, 1}
        ; v7  = {5, 5, 5, 5}    = r_2_max
        ; Floating-point Compare Greater Than   ; v23[i] > v7[i] ?
        ; v23 = {1, 0, 1, 0}

        mvn     v24.16b, v23.16b                ; v24 = inversiya of v23 = {0, 1, 0, 1}

        cmtst   v24.4s, v20.4s, v24.4s          ; Compare Test bits == & + (= 0?) => проверяем inc_vec на 0 для while

        ; Bitwise bit clear
        bic     v20.16b, v20.16b, v23.16b       ; inc & v24 = выключаем у вылетевщих

        ; берем 1 эл и ++ к 
        fmov    w12, s20
        add     w10, w12, w10                   ; Clr_vec[0] = Clr_vec[0] + inc_vec[0];

        xtn     v23.4h, v24.4s                  ; 32bit mask ->> 16 bit mask    // Extract Narrow
        
        umaxv   h23, v23.4h                     ; max of v23.4h --> h23 (младшие 16 бит v23) => if find 1(tbz) => next step 

        fmov    w12, s23                        ; here max of v23 = v24 = -v23 = inc_vect!!

    ; смотрит не каждое а берет максимальное и сравнивает с нулем (вместо цикла с проверкой каждого с 0 и доп переменной для счетчика) 
        tbz     w12, #0, .LBB0_3                ; Test Bit and Branch if Zero  //  
        
        fmul    v19.4s, v18.4s, v19.4s          ; v19 = xy
        fsub    v18.4s, v21.4s, v22.4s          ; v18 = x^2 - y^2
        fadd    v18.4s, v18.4s, v17.4s          ; x_now = x^2 - y^2 + x_0
        fadd    v19.4s, v19.4s, v19.4s          ; v19 = 2xy
        fadd    v19.4s, v19.4s, v16.4s          ; y_now = 2xy + y_0

        add     w12, w11, #1                    ; w12 = how_much_iter++
        cmp     w11, #255                       ; w11 = how_much_iter < 255?
        mov     x11, x12                        ; update how_much_iter

        b.lo    .LBB0_5                         ; jump if < 256
        b       .LBB0_3                         ; jump 

.LBB0_7:
        mov     w0, #0                  ; return 0;
        add     sp, sp, #16             ; return stack 16 from int no_skip_O3

        ret                             ; from main
