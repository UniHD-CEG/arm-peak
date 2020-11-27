.macro BEGIN_ARM_FUNCTION name
	.arm
	.globl \name
	.align 2
	.func \name
	\name:
.endm

.macro END_ARM_FUNCTION name
	.endfunc
	.type \name, %function
	.size \name, .-\name
.endm

.syntax unified

.section .text

BEGIN_ARM_FUNCTION mac_simd_f32
loop:
	.rept (KERNEL_SIZE)
		VMLA.F32 q8, q0, q0
		VMLA.F32 q9, q1, q1
		VMLA.F32 q10, q2, q2
		VMLA.F32 q11, q3, q3
		VMLA.F32 q12, q4, q4
		VMLA.F32 q13, q5, q5
		VMLA.F32 q14, q6, q6
		VMLA.F32 q15, q7, q7
	.endr
	SUBS R0,#1
	BNE  loop
	MOV pc, lr
END_ARM_FUNCTION mac_simd_f32

