.section .rodata
    msg: .ascii "Hello, World!\n"
    .set msglen, (. - msg)

.text
    .global _start

_start:
    # do a write syscall
W:  mov $1, %rax        # syscall number: 1 (write)
    mov $1, %rdi        # arg: write on stdout (1)
    lea msg(%rip), %rsi # arg: buffer
    mov $msglen, %rdx   # arg: buffer length
    syscall

    # do an exit syscall
E:  mov $60, %rax       # syscall number: 60 (exit)
    mov $0, %rdi        # arg: status code 0
    syscall
