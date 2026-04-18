#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H 1

#include <mlibc-config.h>
#include <abi-bits/ioctls.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MLIBC_ABI_ONLY

int ioctl(int __fd, unsigned long __request, ...);

#endif /* !__MLIBC_ABI_ONLY */

#define IOCTLTTYIS 0x8002

#define TIOCGETA 19
#define TIOCSETA 20
#define TIOCSETAW 21
#define TIOCSETAF 22

#ifdef __cplusplus
}
#endif

#endif /* _SYS_IOCTL_H */