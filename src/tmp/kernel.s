	.file	"kernel.cpp"
	.text
	.section	.text._ZN17SegmentDescriptorC2Ejjh,"axG",@progbits,_ZN17SegmentDescriptorC5Ejjh,comdat
	.align 2
	.weak	_ZN17SegmentDescriptorC2Ejjh
	.type	_ZN17SegmentDescriptorC2Ejjh, @function
_ZN17SegmentDescriptorC2Ejjh:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$20, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	20(%ebp), %eax
	movb	%al, -20(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	shrl	$12, 16(%ebp)
	movl	-4(%ebp), %eax
	addl	$6, %eax
	movb	$-64, (%eax)
	movl	16(%ebp), %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	movb	%dl, (%eax)
	movl	16(%ebp), %eax
	shrl	$8, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movb	%dl, (%eax)
	movl	-4(%ebp), %eax
	addl	$6, %eax
	movzbl	(%eax), %edx
	movl	16(%ebp), %eax
	shrl	$16, %eax
	andl	$15, %eax
	movl	%eax, %ecx
	movl	-4(%ebp), %eax
	addl	$6, %eax
	orl	%ecx, %edx
	movb	%dl, (%eax)
	movl	-4(%ebp), %eax
	addl	$2, %eax
	movl	12(%ebp), %edx
	movb	%dl, (%eax)
	movl	12(%ebp), %eax
	shrl	$8, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	addl	$3, %eax
	movb	%dl, (%eax)
	movl	12(%ebp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	addl	$4, %eax
	movb	%dl, (%eax)
	movl	12(%ebp), %eax
	shrl	$24, %eax
	movl	%eax, %edx
	movl	-4(%ebp), %eax
	addl	$7, %eax
	movb	%dl, (%eax)
	movl	-4(%ebp), %eax
	leal	5(%eax), %edx
	movzbl	-20(%ebp), %eax
	movb	%al, (%edx)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	_ZN17SegmentDescriptorC2Ejjh, .-_ZN17SegmentDescriptorC2Ejjh
	.weak	_ZN17SegmentDescriptorC1Ejjh
	.set	_ZN17SegmentDescriptorC1Ejjh,_ZN17SegmentDescriptorC2Ejjh
	.section	.text._ZN21GlobalDescriptorTableC2Ev,"axG",@progbits,_ZN21GlobalDescriptorTableC5Ev,comdat
	.align 2
	.weak	_ZN21GlobalDescriptorTableC2Ev
	.type	_ZN21GlobalDescriptorTableC2Ev, @function
_ZN21GlobalDescriptorTableC2Ev:
.LFB6:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	$2, %ebx
	movl	%eax, %esi
.L4:
	testl	%ebx, %ebx
	js	.L3
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	%esi
	call	_ZN17SegmentDescriptorC1Ejjh
	addl	$16, %esp
	addl	$8, %esi
	subl	$1, %ebx
	jmp	.L4
.L3:
	pushl	$0
	pushl	$0
	pushl	$0
	leal	-32(%ebp), %eax
	pushl	%eax
	call	_ZN17SegmentDescriptorC1Ejjh
	addl	$16, %esp
	movl	8(%ebp), %ecx
	movl	-32(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	pushl	$154
	pushl	$-1
	pushl	$0
	leal	-24(%ebp), %eax
	pushl	%eax
	call	_ZN17SegmentDescriptorC1Ejjh
	addl	$16, %esp
	movl	8(%ebp), %ecx
	movl	-24(%ebp), %eax
	movl	-20(%ebp), %edx
	movl	%eax, 8(%ecx)
	movl	%edx, 12(%ecx)
	pushl	$146
	pushl	$-1
	pushl	$0
	leal	-16(%ebp), %eax
	pushl	%eax
	call	_ZN17SegmentDescriptorC1Ejjh
	addl	$16, %esp
	movl	8(%ebp), %ecx
	movl	-16(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%eax, 16(%ecx)
	movl	%edx, 20(%ecx)
	movl	8(%ebp), %eax
	movl	$1507328, 24(%eax)
	movl	8(%ebp), %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movl	%edx, 28(%eax)
	movl	8(%ebp), %eax
	addl	$24, %eax
	addl	$2, %eax
#APP
# 81 "src/include/gdt.h" 1
	lgdt (%eax)
# 0 "" 2
#NO_APP
	nop
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	_ZN21GlobalDescriptorTableC2Ev, .-_ZN21GlobalDescriptorTableC2Ev
	.weak	_ZN21GlobalDescriptorTableC1Ev
	.set	_ZN21GlobalDescriptorTableC1Ev,_ZN21GlobalDescriptorTableC2Ev
	.local	_ZL12x_coordinate
	.comm	_ZL12x_coordinate,4,4
	.local	_ZL12y_coordinate
	.comm	_ZL12y_coordinate,4,4
	.section	.rodata
	.align 4
	.type	_ZL12VIDEO_MEMORY, @object
	.size	_ZL12VIDEO_MEMORY, 4
_ZL12VIDEO_MEMORY:
	.long	753664
.LC0:
	.string	"0123456789ABCDEF"
	.section	.data.rel.local,"aw"
	.align 4
	.type	_ZL3hex, @object
	.size	_ZL3hex, 4
_ZL3hex:
	.long	.LC0
	.text
	.globl	_Z7kprintfPKc
	.type	_Z7kprintfPKc, @function
_Z7kprintfPKc:
.LFB8:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -8(%ebp)
.L15:
	movl	-8(%ebp), %ecx
	movl	8(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%edx), %edx
	testb	%dl, %dl
	je	.L16
	movl	-8(%ebp), %ecx
	movl	8(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%edx), %edx
	movsbl	%dl, %edx
	cmpl	$10, %edx
	jne	.L7
	movl	$0, _ZL12x_coordinate@GOTOFF(%eax)
	movl	_ZL12y_coordinate@GOTOFF(%eax), %edx
	addl	$1, %edx
	movl	%edx, _ZL12y_coordinate@GOTOFF(%eax)
	jmp	.L8
.L7:
	movl	-8(%ebp), %ecx
	movl	8(%ebp), %edx
	leal	(%ecx,%edx), %ebx
	movl	_ZL12y_coordinate@GOTOFF(%eax), %ecx
	movl	%ecx, %edx
	sall	$2, %edx
	addl	%ecx, %edx
	sall	$4, %edx
	movl	%edx, %ecx
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	addl	%ecx, %edx
	addl	%edx, %edx
	leal	753664(%edx), %ecx
	movzbl	(%ebx), %edx
	movb	%dl, (%ecx)
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	addl	$1, %edx
	movl	%edx, _ZL12x_coordinate@GOTOFF(%eax)
	nop
.L8:
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	cmpl	$79, %edx
	jle	.L9
	movl	$0, _ZL12x_coordinate@GOTOFF(%eax)
	movl	_ZL12y_coordinate@GOTOFF(%eax), %edx
	addl	$1, %edx
	movl	%edx, _ZL12y_coordinate@GOTOFF(%eax)
.L9:
	movl	_ZL12y_coordinate@GOTOFF(%eax), %edx
	cmpl	$24, %edx
	jle	.L10
	movl	$0, _ZL12y_coordinate@GOTOFF(%eax)
.L14:
	movl	_ZL12y_coordinate@GOTOFF(%eax), %edx
	cmpl	$24, %edx
	jg	.L11
	movl	$0, _ZL12x_coordinate@GOTOFF(%eax)
.L13:
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	cmpl	$79, %edx
	jg	.L12
	movl	_ZL12y_coordinate@GOTOFF(%eax), %ecx
	movl	%ecx, %edx
	sall	$2, %edx
	addl	%ecx, %edx
	sall	$4, %edx
	movl	%edx, %ecx
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	addl	%ecx, %edx
	addl	%edx, %edx
	addl	$753664, %edx
	movb	$32, (%edx)
	movl	_ZL12x_coordinate@GOTOFF(%eax), %edx
	addl	$1, %edx
	movl	%edx, _ZL12x_coordinate@GOTOFF(%eax)
	jmp	.L13
.L12:
	movl	_ZL12y_coordinate@GOTOFF(%eax), %edx
	addl	$1, %edx
	movl	%edx, _ZL12y_coordinate@GOTOFF(%eax)
	jmp	.L14
.L11:
	movl	$0, _ZL12x_coordinate@GOTOFF(%eax)
	movl	$0, _ZL12y_coordinate@GOTOFF(%eax)
.L10:
	addl	$1, -8(%ebp)
	jmp	.L15
.L16:
	nop
	addl	$16, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	_Z7kprintfPKc, .-_Z7kprintfPKc
	.section	.rodata
.LC1:
	.string	"0x"
	.text
	.globl	_Z12kprintf_hex8hb
	.type	_Z12kprintf_hex8hb, @function
_Z12kprintf_hex8hb:
.LFB9:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %ecx
	movl	12(%ebp), %edx
	movb	%cl, -20(%ebp)
	movb	%dl, -24(%ebp)
	movl	$0, -4(%ebp)
	movl	_ZL3hex@GOTOFF(%eax), %ecx
	movzbl	-20(%ebp), %edx
	shrb	$4, %dl
	movzbl	%dl, %edx
	addl	%ecx, %edx
	movzbl	(%edx), %ecx
	movl	-4(%ebp), %edx
	movb	%cl, (%edx)
	movl	_ZL3hex@GOTOFF(%eax), %edx
	movzbl	-20(%ebp), %ecx
	andl	$15, %ecx
	addl	%ecx, %edx
	movl	-4(%ebp), %ecx
	addl	$1, %ecx
	movzbl	(%edx), %edx
	movb	%dl, (%ecx)
	cmpb	$0, -24(%ebp)
	je	.L18
	leal	.LC1@GOTOFF(%eax), %eax
	pushl	%eax
	call	_Z7kprintfPKc
	addl	$4, %esp
.L18:
	pushl	-4(%ebp)
	call	_Z7kprintfPKc
	addl	$4, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	_Z12kprintf_hex8hb, .-_Z12kprintf_hex8hb
	.globl	_Z11kprintf_hexj
	.type	_Z11kprintf_hexj, @function
_Z11kprintf_hexj:
.LFB10:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	leal	.LC1@GOTOFF(%eax), %eax
	pushl	%eax
	call	_Z7kprintfPKc
	addl	$4, %esp
	movl	8(%ebp), %eax
	shrl	$24, %eax
	movzbl	%al, %eax
	pushl	$0
	pushl	%eax
	call	_Z12kprintf_hex8hb
	addl	$8, %esp
	movl	8(%ebp), %eax
	shrl	$16, %eax
	movzbl	%al, %eax
	pushl	$0
	pushl	%eax
	call	_Z12kprintf_hex8hb
	addl	$8, %esp
	movl	8(%ebp), %eax
	shrl	$8, %eax
	movzbl	%al, %eax
	pushl	$0
	pushl	%eax
	call	_Z12kprintf_hex8hb
	addl	$8, %esp
	movl	8(%ebp), %eax
	movzbl	%al, %eax
	pushl	$0
	pushl	%eax
	call	_Z12kprintf_hex8hb
	addl	$8, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	_Z11kprintf_hexj, .-_Z11kprintf_hexj
	.globl	init_constructors
	.type	init_constructors, @function
init_constructors:
.LFB11:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	start_ctors@GOT(%ebx), %eax
	movl	%eax, -12(%ebp)
.L22:
	movl	end_ctors@GOT(%ebx), %eax
	cmpl	%eax, -12(%ebp)
	je	.L23
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	call	*%eax
	addl	$4, -12(%ebp)
	jmp	.L22
.L23:
	nop
	addl	$20, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	init_constructors, .-init_constructors
	.globl	GDT
	.bss
	.align 32
	.type	GDT, @object
	.size	GDT, 32
GDT:
	.zero	32
	.section	.rodata
	.align 4
.LC2:
	.string	"OS-ONE (version 0.0.1-target=i386)"
	.text
	.globl	kernel_main
	.type	kernel_main, @function
kernel_main:
.LFB12:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	pushl	$1
	pushl	$43
	call	_Z12kprintf_hex8hb
	addl	$8, %esp
	leal	.LC2@GOTOFF(%ebx), %eax
	pushl	%eax
	call	_Z7kprintfPKc
	addl	$4, %esp
.L25:
	jmp	.L25
	.cfi_endproc
.LFE12:
	.size	kernel_main, .-kernel_main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB13:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	cmpl	$1, 8(%ebp)
	jne	.L28
	cmpl	$65535, 12(%ebp)
	jne	.L28
	subl	$12, %esp
	leal	GDT@GOTOFF(%eax), %eax
	pushl	%eax
	call	_ZN21GlobalDescriptorTableC1Ev
	addl	$16, %esp
.L28:
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z7kprintfPKc, @function
_GLOBAL__sub_I__Z7kprintfPKc:
.LFB14:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	subl	$8, %esp
	pushl	$65535
	pushl	$1
	call	_Z41__static_initialization_and_destruction_0ii
	addl	$16, %esp
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	_GLOBAL__sub_I__Z7kprintfPKc, .-_GLOBAL__sub_I__Z7kprintfPKc
	.section	.init_array,"aw"
	.align 4
	.long	_GLOBAL__sub_I__Z7kprintfPKc
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB15:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE15:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB16:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE16:
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
