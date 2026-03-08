.section .text
.global _start
_start:
    // The kernel is stupid and doesn't usually align our stack
    // So align the stack to 16-byte boundary (SSE instructions and whatnot need this)
    mov %rsp, %rdi
    and $0xFFFFFFFFFFFFFFF0, %rsp


    mov $main, %rsi
    call __mlibc_entry
.section .note.GNU-stack,"",%progbits