.section .init
.global _init
_init:
	push %rax

.section .fini
.global _fini
_fini:
	push %rax

/* Provide __dso_handle for static builds */
.section .data
.global __dso_handle
.hidden __dso_handle
__dso_handle:
	.quad __dso_handle

.section .note.GNU-stack,"",%progbits
