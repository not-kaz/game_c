#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>
#include <stdbool.h>

#define UNUSED(_x_) (void)(_x_)
#define IN_RANGE(_x_, _a_, _b_) ((_x_) >= (_a_) && (_x_) <= (_b_))

struct vec2i {
	int x;
	int y;
};

#endif
