.section .text
.global jump_to_load_addr
.global SIGKILL

jump_to_load_addr:
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    movl $0xF0000000, %eax
    call %eax
    movl $0x00000001, %eax
    mov %eax, (SIGKILL)

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp

.loop:
    jmp .loop

.section .data
    SIGKILL: .long 0x0
