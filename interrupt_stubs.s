.section .text
.extern handle_interrupt_main

.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interrupt_number)
    jmp common_stub
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num, (interrupt_number)
    jmp common_stub
.endm

HandleInterruptRequest 0x20
HandleInterruptRequest 0x21

common_stub:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushl %esp
    push (interrupt_number)
    call handle_interrupt_main
    mov %eax, %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:

    iret

.section .data
    interrupt_number: .byte 0
