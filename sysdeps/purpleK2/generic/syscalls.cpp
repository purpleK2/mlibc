#include <bits/ensure.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <mlibc/all-sysdeps.hpp>
#include <mlibc/debug.hpp>
#include <sys/syscall.h>
#include <sys/syscall_nums.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <mlibc/arch-defs.hpp>
#include <sys/ioctl.h>

DEFINE_SYSCALL1(exit, SYS_EXIT, int)
DEFINE_SYSCALL3(open, SYS_OPEN, const char *, int, mode_t)
DEFINE_SYSCALL3(read, SYS_READ, int, void *, size_t)
DEFINE_SYSCALL1(close, SYS_CLOSE, int)
DEFINE_SYSCALL3(write, SYS_WRITE, int, const void *, size_t)
DEFINE_SYSCALL3(ioctl, SYS_IOCTL, int, unsigned long, void *)
DEFINE_SYSCALL3(seek, SYS_SEEK, int, off_t, int) // todo: verify that this is the correct order
DEFINE_SYSCALL3(fcntl, SYS_FCNTL, int, int, long)
DEFINE_SYSCALL2(dup, SYS_DUP, int, int)
DEFINE_SYSCALL0(getpid, SYS_GETPID)
DEFINE_SYSCALL0(getuid, SYS_GETUID)
DEFINE_SYSCALL0(geteuid, SYS_GETEUID)
DEFINE_SYSCALL0(getgid, SYS_GETGID)
DEFINE_SYSCALL0(getegid, SYS_GETEGID)
DEFINE_SYSCALL1(setuid, SYS_SETUID, uid_t)
DEFINE_SYSCALL1(seteuid, SYS_SETEUID, uid_t)
DEFINE_SYSCALL2(setreuid, SYS_SETREUID, uid_t, uid_t)
DEFINE_SYSCALL3(setresuid, SYS_SETRESUID, uid_t, uid_t, uid_t)
DEFINE_SYSCALL3(getresuid, SYS_GETRESUID, uid_t *, uid_t *, uid_t *)
DEFINE_SYSCALL1(setgid, SYS_SETGID, gid_t)
DEFINE_SYSCALL1(setegid, SYS_SETEGID, gid_t)
DEFINE_SYSCALL2(setregid, SYS_SETREGID, gid_t, gid_t)
DEFINE_SYSCALL3(setresgid, SYS_SETRESGID, gid_t, gid_t, gid_t)
DEFINE_SYSCALL3(getresgid, SYS_GETRESGID, gid_t *, gid_t *, gid_t *)
DEFINE_SYSCALL0(fork, SYS_FORK)
DEFINE_SYSCALL6(mmap, SYS_MMAP, void *, size_t, int, int, int, off_t)
DEFINE_SYSCALL2(munmap, SYS_MUNMAP, void *, size_t)
DEFINE_SYSCALL3(mprotect, SYS_MPROTECT, void *, size_t, int)
DEFINE_SYSCALL3(msync, SYS_MSYNC, void *, size_t, int)
DEFINE_SYSCALL2(pipe, SYS_PIPE, int *, int)
DEFINE_SYSCALL2(nanosleep, SYS_NANOSLEEP, struct timespec *, struct timespec *)
DEFINE_SYSCALL3(execve, SYS_EXECVE, const char *, const char**, const char**)
DEFINE_SYSCALL3(waitpid, SYS_WAITPID, pid_t, int *, int)
DEFINE_SYSCALL4(wait4, SYS_WAIT4, pid_t, int *, int, struct rusage *)
DEFINE_SYSCALL0(getppid, SYS_GETPPID)
DEFINE_SYSCALL0(getpgrp, SYS_GETPGRP)
DEFINE_SYSCALL2(setpgid, SYS_SETPGID, pid_t, pid_t)
DEFINE_SYSCALL0(getpgid, SYS_GETPGID)
DEFINE_SYSCALL0(setsid, SYS_SETSID)
DEFINE_SYSCALL0(getsid, SYS_GETSID)
DEFINE_SYSCALL1(settls, SYS_SETTLS, void *)
DEFINE_SYSCALL3(poll, SYS_POLL, struct pollfd *, size_t, int)

namespace mlibc {
    void sys_libc_log(const char *message) {
        __syscall_write(3, (char*)message, strlen(message));
        __syscall_write(3, "\n", 1);
    }

    void sys_libc_panic() {
        sys_libc_log("mlibc panic detected!");
        sys_exit(1);
    }

    void sys_exit(int status) {
        __syscall_exit(status);
        sys_libc_panic();
        __builtin_unreachable();
    }

    int sys_isatty(int fd) {
        int is_tty = 0;
        long ret = __syscall_ioctl(fd, IOCTLTTYIS, &is_tty);

        if (ret < 0) {
            return ENOTTY;
        }

        return 0;
    }

    int sys_read(int fd, void *buf, size_t n, ssize_t *bytes_read) {
        long err = __syscall_read(fd, buf, n);

        if (err < 0) {
            return -err;
        }

        *bytes_read = err;
        return 0; 
    }

    int sys_write(int fd, const void *buf, size_t n, ssize_t *bytes_written) {
        ssize_t err = __syscall_write(fd, buf, n);

        if (err < 0) {
            return -err;
        }

        *bytes_written = err;
        return 0;
    }

    int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
        long error = __syscall_seek(fd, offset, whence);

        if (error < 0) {
            return -error;
        }

        *new_offset = error;
        return 0;
    }

    int sys_open(const char *pathname, int flags, mode_t mode, int *fd) {
        int error = __syscall_open(pathname, flags, mode);
        
        if (error < 0) {
            return -error;
        }

        *fd = error;
        return 0;
    }

    int sys_futex_wake(int *futex, bool wake_all) {
        return -ENOSYS;
    }
    
    int sys_futex_wait(int *futex, int expected, const struct timespec *timeout) {
        return -ENOSYS;
    }

    int sys_clock_get(int, long*, long*) {
        return -ENOSYS;
    }

    int sys_close(int fd) {
        return -__syscall_close(fd);
    }

    int sys_mkdir(const char *path, mode_t mode) {
        return -ENOSYS;
    }

    int sys_vm_map(void *addr, size_t size, int prot, int flags, int fd, off_t offset, void** window) {
        void *result = (void*)__syscall_mmap(addr, size, prot, flags, fd, offset);

        if (result == MAP_FAILED) {
            return -errno;
        }

        *window = result;
        return 0;
    }

    int sys_vm_unmap(void *pointer, size_t size) {
        return __syscall_munmap(pointer, size);
    }

    int sys_vm_protect(void *pointer, size_t size, int prot) {
        return __syscall_mprotect(pointer, size, prot);
    }

    int sys_anon_allocate(size_t size, void **pointer) {
        return sys_vm_map(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0, pointer);
    }

    int sys_anon_free(void *pointer, size_t size) {
        return sys_vm_unmap(pointer, size);
    }

    int sys_tcb_set(void *pointer) {
        return __syscall_settls(pointer);
    }

    int sys_waitpid(pid_t pid, int *status, int flags, struct rusage *ru, pid_t *ret_pid) {
        long error = __syscall_waitpid(pid, status, flags);

        if (error < 0)  {
            return -error;
        }

        *ret_pid = (pid_t)error;
        return 0;
    }

    int sys_execve(const char *path, char *const argv[], char *const envp[]) {
        long error = __syscall_execve((char*)path, (const char**)argv, (const char**)envp);
        if (error < 0) {
            return -error;
        }
       
        return 0;
    }

    int sys_fork(pid_t *child) {
        long error = __syscall_fork();
        if (error < 0) return -error;
        *child = error;
        return 0;
    }

    int sys_ioctl(int fd, unsigned long request, void *arg, int *result) {
        long err = __syscall_ioctl(fd, request, arg);
        if (err < 0) {
            return -err;
        }

        *result = err;
        return 0;
    }

    int sys_dup(int fd, int flags, int *newfd) {
        int ret = __syscall_dup(fd, -1);
        if (ret < 0) return -ret;
        *newfd = ret;
        return 0;
    }

    int sys_poll(struct pollfd *fds, nfds_t count, int timeout, int *num_events) {
        long error = __syscall_poll(fds, count, timeout);
        if (error < 0) return -error;
        *num_events = error;
        return 0;
    }
}