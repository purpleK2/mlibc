#ifndef _SYS_MEOWDEV_H
#define _SYS_MEOWDEV_H 1

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MDEVGRABDEV 0x4D01

#define MDEV_KEY 1
#define MDEV_REL 2

#define MDEV_AXIS_X     0x00
#define MDEV_AXIS_Y     0x01
#define MDEV_AXIS_WHEEL 0x08

#define MDEV_BTN_LEFT   0x110
#define MDEV_BTN_RIGHT  0x111
#define MDEV_BTN_MIDDLE 0x112

typedef struct mdev_event {
	struct timeval timestamp;
	unsigned short type;
	unsigned short code;
	unsigned int value;
} __attribute__((packed)) mdev_event_t;

#ifdef __cplusplus
}
#endif

#endif /* _SYS_MEOWDEV_H */
