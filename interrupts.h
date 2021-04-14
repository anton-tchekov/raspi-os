#ifndef __INTERRUPTS_H_
#define __INTERRUPTS_H__

#include <stdint.h>
#include "base.h"

extern volatile int uptime;

void _enable_interrupts(void);
void RPI_EnableARMTimerInterrupt(void);

#endif
