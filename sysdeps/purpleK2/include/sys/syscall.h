#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define SYSCALL_INSTRUCTION "int $0x80"
#define SYSCALL_CLOBBERS "memory"

#ifdef __cplusplus
#define __syscall_prefix extern "C" long
#else
#define __syscall_prefix long
#endif


#define DEFINE_SYSCALL0(name, num) \
__syscall_prefix __syscall_##name() { \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL1(name, num, t1) \
__syscall_prefix __syscall_##name(t1 p1) { \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL2(name, num, t1, t2) \
__syscall_prefix __syscall_##name(t1 p1, t2 p2) { \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL3(name, num, t1, t2, t3) \
__syscall_prefix __syscall_##name(t1 p1, t2 p2, t3 p3) { \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL4(name, num, t1, t2, t3, t4) \
__syscall_prefix __syscall_##name(t1 p1, t2 p2, t3 p3, t4 p4) { \
    long ret = num; \
    register long r8 __asm__("r8") = (long)p4; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3), "r"(r8) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL5(name, num, t1, t2, t3, t4, t5) \
__syscall_prefix __syscall_##name(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5) { \
    long ret = num; \
    register long r8 __asm__("r8") = (long)p4; \
    register long r9 __asm__("r9") = (long)p5; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3), \
          "r"(r8), "r"(r9) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}


#define DEFINE_SYSCALL6(name, num, t1, t2, t3, t4, t5, t6) \
__syscall_prefix __syscall_##name(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6) { \
    long ret = num; \
    register long r8  __asm__("r8")  = (long)p4; \
    register long r9  __asm__("r9")  = (long)p5; \
    register long r10 __asm__("r10") = (long)p6; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3), \
          "r"(r8), "r"(r9), "r"(r10) \
        : SYSCALL_CLOBBERS); \
    return ret; \
}

#define SYSCALL0(num) \
({ \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret) \
        : SYSCALL_CLOBBERS); \
    ret; \
})


#define SYSCALL1(num,p1) \
({ \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1) \
        : SYSCALL_CLOBBERS); \
    ret; \
})


#define SYSCALL2(num,p1,p2) \
({ \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2) \
        : SYSCALL_CLOBBERS); \
    ret; \
})


#define SYSCALL3(num,p1,p2,p3) \
({ \
    long ret = num; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3) \
        : SYSCALL_CLOBBERS); \
    ret; \
})


#define SYSCALL4(num,p1,p2,p3,p4) \
({ \
    long ret = num; \
    register long r8 __asm__("r8") = (long)p4; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3), "r"(r8) \
        : SYSCALL_CLOBBERS); \
    ret; \
})


#define SYSCALL5(num,p1,p2,p3,p4,p5) \
({ \
    long ret = num; \
    register long r8 __asm__("r8") = (long)p4; \
    register long r9 __asm__("r9") = (long)p5; \
    asm volatile (SYSCALL_INSTRUCTION \
        : "=a"(ret) \
        : "a"(ret), "D"((long)p1), "S"((long)p2), "d"((long)p3), \
          "r"(r8), "r"(r9) \
        : SYSCALL_CLOBBERS); \
    ret; \
})

#endif /* _SYS_SYSCALL_H */

#ifdef __cplusplus
}
#endif