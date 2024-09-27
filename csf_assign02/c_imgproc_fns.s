	.file	"c_imgproc_fns.c"
	.text
.Ltext0:
	.file 0 "/home/bwang89/csf_fall_2024/csf_assign02" "c_imgproc_fns.c"
	.globl	get_r
	.type	get_r, @function
get_r:
.LFB6:
	.file 1 "c_imgproc_fns.c"
	.loc 1 12 48
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	.loc 1 13 22
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 13 28
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	.loc 1 13 39
	shrl	$24, %eax
	.loc 1 14 1
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	get_r, .-get_r
	.globl	get_g
	.type	get_g, @function
get_g:
.LFB7:
	.loc 1 15 48
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	.loc 1 16 22
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 16 28
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	.loc 1 16 32
	shrl	$16, %eax
	.loc 1 16 39
	movzbl	%al, %eax
	.loc 1 17 1
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	get_g, .-get_g
	.globl	get_b
	.type	get_b, @function
get_b:
.LFB8:
	.loc 1 18 48
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	.loc 1 19 22
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 19 28
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	.loc 1 19 32
	shrl	$8, %eax
	.loc 1 19 38
	movzbl	%al, %eax
	.loc 1 20 1
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	get_b, .-get_b
	.globl	get_a
	.type	get_a, @function
get_a:
.LFB9:
	.loc 1 21 48
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	.loc 1 22 22
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 22 28
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	.loc 1 22 33
	movzbl	%al, %eax
	.loc 1 23 1
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	get_a, .-get_a
	.globl	make_pixel
	.type	make_pixel, @function
make_pixel:
.LFB10:
	.loc 1 26 66
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	.loc 1 27 13
	movl	-4(%rbp), %eax
	sall	$24, %eax
	movl	%eax, %edx
	.loc 1 27 25
	movl	-8(%rbp), %eax
	sall	$16, %eax
	.loc 1 27 20
	addl	%eax, %edx
	.loc 1 27 37
	movl	-12(%rbp), %eax
	sall	$8, %eax
	.loc 1 27 32
	addl	%eax, %edx
	.loc 1 27 43
	movl	-16(%rbp), %eax
	addl	%edx, %eax
	.loc 1 28 1
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	make_pixel, .-make_pixel
	.globl	to_grayscale
	.type	to_grayscale, @function
