#include "uart.h"
#include "base.h"
#include "gpio.h"

static aux_t *auxillary = (aux_t *)AUX_BASE;

void uart_init(int baud, int bits)
{
	volatile int i;
	auxillary->ENABLES = AUX_ENA_MINIUART;
	auxillary->MU_IER = 0;
	auxillary->MU_CNTL = 0;
	if(bits == 8)
	{
		auxillary->MU_LCR = AUX_MULCR_8BIT_MODE;
	}
	else
	{
		auxillary->MU_LCR = 0;
	}

	auxillary->MU_MCR = 0;
	auxillary->MU_IER = 0;
	auxillary->MU_IIR = 0xC6;
	auxillary->MU_BAUD = (SYSFREQ / (8 * baud)) - 1;
	RPI_SetGpioPinFunction(RPI_GPIO14, FS_ALT5);
	RPI_SetGpioPinFunction(RPI_GPIO15, FS_ALT5);
	RPI_GetGpio()->GPPUD = 0;
	for(i = 0; i < 150; ++i) ;
	RPI_GetGpio()->GPPUDCLK0 = (1 << 14);
	for(i = 0; i < 150; ++i) ;
	RPI_GetGpio()->GPPUDCLK0 = 0;
	auxillary->MU_CNTL = AUX_MUCNTL_TX_ENABLE;
}


void uart_write(char c)
{
	while((auxillary->MU_LSR & AUX_MULSR_TX_EMPTY) == 0) ;
	auxillary->MU_IO = c;
}
