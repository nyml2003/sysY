	.text
	.file	"sysy"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$233, -4(%rsp)
	movl	$3, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	a,@object                       # @a
	.data
	.globl	a
	.p2align	2
a:
	.long	1                               # 0x1
	.size	a, 4

	.type	b,@object                       # @b
	.globl	b
	.p2align	2
b:
	.long	2                               # 0x2
	.size	b, 4

	.section	".note.GNU-stack","",@progbits
