#include <stdarg.h>
#include <stdio.h>
#include "err_msg.h"

#define ERR_MSG_MAXLEN 128

// HACK: This is not thread safe but shouldn't matter to us.
static char err_msg[ERR_MSG_MAXLEN];

void err_msg_set(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(err_msg, ERR_MSG_MAXLEN, fmt, args);
	va_end(args);
}

const char *err_msg_get(void)
{
	return err_msg;
}
