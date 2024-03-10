	.text
	.file	"sysy"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -4(%rsp)
	movl	$5, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	b,@object                       # @b
	.data
	.globl	b
	.p2align	2
b:
	.long	2                               # 0x2
	.size	b, 4

	.type	a,@object                       # @a
	.globl	a
	.p2align	2
a:
	.long	123                             # 0x7b
	.size	a, 4

	.section	".note.GNU-stack","",@progbits
