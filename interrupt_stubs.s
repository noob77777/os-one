.section .text
.extern handle_interrupt_main

.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interrupt_number)
    pushl $0
    jmp common_stub
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num, (interrupt_number)
    pushl $0
    jmp common_stub
.endm

HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x20
HandleInterruptRequest 0x21
HandleInterruptRequest 0x22
HandleInterruptRequest 0x23
HandleInterruptRequest 0x24
HandleInterruptRequest 0x25
HandleInterruptRequest 0x26
HandleInterruptRequest 0x27
HandleInterruptRequest 0x28
HandleInterruptRequest 0x29
HandleInterruptRequest 0x2A
HandleInterruptRequest 0x2B
HandleInterruptRequest 0x2C
HandleInterruptRequest 0x2D
HandleInterruptRequest 0x2E
HandleInterruptRequest 0x2F

HandleInterruptRequest 0x80

common_stub:
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    pushl %esp
    push (interrupt_number)
    call handle_interrupt_main
    mov %eax, %esp
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp

    addl $4, %esp

.global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:

    iret

.section .data
    interrupt_number: .byte 0
