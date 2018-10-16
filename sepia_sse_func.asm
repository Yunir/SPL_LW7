; attributes
%define b_cs rdi
%define g_cs rsi
%define r_cs rdx
%define result rcx

align 16
maximum: dd 255, 255, 255, 255

%macro return_values 0
    cvtps2dq xmm0, xmm0 ; round float to int
    pminsd xmm0, [maximum] ; min_int(xmm0, xmm6) for cut to 255

    pextrb [result], xmm0, 0 ; write value (extract byte)
    pextrb [result + 1], xmm0, 4
    pextrb [result + 2], xmm0, 8
    pextrb [result + 3], xmm0, 12
%endmacro

align 16
c1_1: dd 0.272, 0.349, 0.393, 0.272
align 16
c2_1: dd 0.543, 0.686, 0.769, 0.543
align 16
c3_1: dd 0.131, 0.168, 0.189, 0.131

align 16
c1_2: dd 0.349, 0.393, 0.272, 0.349
align 16
c2_2: dd 0.686, 0.769, 0.543, 0.686
align 16
c3_2: dd 0.168, 0.189, 0.131, 0.168

align 16
c1_3: dd 0.393, 0.272, 0.349, 0.393
align 16
c2_3: dd 0.769, 0.543, 0.686, 0.769
align 16
c3_3: dd 0.189, 0.131, 0.168, 0.189

section .text
global sepia_sse

sepia_sse:
    ;movaps xmm6, [maximum] ; move aligned packed single-precision floating-point values
    movq xmm0, [b_cs]   ;   b1 b2 b3 b4 set to xmms (move quadword)
    movq xmm1, [g_cs]   ;   g1 g2 g3 g4
    movq xmm2, [r_cs]   ;   r1 r2 r3 r4

    shufps xmm0, xmm0, 0b00000001 ; b1 b1 b1 b2
    shufps xmm1, xmm1, 0b00000001
    shufps xmm2, xmm2, 0b00000001

    ; array of c
    movaps xmm3, [c1_1]
    movaps xmm4, [c2_1]
    movaps xmm5, [c3_1]

    ; multiplication
    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5
    ; summary
    addps  xmm0, xmm1
    addps  xmm0, xmm2

    return_values

    add r_cs, 4
    add g_cs, 4
    add b_cs, 4
    add result, 4

    movq xmm0, [b_cs]
    movq xmm1, [g_cs]
    movq xmm2, [r_cs]
    shufps xmm0, xmm0, 0b00000101
    shufps xmm1, xmm1, 0b00000101
    shufps xmm2, xmm2, 0b00000101

    movaps xmm3, [c1_2]
    movaps xmm4, [c2_2]
    movaps xmm5, [c3_2]
    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5
    addps  xmm0, xmm1
    addps  xmm0, xmm2

    return_values

    add r_cs, 4
    add g_cs, 4
    add b_cs, 4
    add result, 4

    movq   xmm0, [b_cs]
    movq   xmm1, [g_cs]
    movq   xmm2, [r_cs]
    shufps xmm0, xmm0, 0b00010101
    shufps xmm1, xmm1, 0b00010101
    shufps xmm2, xmm2, 0b00010101

    movaps xmm3, [c1_3]
    movaps xmm4, [c2_3]
    movaps xmm5, [c3_3]
    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5
    addps  xmm0, xmm1
    addps  xmm0, xmm2

    return_values
  ret
