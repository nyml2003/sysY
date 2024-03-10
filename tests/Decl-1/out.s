	.text
	.file	"sysy"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$15129, -4(%rsp)                # imm = 0x3B19
	movl	$246, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	a,@object                       # @a
	.section	.rodata,"a",@progbits
	.globl	a
	.p2align	2
a:
	.long	123                             # 0x7b
	.size	a, 4

	.section	".note.GNU-stack","",@progbits
