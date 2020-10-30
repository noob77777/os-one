.set MAGIC, 0x1badb002
.set FLAGS, 0x00
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .align 4 
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernel_main
.extern init_constructors
.global loader
loader:
    mov $kernel_stack, %esp
    mov %esp, %ebp
    call init_constructors
    push %eax
    push %ebx
    call kernel_main
_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024 ; # 2 MiB
kernel_stack:
