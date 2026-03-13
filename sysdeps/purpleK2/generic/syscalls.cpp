#include "mlibc/posix-sysdeps.hpp"
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
#include <poll.h>

DEFINE_SYSCALL1(exit, SYS_EXIT, int)
DEFINE_SYSCALL3(open, SYS_OPEN, const char *, int, mode_t)
DEFINE_SYSCALL3(read, SYS_READ, int, void *, size_t)
DEFINE_SYSCALL1(close, SYS_CLOSE, int)
DEFINE_SYSCALL3(write, SYS_WRITE, int, const void *, size_t)
DEFINE_SYSCALL3(ioctl, SYS_IOCTL, int, unsigned long, void *)
DEFINE_SYSCALL3(seek, SYS_SEEK, int, off_t, int)
DEFINE_SYSCALL3(fcntl, SYS_FCNTL, int, int, long)
DEFINE_SYSCALL2(dup, SYS_DUP, int, int)

DEFINE_SYSCALL0(getpid, SYS_GETPID)
DEFINE_SYSCALL0(getppid, SYS_GETPPID)

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

DEFINE_SYSCALL0(getpgrp, SYS_GETPGRP)
DEFINE_SYSCALL2(setpgid, SYS_SETPGID, pid_t, pid_t)
DEFINE_SYSCALL1(getpgid, SYS_GETPGID, pid_t)
DEFINE_SYSCALL0(setsid, SYS_SETSID)
DEFINE_SYSCALL1(getsid, SYS_GETSID, pid_t)

DEFINE_SYSCALL0(fork, SYS_FORK)
DEFINE_SYSCALL3(execve, SYS_EXECVE, const char *, const char**, const char**)
DEFINE_SYSCALL3(waitpid, SYS_WAITPID, pid_t, int *, int)
DEFINE_SYSCALL4(wait4, SYS_WAIT4, pid_t, int *, int, struct rusage *)

DEFINE_SYSCALL6(mmap, SYS_MMAP, void *, size_t, int, int, int, off_t)
DEFINE_SYSCALL2(munmap, SYS_MUNMAP, void *, size_t)
DEFINE_SYSCALL3(mprotect, SYS_MPROTECT, void *, size_t, int)
DEFINE_SYSCALL3(msync, SYS_MSYNC, void *, size_t, int)

DEFINE_SYSCALL2(pipe, SYS_PIPE, int *, int)

DEFINE_SYSCALL2(nanosleep, SYS_NANOSLEEP, struct timespec *, struct timespec *)

DEFINE_SYSCALL1(settls, SYS_SETTLS, void *)

DEFINE_SYSCALL3(poll, SYS_POLL, struct pollfd *, size_t, int)

DEFINE_SYSCALL2(mkdir, SYS_MKDIR, const char *, mode_t)
DEFINE_SYSCALL1(rmdir, SYS_RMDIR, const char *)
DEFINE_SYSCALL1(remove, SYS_REMOVE, const char *)
DEFINE_SYSCALL2(symlink, SYS_SYMLINK, const char *, const char *)
DEFINE_SYSCALL3(readlink, SYS_READLINK, const char *, char *, size_t)

DEFINE_SYSCALL5(mount, SYS_MOUNT, const char *, const char *, const char *, unsigned long, const void *)
DEFINE_SYSCALL2(umount, SYS_UMOUNT, const char *, int)

DEFINE_SYSCALL1(opendir, SYS_OPEN_DIR, const char *)
DEFINE_SYSCALL3(readdir, SYS_READ_DIR, int, void *, size_t)
DEFINE_SYSCALL1(closedir, SYS_CLOSE_DIR, int)

DEFINE_SYSCALL2(getcwd, SYS_GETCWD, char *, size_t)

DEFINE_SYSCALL3(futex_wait, SYS_FUTEX_WAIT, int *, int, const struct timespec *)
DEFINE_SYSCALL2(futex_wake, SYS_FUTEX_WAKE, int *, int)

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
        sys_libc_log("mlibc: exit syscall returned unexpectedly");
        sys_libc_panic();
        __builtin_unreachable();
    }

    int sys_open(const char *pathname, int flags, mode_t mode, int *fd) {
        long error = __syscall_open(pathname, flags, mode);
        if (error < 0) {
            return -error;
        }
        *fd = error;
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

    int sys_close(int fd) {
        long ret = __syscall_close(fd);
        if (ret < 0) {
            return -ret;
        }
        return 0;
    }

    int sys_dup(int fd, int flags, int *newfd) {
        (void)flags;
        long ret = __syscall_dup(fd, -1);
        if (ret < 0) return -ret;
        *newfd = ret;
        return 0;
    }

    int sys_dup2(int fd, int flags, int newfd) {
        (void)flags;
        long ret = __syscall_dup(fd, newfd);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_isatty(int fd) {
        int is_tty = 0;
        long ret = __syscall_ioctl(fd, IOCTLTTYIS, &is_tty);
        if (ret < 0) {
            return ENOTTY;
        }
        return 0;
    }

    int sys_tcgetattr(int fd, struct termios *attr) {
        int result;
        return sys_ioctl(fd, TIOCGETA, attr, &result);
    }

    int sys_tcsetattr(int fd, int optional_action, const struct termios *attr) {
        int request;
        switch (optional_action) {
            case TCSANOW:
                request = TIOCSETA;
                break;
            case TCSADRAIN:
                request = TIOCSETAW;
                break;
            case TCSAFLUSH:
                request = TIOCSETAF;
                break;
            default:
                return EINVAL;
        }

        int result;
        return sys_ioctl(fd, request, const_cast<struct termios *>(attr), &result);
    }

    int sys_ioctl(int fd, unsigned long request, void *arg, int *result) {
        long err = __syscall_ioctl(fd, request, arg);
        if (err < 0) {
            return -err;
        }
        *result = err;
        return 0;
    }

    int sys_fcntl(int fd, int request, va_list args, int *result) {
        long arg = va_arg(args, long);
        long ret = __syscall_fcntl(fd, request, arg);
        if (ret < 0) {
            return -ret;
        }
        *result = ret;
        return 0;
    }

    pid_t sys_getpid() {
        return __syscall_getpid();
    }

    pid_t sys_getppid() {
        return __syscall_getppid();
    }

    uid_t sys_getuid() {
        return __syscall_getuid();
    }

    uid_t sys_geteuid() {
        return __syscall_geteuid();
    }

    gid_t sys_getgid() {
        return __syscall_getgid();
    }

    gid_t sys_getegid() {
        return __syscall_getegid();
    }

    int sys_setuid(uid_t uid) {
        long ret = __syscall_setuid(uid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_seteuid(uid_t euid) {
        long ret = __syscall_seteuid(euid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setgid(gid_t gid) {
        long ret = __syscall_setgid(gid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setegid(gid_t egid) {
        long ret = __syscall_setegid(egid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setreuid(uid_t ruid, uid_t euid) {
        long ret = __syscall_setreuid(ruid, euid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setregid(gid_t rgid, gid_t egid) {
        long ret = __syscall_setregid(rgid, egid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setresuid(uid_t ruid, uid_t euid, uid_t suid) {
        long ret = __syscall_setresuid(ruid, euid, suid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setresgid(gid_t rgid, gid_t egid, gid_t sgid) {
        long ret = __syscall_setresgid(rgid, egid, sgid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_getresuid(uid_t *ruid, uid_t *euid, uid_t *suid) {
        long ret = __syscall_getresuid(ruid, euid, suid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid) {
        long ret = __syscall_getresgid(rgid, egid, sgid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_getpgid(pid_t pid, pid_t *pgid) {
        long ret = __syscall_getpgid(pid);
        if (ret < 0) return -ret;
        *pgid = ret;
        return 0;
    }

    int sys_setpgid(pid_t pid, pid_t pgid) {
        long ret = __syscall_setpgid(pid, pgid);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_setsid(pid_t *sid) {
        long ret = __syscall_setsid();
        if (ret < 0) return -ret;
        *sid = ret;
        return 0;
    }

    int sys_getsid(pid_t pid, pid_t *sid) {
        long ret = __syscall_getsid(pid);
        if (ret < 0) return -ret;
        *sid = ret;
        return 0;
    }

    int sys_fork(pid_t *child) {
        long error = __syscall_fork();
        if (error < 0) return -error;
        *child = error;
        return 0;
    }

    int sys_execve(const char *path, char *const argv[], char *const envp[]) {
        long error = __syscall_execve((char*)path, (const char**)argv, (const char**)envp);
        if (error < 0) {
            return -error;
        }
        return 0;
    }

    int sys_waitpid(pid_t pid, int *status, int flags, struct rusage *ru, pid_t *ret_pid) {
        long error;
        if (ru) {
            error = __syscall_wait4(pid, status, flags, ru);
        } else {
            error = __syscall_waitpid(pid, status, flags);
        }
        if (error < 0) {
            return -error;
        }
        *ret_pid = (pid_t)error;
        return 0;
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
        long ret = __syscall_munmap(pointer, size);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_vm_protect(void *pointer, size_t size, int prot) {
        long ret = __syscall_mprotect(pointer, size, prot);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_msync(void *addr, size_t length, int flags) {
        long ret = __syscall_msync(addr, length, flags);
        if (ret < 0) return -ret;
        return 0;
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

    int sys_pipe(int *fds, int flags) {
        long ret = __syscall_pipe(fds, flags);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_poll(struct pollfd *fds, nfds_t count, int timeout, int *num_events) {
        long error = __syscall_poll(fds, count, timeout);
        if (error < 0) return -error;
        *num_events = error;
        return 0;
    }

    int sys_sleep(time_t *secs, long *nanos) {
        struct timespec req = { *secs, *nanos };
        struct timespec rem = { 0, 0 };
        
        long ret = __syscall_nanosleep(&req, &rem);
        if (ret < 0) {
            *secs = rem.tv_sec;
            *nanos = rem.tv_nsec;
            return -ret;
        }
        *secs = 0;
        *nanos = 0;
        return 0;
    }

    int sys_clock_get(int clock, time_t *secs, long *nanos) {
        (void)clock;
        (void)secs;
        (void)nanos;
        return ENOSYS;
    }

    int sys_open_dir(const char *path, int *handle) {
        long ret = __syscall_opendir(path);
        if (ret < 0) return -ret;
        *handle = ret;
        return 0;
    }

    int sys_read_entries(int handle, void *buffer, size_t max_size, size_t *bytes_read) {
        long ret = __syscall_readdir(handle, buffer, max_size);
        if (ret < 0) return -ret;
        *bytes_read = ret;
        return 0;
    }

    int sys_mkdir(const char *path, mode_t mode) {
        long ret = __syscall_mkdir(path, mode);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_rmdir(const char *path) {
        long ret = __syscall_rmdir(path);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_symlink(const char *target_path, const char *link_path) {
        long ret = __syscall_symlink(target_path, link_path);
        if (ret < 0) return -ret;
        return 0;
    }

    int sys_readlink(const char *path, void *buffer, size_t max_size, ssize_t *length) {
        long ret = __syscall_readlink(path, (char*)buffer, max_size);
        if (ret < 0) return -ret;
        *length = ret;
        return 0;
    }

    int sys_unlinkat(int dirfd, const char *path, int flags) {
        if (dirfd == AT_FDCWD) {
            if (flags & AT_REMOVEDIR) {
                return sys_rmdir(path);
            }
            long ret = __syscall_remove(path);
            if (ret < 0) return -ret;
            return 0;
        }
        return ENOSYS;
    }

    int sys_futex_wake(int *futex, bool wake_all) {
        return __syscall_futex_wake(futex, wake_all ? 1 : 0);
    }
    
    int sys_futex_wait(int *futex, int expected, const struct timespec *timeout) {
        return __syscall_futex_wait(futex, expected, timeout);
    }

    int sys_futex_tid() {
        return 1;
    }

    int sys_getcwd(char *buffer, size_t size) {
        return __syscall_getcwd(buffer, size);
    }

} // namespace mlibc