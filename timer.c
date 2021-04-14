#include <stdint.h>
#include "timer.h"

static rpi_arm_timer_t *rpiArmTimer = (rpi_arm_timer_t *)ARMTIMER_BASE;
static rpi_sys_timer_t *rpiSystemTimer = (rpi_sys_timer_t *)SYSTIMER_BASE;

rpi_arm_timer_t *get_arm_timer(void)
{
	return rpiArmTimer;
}

rpi_sys_timer_t *get_sys_timer(void)
{
	return rpiSystemTimer;
}

void delay_us(uint32_t us)
{
	volatile uint32_t ts = rpiSystemTimer->counter_lo;
	while((rpiSystemTimer->counter_lo - ts) < us) ;
}
