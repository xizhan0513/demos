ifndef CONFIG_LOG_LEVEL
CONFIG_LOG_LEVEL = 4
endif

all:
	make demo_printf
demo_printf: demo_printf.c
	@gcc -o demo_printf demo_printf.c -DLOG_LEVEL=$(CONFIG_LOG_LEVEL)
clean:
	@rm demo_printf