to_grayscale:
.LFB11:
	.loc 1 31 55
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -32(%rbp)
	movl	%esi, -36(%rbp)
	.loc 1 32 27
	movl	-36(%rbp), %edx
	movq	-32(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_r
	.loc 1 32 25 discriminator 1
	imull	$79, %eax, %ebx
	.loc 1 32 55 discriminator 1
	movl	-36(%rbp), %edx
	movq	-32(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_g
	.loc 1 32 53 discriminator 2
	sall	$7, %eax
	.loc 1 32 47 discriminator 2
	addl	%eax, %ebx
	.loc 1 32 82 discriminator 2
	movl	-36(%rbp), %edx
	movq	-32(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_b
	.loc 1 32 80 discriminator 3
	imull	$49, %eax, %eax
	.loc 1 32 75 discriminator 3
	addl	%ebx, %eax
	.loc 1 32 12 discriminator 3
	shrl	$8, %eax
	movl	%eax, -12(%rbp)
	.loc 1 33 10
	movl	-36(%rbp), %edx
	movq	-32(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_a
	movl	%eax, %ecx
	.loc 1 33 10 is_stmt 0 discriminator 1
	movl	-12(%rbp), %edx
	movl	-12(%rbp), %esi
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	make_pixel
	.loc 1 34 1 is_stmt 1
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	to_grayscale, .-to_grayscale
	.globl	to_composite
	.type	to_composite, @function
to_composite:
.LFB12:
	.loc 1 37 66
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movl	%edx, -52(%rbp)
	.loc 1 38 29
	movl	-52(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_a
	movl	%eax, -20(%rbp)
	.loc 1 39 41
	movl	-52(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_r
	.loc 1 39 39 discriminator 1
	imull	-20(%rbp), %eax
	movl	%eax, %r12d
	.loc 1 39 60 discriminator 1
	movl	$255, %eax
	subl	-20(%rbp), %eax
	movl	%eax, %ebx
	.loc 1 39 75 discriminator 1
	movl	-52(%rbp), %edx
	movq	-48(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_r
	.loc 1 39 74 discriminator 2
	imull	%ebx, %eax
	.loc 1 39 54 discriminator 2
	addl	%r12d, %eax
	.loc 1 39 14 discriminator 2
	movl	%eax, %edx
	movl	$2155905153, %eax
	imulq	%rdx, %rax
	shrq	$32, %rax
	shrl	$7, %eax
	movl	%eax, -24(%rbp)
	.loc 1 40 41
	movl	-52(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_g
	.loc 1 40 39 discriminator 1
	imull	-20(%rbp), %eax
	movl	%eax, %r12d
	.loc 1 40 60 discriminator 1
	movl	$255, %eax
	subl	-20(%rbp), %eax
	movl	%eax, %ebx
	.loc 1 40 75 discriminator 1
	movl	-52(%rbp), %edx
	movq	-48(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_g
	.loc 1 40 74 discriminator 2
	imull	%ebx, %eax
	.loc 1 40 54 discriminator 2
	addl	%r12d, %eax
	.loc 1 40 14 discriminator 2
	movl	%eax, %edx
	movl	$2155905153, %eax
	imulq	%rdx, %rax
	shrq	$32, %rax
	shrl	$7, %eax
	movl	%eax, -28(%rbp)
	.loc 1 41 41
	movl	-52(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_b
	.loc 1 41 39 discriminator 1
	imull	-20(%rbp), %eax
	movl	%eax, %r12d
	.loc 1 41 60 discriminator 1
	movl	$255, %eax
	subl	-20(%rbp), %eax
	movl	%eax, %ebx
	.loc 1 41 75 discriminator 1
	movl	-52(%rbp), %edx
	movq	-48(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	get_b
	.loc 1 41 74 discriminator 2
	imull	%ebx, %eax
	.loc 1 41 54 discriminator 2
	addl	%r12d, %eax
	.loc 1 41 14 discriminator 2
	movl	%eax, %edx
	movl	$2155905153, %eax
	imulq	%rdx, %rax
	shrq	$32, %rax
	shrl	$7, %eax
	movl	%eax, -32(%rbp)
	.loc 1 42 12
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %esi
	movl	-24(%rbp), %eax
	movl	$255, %ecx
	movl	%eax, %edi
	call	make_pixel
	.loc 1 43 1
	addq	$40, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	to_composite, .-to_composite
	.globl	imgproc_mirror_h
	.type	imgproc_mirror_h, @function
imgproc_mirror_h:
.LFB13:
	.loc 1 52 76
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	.loc 1 54 7
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	.loc 1 55 7
	movq	-24(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -16(%rbp)
	.loc 1 56 3
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %ecx
	movq	-32(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	img_init
.LBB2:
	.loc 1 58 12
	movl	$0, -4(%rbp)
	.loc 1 58 3
	jmp	.L16
.L19:
.LBB3:
	.loc 1 59 14
	movl	$0, -8(%rbp)
	.loc 1 59 5
	jmp	.L17
.L18:
	.loc 1 60 46
	movq	-24(%rbp), %rax
	movq	8(%rax), %rdx
	.loc 1 60 54
	movl	-4(%rbp), %eax
	imull	-12(%rbp), %eax
	movl	%eax, %ecx
	.loc 1 60 67
	movl	-12(%rbp), %eax
	subl	-8(%rbp), %eax
	.loc 1 60 69
	subl	$1, %eax
	.loc 1 60 60
	addl	%ecx, %eax
	cltq
	.loc 1 60 52
	salq	$2, %rax
	leaq	(%rdx,%rax), %rcx
	.loc 1 60 17
	movq	-32(%rbp), %rax
	movq	8(%rax), %rdx
	.loc 1 60 25
	movl	-4(%rbp), %eax
	imull	-12(%rbp), %eax
	movl	%eax, %esi
	.loc 1 60 31
	movl	-8(%rbp), %eax
	addl	%esi, %eax
	cltq
	.loc 1 60 23
	salq	$2, %rax
	addq	%rax, %rdx
	.loc 1 60 52
	movl	(%rcx), %eax
	.loc 1 60 35
	movl	%eax, (%rdx)
	.loc 1 59 33 discriminator 3
	addl	$1, -8(%rbp)
.L17:
	.loc 1 59 23 discriminator 1
	movl	-8(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jl	.L18
.LBE3:
	.loc 1 58 32 discriminator 2
	addl	$1, -4(%rbp)
.L16:
	.loc 1 58 21 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-16(%rbp), %eax
	jl	.L19
.LBE2:
	.loc 1 63 1
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	imgproc_mirror_h, .-imgproc_mirror_h
	.globl	imgproc_mirror_v
	.type	imgproc_mirror_v, @function
imgproc_mirror_v:
.LFB14:
	.loc 1 72 76
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	.loc 1 74 7
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	.loc 1 75 7
	movq	-24(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -16(%rbp)
	.loc 1 76 3
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %ecx
	movq	-32(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	img_init
.LBB4:
	.loc 1 78 12
	movl	$0, -4(%rbp)
	.loc 1 78 3
	jmp	.L21
.L24:
.LBB5:
	.loc 1 79 14
	movl	$0, -8(%rbp)
	.loc 1 79 5
	jmp	.L22
.L23:
	.loc 1 80 46
	movq	-24(%rbp), %rax
	movq	8(%rax), %rdx
	.loc 1 80 60
	movl	-16(%rbp), %eax
	subl	-4(%rbp), %eax
	.loc 1 80 62
	subl	$1, %eax
	.loc 1 80 66
	imull	-12(%rbp), %eax
	movl	%eax, %ecx
	.loc 1 80 73
	movl	-8(%rbp), %eax
	addl	%ecx, %eax
	cltq
	.loc 1 80 52
	salq	$2, %rax
	leaq	(%rdx,%rax), %rcx
	.loc 1 80 17
	movq	-32(%rbp), %rax
	movq	8(%rax), %rdx
	.loc 1 80 25
	movl	-4(%rbp), %eax
	imull	-12(%rbp), %eax
	movl	%eax, %esi
	.loc 1 80 31
	movl	-8(%rbp), %eax
	addl	%esi, %eax
	cltq
	.loc 1 80 23
	salq	$2, %rax
	addq	%rax, %rdx
	.loc 1 80 52
	movl	(%rcx), %eax
	.loc 1 80 35
	movl	%eax, (%rdx)
	.loc 1 79 33 discriminator 3
	addl	$1, -8(%rbp)
.L22:
	.loc 1 79 23 discriminator 1
	movl	-8(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jl	.L23
.LBE5:
	.loc 1 78 32 discriminator 2
	addl	$1, -4(%rbp)
.L21:
	.loc 1 78 21 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-16(%rbp), %eax
	jl	.L24
.LBE4:
	.loc 1 83 1
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	imgproc_mirror_v, .-imgproc_mirror_v
	.globl	imgproc_tile
	.type	imgproc_tile, @function
imgproc_tile:
.LFB15:
	.loc 1 98 78
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movl	%esi, -76(%rbp)
	movq	%rdx, -88(%rbp)
	.loc 1 100 6
	cmpl	$0, -76(%rbp)
	jle	.L26
	.loc 1 100 29 discriminator 1
	movq	-72(%rbp), %rax
	movl	(%rax), %eax
	.loc 1 100 13 discriminator 1
	cmpl	%eax, -76(%rbp)
	jg	.L26
	.loc 1 100 53 discriminator 2
	movq	-72(%rbp), %rax
	movl	4(%rax), %eax
	.loc 1 100 37 discriminator 2
	cmpl	%eax, -76(%rbp)
	jle	.L27
.L26:
	.loc 1 101 12
	movl	$0, %eax
	jmp	.L28
.L27:
	.loc 1 103 7
	movq	-72(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -44(%rbp)
	.loc 1 104 7
	movq	-72(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -48(%rbp)
	.loc 1 105 3
	movl	-48(%rbp), %edx
	movl	-44(%rbp), %ecx
	movq	-88(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	img_init
	.loc 1 108 7
	movl	-48(%rbp), %eax
	cltd
	idivl	-76(%rbp)
	movl	%edx, -52(%rbp)
	.loc 1 109 7
	movl	-44(%rbp), %eax
	cltd
	idivl	-76(%rbp)
	movl	%edx, -56(%rbp)
.LBB6:
	.loc 1 112 12
	movl	$0, -4(%rbp)
	.loc 1 112 3
	jmp	.L29
.L41:
.LBB7:
	.loc 1 113 14
	movl	$0, -8(%rbp)
	.loc 1 113 5
	jmp	.L30
.L40:
.LBB8:
	.loc 1 115 11
	movl	$0, -12(%rbp)
	.loc 1 116 11
	movl	$0, -16(%rbp)
.LBB9:
	.loc 1 118 16
	movl	$0, -20(%rbp)
	.loc 1 118 7
	jmp	.L31
.L39:
.LBB10:
	.loc 1 120 13
	movl	$0, -24(%rbp)
	.loc 1 121 13
	movl	$0, -28(%rbp)
	.loc 1 122 13
	movl	$0, -32(%rbp)
	.loc 1 124 23
	movl	-56(%rbp), %eax
	subl	$1, %eax
	.loc 1 124 12
	cmpl	%eax, -20(%rbp)
	jge	.L32
	.loc 1 125 15
	movl	$1, -32(%rbp)
	jmp	.L33
.L32:
	.loc 1 127 15
	movl	$0, -32(%rbp)
.L33:
	.loc 1 129 19
	movl	$0, -16(%rbp)
.LBB11:
	.loc 1 130 18
	movl	$0, -36(%rbp)
	.loc 1 130 9
	jmp	.L34
.L38:
.LBB12:
	.loc 1 131 15
	movl	$0, -40(%rbp)
	.loc 1 133 25
	movl	-52(%rbp), %eax
	subl	$1, %eax
	.loc 1 133 14
	cmpl	%eax, -36(%rbp)
	jge	.L35
	.loc 1 134 18
	movl	$1, -40(%rbp)
.L35:
	.loc 1 137 13
	movl	-20(%rbp), %eax
	cmpl	-56(%rbp), %eax
	jl	.L36
	.loc 1 137 25 discriminator 1
	movl	-8(%rbp), %eax
	cmpl	-76(%rbp), %eax
	jl	.L36
	.loc 1 137 35 discriminator 2
	cmpl	$0, -56(%rbp)
	je	.L36
	.loc 1 138 21
	movl	$1, -24(%rbp)
.L36:
	.loc 1 140 13
	movl	-36(%rbp), %eax
	cmpl	-52(%rbp), %eax
	jl	.L37
	.loc 1 140 25 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-76(%rbp), %eax
	jl	.L37
	.loc 1 140 35 discriminator 2
	cmpl	$0, -52(%rbp)
	je	.L37
	.loc 1 141 21
	movl	$1, -28(%rbp)
.L37:
	.loc 1 144 94
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	.loc 1 144 110
	movl	-24(%rbp), %eax
	imull	-76(%rbp), %eax
	.loc 1 144 102
	movl	-8(%rbp), %ecx
	movl	%ecx, %esi
	subl	%eax, %esi
	.loc 1 144 123
	movl	-28(%rbp), %eax
	imull	-76(%rbp), %eax
	.loc 1 144 115
	movl	-4(%rbp), %ecx
	subl	%eax, %ecx
	.loc 1 144 126
	movl	%ecx, %eax
	imull	-44(%rbp), %eax
	.loc 1 144 112
	addl	%esi, %eax
	cltq
	.loc 1 144 100
	salq	$2, %rax
	leaq	(%rdx,%rax), %rcx
	.loc 1 144 21
	movq	-88(%rbp), %rax
	movq	8(%rax), %rsi
	.loc 1 144 31
	movl	-8(%rbp), %eax
	cltd
	idivl	-76(%rbp)
	movl	%eax, %edi
	.loc 1 144 38
	movl	-4(%rbp), %eax
	imull	-44(%rbp), %eax
	.loc 1 144 45
	cltd
	idivl	-76(%rbp)
	.loc 1 144 34
	leal	(%rdi,%rax), %edx
	.loc 1 144 48
	movl	-12(%rbp), %eax
	addl	%eax, %edx
	.loc 1 144 68
	movl	-44(%rbp), %eax
	imull	-16(%rbp), %eax
	.loc 1 144 60
	addl	%edx, %eax
	cltq
	.loc 1 144 27
	salq	$2, %rax
	leaq	(%rsi,%rax), %rdx
	.loc 1 144 100
	movl	(%rcx), %eax
	.loc 1 144 83
	movl	%eax, (%rdx)
	.loc 1 146 31
	movl	-48(%rbp), %eax
	cltd
	idivl	-76(%rbp)
	movl	%eax, %edx
	.loc 1 146 35
	movl	-40(%rbp), %eax
	addl	%edx, %eax
	.loc 1 146 21
	addl	%eax, -16(%rbp)
.LBE12:
	.loc 1 130 33 discriminator 2
	addl	$1, -36(%rbp)
.L34:
	.loc 1 130 27 discriminator 1
	movl	-36(%rbp), %eax
	cmpl	-76(%rbp), %eax
	jl	.L38
.LBE11:
	.loc 1 149 28
	movl	-44(%rbp), %eax
	cltd
	idivl	-76(%rbp)
	movl	%eax, %edx
	.loc 1 149 32
	movl	-32(%rbp), %eax
	addl	%edx, %eax
	.loc 1 149 19
	addl	%eax, -12(%rbp)
.LBE10:
	.loc 1 118 31 discriminator 2
	addl	$1, -20(%rbp)
.L31:
	.loc 1 118 25 discriminator 1
	movl	-20(%rbp), %eax
	cmpl	-76(%rbp), %eax
	jl	.L39
.LBE9:
.LBE8:
	.loc 1 113 34 discriminator 2
	movl	-76(%rbp), %eax
	addl	%eax, -8(%rbp)
.L30:
	.loc 1 113 23 discriminator 1
	movl	-8(%rbp), %eax
	cmpl	-44(%rbp), %eax
	jl	.L40
.LBE7:
	.loc 1 112 33 discriminator 2
	movl	-76(%rbp), %eax
	addl	%eax, -4(%rbp)
.L29:
	.loc 1 112 21 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-48(%rbp), %eax
	jl	.L41
.LBE6:
	.loc 1 154 10
	movl	$1, %eax
.L28:
	.loc 1 155 1
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	imgproc_tile, .-imgproc_tile
	.globl	imgproc_grayscale
	.type	imgproc_grayscale, @function
imgproc_grayscale:
.LFB16:
	.loc 1 164 77
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	.loc 1 165 7
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -24(%rbp)
	.loc 1 166 7
	movq	-40(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -28(%rbp)
	.loc 1 167 3
	movl	-28(%rbp), %edx
	movl	-24(%rbp), %ecx
	movq	-48(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	img_init
.LBB13:
	.loc 1 169 12
	movl	$0, -20(%rbp)
	.loc 1 169 3
	jmp	.L43
.L44:
	.loc 1 170 15
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 170 21
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	leaq	(%rax,%rdx), %rbx
	.loc 1 170 27
	movl	-20(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	to_grayscale
	.loc 1 170 25 discriminator 1
	movl	%eax, (%rbx)
	.loc 1 169 38 discriminator 3
	addl	$1, -20(%rbp)
.L43:
	.loc 1 169 29 discriminator 1
	movl	-28(%rbp), %eax
	imull	-24(%rbp), %eax
	.loc 1 169 21 discriminator 1
	cmpl	%eax, -20(%rbp)
	jl	.L44
.LBE13:
	.loc 1 172 1
	nop
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	imgproc_grayscale, .-imgproc_grayscale
	.globl	imgproc_composite
	.type	imgproc_composite, @function
imgproc_composite:
.LFB17:
	.loc 1 186 102
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	.loc 1 188 15
	movq	-40(%rbp), %rax
	movl	(%rax), %edx
	.loc 1 188 37
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	.loc 1 188 6
	cmpl	%eax, %edx
	jne	.L46
	.loc 1 188 56 discriminator 1
	movq	-40(%rbp), %rax
	movl	4(%rax), %edx
	.loc 1 188 79 discriminator 1
	movq	-48(%rbp), %rax
	movl	4(%rax), %eax
	.loc 1 188 45 discriminator 1
	cmpl	%eax, %edx
	je	.L47
.L46:
	.loc 1 189 12
	movl	$0, %eax
	jmp	.L48
.L47:
	.loc 1 191 7
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -24(%rbp)
	.loc 1 192 7
	movq	-40(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -28(%rbp)
	.loc 1 193 3
	movl	-28(%rbp), %edx
	movl	-24(%rbp), %ecx
	movq	-56(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	img_init
.LBB14:
	.loc 1 195 12
	movl	$0, -20(%rbp)
	.loc 1 195 3
	jmp	.L49
.L50:
	.loc 1 196 15
	movq	-56(%rbp), %rax
	movq	8(%rax), %rax
	.loc 1 196 21
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	leaq	(%rax,%rdx), %rbx
	.loc 1 196 27
	movl	-20(%rbp), %edx
	movq	-40(%rbp), %rcx
	movq	-48(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	to_composite
	.loc 1 196 25 discriminator 1
	movl	%eax, (%rbx)
	.loc 1 195 38 discriminator 3
	addl	$1, -20(%rbp)
.L49:
	.loc 1 195 29 discriminator 1
	movl	-28(%rbp), %eax
	imull	-24(%rbp), %eax
	.loc 1 195 21 discriminator 1
	cmpl	%eax, -20(%rbp)
	jl	.L50
.LBE14:
	.loc 1 198 10
	movl	$1, %eax
.L48:
	.loc 1 199 1
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	imgproc_composite, .-imgproc_composite
.Letext0:
	.file 2 "/usr/include/bits/types.h"
	.file 3 "/usr/include/bits/stdint-intn.h"
	.file 4 "/usr/include/bits/stdint-uintn.h"
	.file 5 "image.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x6af
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0xe
	.long	.LASF47
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF2
	.uleb128 0xf
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF3
	.uleb128 0x6
	.byte	0x8
	.byte	0x5
	.long	.LASF4
	.uleb128 0x6
	.byte	0x1
	.byte	0x8
	.long	.LASF5
	.uleb128 0x6
	.byte	0x2
	.byte	0x7
	.long	.LASF6
	.uleb128 0x6
	.byte	0x4
	.byte	0x7
	.long	.LASF7
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x6
	.byte	0x2
	.byte	0x5
	.long	.LASF9
	.uleb128 0x7
	.long	.LASF10
	.byte	0x2
	.byte	0x29
	.byte	0x14
	.long	0x35
	.uleb128 0x7
	.long	.LASF11
	.byte	0x2
	.byte	0x2a
	.byte	0x16
	.long	0x58
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF12
	.uleb128 0x7
	.long	.LASF13
	.byte	0x3
	.byte	0x1a
	.byte	0x13
	.long	0x6d
	.uleb128 0x6
	.byte	0x8
	.byte	0x7
	.long	.LASF14
	.uleb128 0x7
	.long	.LASF15
	.byte	0x4
	.byte	0x1a
	.byte	0x14
	.long	0x79
	.uleb128 0x10
	.long	.LASF48
	.byte	0x10
	.byte	0x5
	.byte	0xe
	.byte	0x8
	.long	0xdd
	.uleb128 0xa
	.long	.LASF16
	.byte	0xf
	.byte	0xb
	.long	0x8c
	.byte	0
	.uleb128 0xa
	.long	.LASF17
	.byte	0x10
	.byte	0xb
	.long	0x8c
	.byte	0x4
	.uleb128 0xa
	.long	.LASF18
	.byte	0x11
	.byte	0xd
	.long	0xdd
	.byte	0x8
	.byte	0
	.uleb128 0xd
	.long	0x9f
	.uleb128 0x11
	.long	.LASF49
	.byte	0x5
	.byte	0x23
	.byte	0x5
	.long	0x35
	.long	0x102
	.uleb128 0xb
	.long	0x102
	.uleb128 0xb
	.long	0x8c
	.uleb128 0xb
	.long	0x8c
	.byte	0
	.uleb128 0xd
	.long	0xab
	.uleb128 0x8
	.long	.LASF23
	.byte	0xba
	.byte	0x5
	.long	0x35
	.quad	.LFB17
	.quad	.LFE17-.LFB17
	.uleb128 0x1
	.byte	0x9c
	.long	0x18e
	.uleb128 0x2
	.long	.LASF19
	.byte	0xba
	.byte	0x26
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x2
	.long	.LASF20
	.byte	0xba
	.byte	0x3e
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x2
	.long	.LASF21
	.byte	0xba
	.byte	0x59
	.long	0x102
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x1
	.long	.LASF16
	.byte	0xbf
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1
	.long	.LASF17
	.byte	0xc0
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x3
	.quad	.LBB14
	.quad	.LBE14-.LBB14
	.uleb128 0x5
	.string	"i"
	.byte	0xc3
	.byte	0xc
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.byte	0
	.uleb128 0xc
	.long	.LASF32
	.byte	0xa4
	.quad	.LFB16
	.quad	.LFE16-.LFB16
	.uleb128 0x1
	.byte	0x9c
	.long	0x201
	.uleb128 0x2
	.long	.LASF22
	.byte	0xa4
	.byte	0x27
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x2
	.long	.LASF21
	.byte	0xa4
	.byte	0x40
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x1
	.long	.LASF16
	.byte	0xa5
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1
	.long	.LASF17
	.byte	0xa6
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x3
	.quad	.LBB13
	.quad	.LBE13-.LBB13
	.uleb128 0x5
	.string	"i"
	.byte	0xa9
	.byte	0xc
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.byte	0
	.uleb128 0x8
	.long	.LASF24
	.byte	0x62
	.byte	0x5
	.long	0x35
	.quad	.LFB15
	.quad	.LFE15-.LFB15
	.uleb128 0x1
	.byte	0x9c
	.long	0x38a
	.uleb128 0x2
	.long	.LASF22
	.byte	0x62
	.byte	0x21
	.long	0x102
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x4
	.string	"n"
	.byte	0x62
	.byte	0x30
	.long	0x35
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x2
	.long	.LASF21
	.byte	0x62
	.byte	0x41
	.long	0x102
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0x1
	.long	.LASF16
	.byte	0x67
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x1
	.long	.LASF17
	.byte	0x68
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x1
	.long	.LASF25
	.byte	0x6c
	.byte	0x7
	.long	0x35
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1
	.long	.LASF26
	.byte	0x6d
	.byte	0x7
	.long	0x35
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x3
	.quad	.LBB6
	.quad	.LBE6-.LBB6
	.uleb128 0x5
	.string	"i"
	.byte	0x70
	.byte	0xc
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x3
	.quad	.LBB7
	.quad	.LBE7-.LBB7
	.uleb128 0x5
	.string	"j"
	.byte	0x71
	.byte	0xe
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x3
	.quad	.LBB8
	.quad	.LBE8-.LBB8
	.uleb128 0x1
	.long	.LASF27
	.byte	0x73
	.byte	0xb
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1
	.long	.LASF28
	.byte	0x74
	.byte	0xb
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x3
	.quad	.LBB9
	.quad	.LBE9-.LBB9
	.uleb128 0x5
	.string	"k"
	.byte	0x76
	.byte	0x10
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x3
	.quad	.LBB10
	.quad	.LBE10-.LBB10
	.uleb128 0x1
	.long	.LASF29
	.byte	0x78
	.byte	0xd
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1
	.long	.LASF30
	.byte	0x79
	.byte	0xd
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x5
	.string	"add"
	.byte	0x7a
	.byte	0xd
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x3
	.quad	.LBB11
	.quad	.LBE11-.LBB11
	.uleb128 0x5
	.string	"l"
	.byte	0x82
	.byte	0x12
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x3
	.quad	.LBB12
	.quad	.LBE12-.LBB12
	.uleb128 0x1
	.long	.LASF31
	.byte	0x83
	.byte	0xf
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xc
	.long	.LASF33
	.byte	0x48
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.uleb128 0x1
	.byte	0x9c
	.long	0x41b
	.uleb128 0x2
	.long	.LASF22
	.byte	0x48
	.byte	0x26
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x2
	.long	.LASF21
	.byte	0x48
	.byte	0x3f
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1
	.long	.LASF16
	.byte	0x4a
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1
	.long	.LASF17
	.byte	0x4b
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x3
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0x5
	.string	"i"
	.byte	0x4e
	.byte	0xc
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x3
	.quad	.LBB5
	.quad	.LBE5-.LBB5
	.uleb128 0x5
	.string	"j"
	.byte	0x4f
	.byte	0xe
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xc
	.long	.LASF34
	.byte	0x34
	.quad	.LFB13
	.quad	.LFE13-.LFB13
	.uleb128 0x1
	.byte	0x9c
	.long	0x4ac
	.uleb128 0x2
	.long	.LASF22
	.byte	0x34
	.byte	0x26
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x2
	.long	.LASF21
	.byte	0x34
	.byte	0x3f
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1
	.long	.LASF16
	.byte	0x36
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1
	.long	.LASF17
	.byte	0x37
	.byte	0x7
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x3
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x5
	.string	"i"
	.byte	0x3a
	.byte	0xc
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x3
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x5
	.string	"j"
	.byte	0x3b
	.byte	0xe
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x8
	.long	.LASF35
	.byte	0x25
	.byte	0xa
	.long	0x9f
	.quad	.LFB12
	.quad	.LFE12-.LFB12
	.uleb128 0x1
	.byte	0x9c
	.long	0x52d
	.uleb128 0x4
	.string	"fg"
	.byte	0x25
	.byte	0x25
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x4
	.string	"bg"
	.byte	0x25
	.byte	0x37
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x4
	.string	"i"
	.byte	0x25
	.byte	0x3f
	.long	0x35
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1
	.long	.LASF36
	.byte	0x26
	.byte	0xe
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1
	.long	.LASF37
	.byte	0x27
	.byte	0xe
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1
	.long	.LASF38
	.byte	0x28
	.byte	0xe
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x1
	.long	.LASF39
	.byte	0x29
	.byte	0xe
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.byte	0
	.uleb128 0x8
	.long	.LASF40
	.byte	0x1f
	.byte	0xa
	.long	0x9f
	.quad	.LFB11
	.quad	.LFE11-.LFB11
	.uleb128 0x1
	.byte	0x9c
	.long	0x577
	.uleb128 0x2
	.long	.LASF22
	.byte	0x1f
	.byte	0x25
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x4
	.string	"i"
	.byte	0x1f
	.byte	0x34
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x1
	.long	.LASF41
	.byte	0x20
	.byte	0xc
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x9
	.long	.LASF42
	.byte	0x1a
	.long	0x9f
	.quad	.LFB10
	.quad	.LFE10-.LFB10
	.uleb128 0x1
	.byte	0x9c
	.long	0x5c8
	.uleb128 0x4
	.string	"r"
	.byte	0x1a
	.byte	0x1e
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x4
	.string	"g"
	.byte	0x1a
	.byte	0x29
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x4
	.string	"b"
	.byte	0x1a
	.byte	0x34
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x4
	.string	"a"
	.byte	0x1a
	.byte	0x3f
	.long	0x9f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x9
	.long	.LASF43
	.byte	0x15
	.long	0x9f
	.quad	.LFB9
	.quad	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.long	0x603
	.uleb128 0x2
	.long	.LASF22
	.byte	0x15
	.byte	0x1e
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x4
	.string	"i"
	.byte	0x15
	.byte	0x2d
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x9
	.long	.LASF44
	.byte	0x12
	.long	0x9f
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x63e
	.uleb128 0x2
	.long	.LASF22
	.byte	0x12
	.byte	0x1e
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x4
	.string	"i"
	.byte	0x12
	.byte	0x2d
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x9
	.long	.LASF45
	.byte	0xf
	.long	0x9f
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.long	0x679
	.uleb128 0x2
	.long	.LASF22
	.byte	0xf
	.byte	0x1e
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x4
	.string	"i"
	.byte	0xf
	.byte	0x2d
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x12
	.long	.LASF46
	.byte	0x1
	.byte	0xc
	.byte	0xa
	.long	0x9f
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x2
	.long	.LASF22
	.byte	0xc
	.byte	0x1e
	.long	0x102
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x4
	.string	"i"
	.byte	0xc
	.byte	0x2d
	.long	0x35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 10
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 5
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF12:
	.string	"char"
.LASF45:
	.string	"get_g"
.LASF26:
	.string	"w_mod"
.LASF34:
	.string	"imgproc_mirror_h"
.LASF10:
	.string	"__int32_t"
.LASF28:
	.string	"h_counter"
.LASF33:
	.string	"imgproc_mirror_v"
.LASF32:
	.string	"imgproc_grayscale"
.LASF41:
	.string	"target"
.LASF18:
	.string	"data"
.LASF46:
	.string	"get_r"
.LASF5:
	.string	"unsigned char"
.LASF2:
	.string	"long unsigned int"
.LASF6:
	.string	"short unsigned int"
.LASF31:
	.string	"add2"
.LASF17:
	.string	"height"
.LASF37:
	.string	"target_r"
.LASF29:
	.string	"xoffset"
.LASF19:
	.string	"base_img"
.LASF21:
	.string	"output_img"
.LASF35:
	.string	"to_composite"
.LASF11:
	.string	"__uint32_t"
.LASF30:
	.string	"yoffset"
.LASF43:
	.string	"get_a"
.LASF25:
	.string	"h_mod"
.LASF7:
	.string	"unsigned int"
.LASF14:
	.string	"long long unsigned int"
.LASF36:
	.string	"foreground_a"
.LASF13:
	.string	"int32_t"
.LASF22:
	.string	"input_img"
.LASF4:
	.string	"long long int"
.LASF47:
	.string	"GNU C17 14.2.1 20240801 (Red Hat 14.2.1-1) -mtune=generic -march=x86-64 -g"
.LASF39:
	.string	"target_b"
.LASF49:
	.string	"img_init"
.LASF16:
	.string	"width"
.LASF48:
	.string	"Image"
.LASF9:
	.string	"short int"
.LASF23:
	.string	"imgproc_composite"
.LASF24:
	.string	"imgproc_tile"
.LASF15:
	.string	"uint32_t"
.LASF3:
	.string	"long int"
.LASF40:
	.string	"to_grayscale"
.LASF20:
	.string	"overlay_img"
.LASF8:
	.string	"signed char"
.LASF38:
	.string	"target_g"
.LASF42:
	.string	"make_pixel"
.LASF27:
	.string	"w_counter"
.LASF44:
	.string	"get_b"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"c_imgproc_fns.c"
.LASF1:
	.string	"/home/bwang89/csf_fall_2024/csf_assign02"
	.ident	"GCC: (GNU) 14.2.1 20240801 (Red Hat 14.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
