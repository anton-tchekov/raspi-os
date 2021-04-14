#ifndef RPI_ARMTIMER_H
#define RPI_ARMTIMER_H

#include <stdint.h>
#include "base.h"

/* ARM Timer */
#define ARMTIMER_BASE               (PERIPHERAL_BASE + 0xB400)
#define ARMTIMER_CTRL_23BIT         (1 << 1)
#define ARMTIMER_CTRL_PRESCALE_1    (0 << 2)
#define ARMTIMER_CTRL_PRESCALE_16   (1 << 2)
#define ARMTIMER_CTRL_PRESCALE_256  (2 << 2)
#define ARMTIMER_CTRL_INT_ENABLE    (1 << 5)
#define ARMTIMER_CTRL_INT_DISABLE   (0 << 5)
#define ARMTIMER_CTRL_ENABLE        (1 << 7)
#define ARMTIMER_CTRL_DISABLE       (0 << 7)

typedef struct
{
	volatile uint32_t Load;
	volatile uint32_t Value;
	volatile uint32_t Control;
	volatile uint32_t IRQClear;
	volatile uint32_t RAWIRQ;
	volatile uint32_t MaskedIRQ;
	volatile uint32_t Reload;
	volatile uint32_t PreDivider;
	volatile uint32_t FreeRunningCounter;
}
rpi_arm_timer_t;

rpi_arm_timer_t *get_arm_timer(void);

/* SYS Timer */
#define SYSTIMER_BASE               (PERIPHERAL_BASE + 0x3000)

typedef struct
{
	volatile uint32_t control_status;
	volatile uint32_t counter_lo;
	volatile uint32_t counter_hi;
	volatile uint32_t compare0;
	volatile uint32_t compare1;
	volatile uint32_t compare2;
	volatile uint32_t compare3;
}
rpi_sys_timer_t;

rpi_sys_timer_t *get_sys_timer(void);
void delay_us(uint32_t us);

#endif
