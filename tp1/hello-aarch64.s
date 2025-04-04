.data

msg:
    .ascii  "Hello, World!\n"
    len = . - msg

.text

.globl _start
_start:
    /* do a write syscall */
W:  mov     x0, #1      /* arg: write on stdout (1) */
    ldr     x1, =msg    /* arg: buffer */
    ldr     x2, =len    /* arg: buffer length */
    mov     w8, #64     /* syscall number: 64 (write) */
    svc     #0

    /* do an exit syscall */
E:  mov     x0, #0      /* arg: status code 0 */
    mov     w8, #93     /* syscall number: 93 (exit) */
    svc     #0
