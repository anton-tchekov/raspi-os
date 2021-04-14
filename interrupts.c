#include <stdint.h>
#include <stdint.h>
#include "timer.h"
#include "base.h"
#include "gpio.h"
#include "interrupts.h"
#include "gic-400.h"

volatile int uptime = 0;

void __attribute__((interrupt("ABORT"))) reset_vector(void)
{
	for(;;) ;
}

void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
	for(;;) ;
}

void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{
	for(;;) ;
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{
	for(;;) ;
}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
	for(;;) ;
}

void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
	static int lit = 0;
	static int jiffies = 0;
	if(get_arm_timer()->MaskedIRQ)
	{
		get_arm_timer()->IRQClear = 1;
		jiffies++;
		if(jiffies == 2)
		{
			jiffies = 0;
			uptime++;
		}

		if(lit)
		{
			LED_OFF();
			lit = 0;
		}
		else
		{
			LED_ON();
			lit = 1;
		}
	}
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
	for(;;) ;
}

#define RPI_INTERRUPT_CONTROLLER_BASE   (PERIPHERAL_BASE + 0xB200UL)
#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

void RPI_EnableGICInterrupts(void);

typedef struct
{
	volatile uint32_t IRQ_basic_pending;
	volatile uint32_t IRQ_pending_1;
	volatile uint32_t IRQ_pending_2;
	volatile uint32_t FIQ_control;
	volatile uint32_t Enable_IRQs_1;
	volatile uint32_t Enable_IRQs_2;
	volatile uint32_t Enable_Basic_IRQs;
	volatile uint32_t Disable_IRQs_1;
	volatile uint32_t Disable_IRQs_2;
	volatile uint32_t Disable_Basic_IRQs;
}
rpi_irq_controller_t;

static rpi_irq_controller_t *rpiIRQController =
		(rpi_irq_controller_t *)RPI_INTERRUPT_CONTROLLER_BASE;

static rpi_irq_controller_t *RPI_GetIrqController(void)
{
	return rpiIRQController;
}

void RPI_EnableARMTimerInterrupt(void)
{
#ifdef RPI4
	gic400_init((void *)0xFF840000UL);
#endif

	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
}
