#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

unsigned int log_level = LOG_LEVEL;

void demo_printf(int level, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (level <= log_level)
		vprintf(fmt, ap);
	va_end(ap);

	return ;
}

int main()
{
	DEMO_ALWAYS("my log level is always\n");
	DEMO_ERR("my log level is err\n");
	DEMO_WARNING("my log level is warning\n");
	DEMO_INFO("my log level is info\n");
	DEMO_DEBUG("my log level is debug\n");
	DEMO_MAX("my log level is max\n");

	demo_printf(_DEMO_ALWAYS_, "%s %d: log level is always\n", __func__, __LINE__);
	demo_printf(_DEMO_ERR_, "%s %d: log level is err\n", __func__, __LINE__);
	demo_printf(_DEMO_WARNING_, "%s %d: log level is warning\n", __func__, __LINE__);
	demo_printf(_DEMO_INFO_, "%s %d: log level is info\n", __func__, __LINE__);
	demo_printf(_DEMO_DEBUG_, "%s %d: log level is debug\n", __func__, __LINE__);
	demo_printf(_DEMO_MAX_, "%s %d: log level is max\n", __func__, __LINE__);

	return 0;
}
