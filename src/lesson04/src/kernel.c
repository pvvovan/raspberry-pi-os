#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"

void process(char const *str)
{
	while (1) {
		const char *ch_ptr = str;
		static int counter = 0;

		++counter;
		printf("\r\n%d: ", counter);

		while ('\0' != *ch_ptr) {
			uart_send(*ch_ptr++);
			delay(100000);
		}
		delay(1000000);
	}
}

void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde");
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}

	while (1) {
		schedule();
	}	
}
