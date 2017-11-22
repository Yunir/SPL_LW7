%define triple_first_last 0b00000001 ; 00000001
%define dup_first_dup_second 0b00000101
%define first_triple_second 0b00010101

%define b_block rdi
%define g_block rsi
%define r_block rdx
%define dst rcx

%macro return_values 0
    cvtps2dq xmm0, xmm0 ; convert packed single-precision floating-point values to packed signed doubleword integer values
    pminsd xmm0, xmm6 ; cut to 255

    pextrb [dst], xmm0, 0 ; return values (extract bytes)
    pextrb [dst + 1], xmm0, 4
    pextrb [dst + 2], xmm0, 8
    pextrb [dst + 3], xmm0, 12
%endmacro

; B = bc11 + gc12 + rc13
; G = bc21 + gc22 + rc23
; R = bc31 + gc32 + rc33

; 393 769 189
; 349 686 168
; 272 543 131

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

align 16
maximum: dd 255, 255, 255, 255

section .text
global sepia_sse_block

sepia_sse_block:
    movaps xmm6, [maximum] ; move aligned packed single-precision floating-point values
    movq   xmm0, [b_block] ;   b1 b2 b3 b4    set to xmms (move quadword)
    movq   xmm1, [g_block] ;   g1 g2 g3 g4
    movq   xmm2, [r_block] ;   r1 r2 r3 r4

    shufps xmm0, xmm0, triple_first_last ;packed interleave shuffle of quadruplets of single-precision floating-point values
    shufps xmm1, xmm1, triple_first_last
    shufps xmm2, xmm2, triple_first_last

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

    add r_block, 4
    add g_block, 4
    add b_block, 4
    add dst, 4

    movq   xmm0, [b_block]
    movq   xmm1, [g_block]
    movq   xmm2, [r_block]
    shufps xmm0, xmm0, dup_first_dup_second
    shufps xmm1, xmm1, dup_first_dup_second
    shufps xmm2, xmm2, dup_first_dup_second

    movaps xmm3, [c1_2]
    movaps xmm4, [c2_2]
    movaps xmm5, [c3_2]
    mulps  xmm0, xmm3
    mulps  xmm1, xmm4
    mulps  xmm2, xmm5
    addps  xmm0, xmm1
    addps  xmm0, xmm2

    return_values

    add r_block, 4
    add g_block, 4
    add b_block, 4
    add dst, 4

    movq   xmm0, [b_block]
    movq   xmm1, [g_block]
    movq   xmm2, [r_block]
    shufps xmm0, xmm0, first_triple_second
    shufps xmm1, xmm1, first_triple_second
    shufps xmm2, xmm2, first_triple_second

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
