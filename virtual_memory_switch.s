.section .text
.global enable_virtual_memory

enable_virtual_memory:
    pushl %eax
    movl 8(%esp), %eax
    movl %eax, %cr3
    movl %cr0, %eax
    or $0x80000000, %eax
    movl %eax, %cr0
    popl %eax
    ret
