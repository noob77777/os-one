.set MAGIC, 0x1BADB002
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
    call _setup_segments
    movl 0x0, %eax
    pushl %eax
    call kernel_main
_stop:
    cli
    hlt
    jmp _stop

_setup_segments:
    jmp $0x08, $.next
.next:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

.section .bss
.space 8*1024*1024 ; # 8 MiB
kernel_stack:
.space 1024 ; # 1 KiB
kernel_heap:
