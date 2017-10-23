#if !defined flagWIN32
#include <sys/times.h>

#ifndef NDLESS
// missing _times function in libc
clock_t _times(struct tms *ptms) {
	return 0;
}
#endif
#endif
