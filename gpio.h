#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include "base.h"

#define RPI_GPIO_BASE   (PERIPHERAL_BASE + 0x200000UL)

#if defined(RPI1) && !defined(IOBPLUS)

#define LED_GPFSEL      GPFSEL1
#define LED_GPFBIT      18
#define LED_GPSET       GPSET0
#define LED_GPCLR       GPCLR0
#define LED_GPIO_BIT    16
#define LED_GPIO        16

#elif (defined(RPI1) && defined(IOBPLUS)) || defined(RPI2) || defined(RPI0)

#define LED_GPFSEL      GPFSEL4
#define LED_GPFBIT      21
#define LED_GPSET       GPSET1
#define LED_GPCLR       GPCLR1
#define LED_GPIO_BIT    15
#define LED_GPIO        47

#elif defined(RPI3) && defined(IOBPLUS)

#define LED_GPFSEL      GPFSEL2
#define LED_GPFBIT      27
#define LED_GPSET       GPSET0
#define LED_GPCLR       GPCLR0
#define LED_GPIO_BIT    29
#define LED_GPIO        29

#elif defined(RPI3)

#error RPI3 is not supported!

#elif defined(RPI4)

#define LED_GPFSEL      GPFSEL4
#define LED_GPFBIT      6
#define LED_GPSET       GPSET1
#define LED_GPCLR       GPCLR1
#define LED_GPIO_BIT    10
#define LED_GPIO        42

#endif

#if defined(RPI0) || (defined(RPI1) && !defined(IOBPLUS))

#define LED_ON()        do { RPI_GetGpio()->LED_GPCLR = (1 << LED_GPIO_BIT); } while(0)
#define LED_OFF()       do { RPI_GetGpio()->LED_GPSET = (1 << LED_GPIO_BIT); } while(0)

#else

#define LED_ON()        do { RPI_GetGpio()->LED_GPSET = (1 << LED_GPIO_BIT); } while(0)
#define LED_OFF()       do { RPI_GetGpio()->LED_GPCLR = (1 << LED_GPIO_BIT); } while(0)

#endif

typedef enum
{
	FS_INPUT = 0,
	FS_OUTPUT,
	FS_ALT5,
	FS_ALT4,
	FS_ALT0,
	FS_ALT1,
	FS_ALT2,
	FS_ALT3,
}
rpi_gpio_alt_function_t;

#define FS_MASK         7

typedef enum
{
	RPI_GPIO0  =  0,
	RPI_GPIO1,
	RPI_GPIO2,
	RPI_GPIO3,
	RPI_GPIO4,
	RPI_GPIO5,
	RPI_GPIO6,
	RPI_GPIO7,
	RPI_GPIO8,
	RPI_GPIO9,
	RPI_GPIO10 = 10,
	RPI_GPIO11,
	RPI_GPIO12,
	RPI_GPIO13,
	RPI_GPIO14,
	RPI_GPIO15,
	RPI_GPIO16,
	RPI_GPIO17,
	RPI_GPIO18,
	RPI_GPIO19,
	RPI_GPIO20 = 20,
	RPI_GPIO21,
	RPI_GPIO22,
	RPI_GPIO23,
	RPI_GPIO24,
	RPI_GPIO25,
	RPI_GPIO26,
	RPI_GPIO27,
	RPI_GPIO28,
	RPI_GPIO29,
	RPI_GPIO30 = 30,
	RPI_GPIO31,
	RPI_GPIO32,
	RPI_GPIO33,
	RPI_GPIO34,
	RPI_GPIO35,
	RPI_GPIO36,
	RPI_GPIO37,
	RPI_GPIO38,
	RPI_GPIO39,
	RPI_GPIO40 = 40,
	RPI_GPIO41,
	RPI_GPIO42,
	RPI_GPIO43,
	RPI_GPIO44,
	RPI_GPIO45,
	RPI_GPIO46,
	RPI_GPIO47,
	RPI_GPIO48,
	RPI_GPIO49,
	RPI_GPIO50 = 50,
	RPI_GPIO51,
	RPI_GPIO52,
	RPI_GPIO53,
}
rpi_gpio_pin_t;

typedef struct
{
	rpi_reg_rw_t GPFSEL0;
	rpi_reg_rw_t GPFSEL1;
	rpi_reg_rw_t GPFSEL2;
	rpi_reg_rw_t GPFSEL3;
	rpi_reg_rw_t GPFSEL4;
	rpi_reg_rw_t GPFSEL5;
	rpi_reg_ro_t Reserved0;
	rpi_reg_wo_t GPSET0;
	rpi_reg_wo_t GPSET1;
	rpi_reg_ro_t Reserved1;
	rpi_reg_wo_t GPCLR0;
	rpi_reg_wo_t GPCLR1;
	rpi_reg_ro_t Reserved2;
	rpi_reg_wo_t GPLEV0;
	rpi_reg_wo_t GPLEV1;
	rpi_reg_ro_t Reserved3;
	rpi_reg_wo_t GPEDS0;
	rpi_reg_wo_t GPEDS1;
	rpi_reg_ro_t Reserved4;
	rpi_reg_wo_t GPREN0;
	rpi_reg_wo_t GPREN1;
	rpi_reg_ro_t Reserved5;
	rpi_reg_wo_t GPFEN0;
	rpi_reg_wo_t GPFEN1;
	rpi_reg_ro_t Reserved6;
	rpi_reg_wo_t GPHEN0;
	rpi_reg_wo_t GPHEN1;
	rpi_reg_ro_t Reserved7;
	rpi_reg_wo_t GPLEN0;
	rpi_reg_wo_t GPLEN1;
	rpi_reg_ro_t Reserved8;
	rpi_reg_wo_t GPAREN0;
	rpi_reg_wo_t GPAREN1;
	rpi_reg_ro_t Reserved9;
	rpi_reg_wo_t GPAFEN0;
	rpi_reg_wo_t GPAFEN1;
	rpi_reg_ro_t Reserved10;
	rpi_reg_wo_t GPPUD;
	rpi_reg_wo_t GPPUDCLK0;
	rpi_reg_wo_t GPPUDCLK1;
	rpi_reg_ro_t Reserved11;
}
rpi_gpio_t;

typedef enum
{
	RPI_IO_LO = 0,
	RPI_IO_HI,
	RPI_IO_ON,
	RPI_IO_OFF,
	RPI_IO_UNKNOWN
}
rpi_gpio_value_t;

rpi_gpio_t *RPI_GetGpio(void);
void RPI_SetGpioPinFunction(rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func);
void RPI_SetGpioOutput(rpi_gpio_pin_t gpio);
void RPI_SetGpioInput(rpi_gpio_pin_t gpio);
rpi_gpio_value_t RPI_GetGpioValue(rpi_gpio_pin_t gpio);
void RPI_SetGpioHi(rpi_gpio_pin_t gpio);
void RPI_SetGpioLo(rpi_gpio_pin_t gpio);
void RPI_SetGpioValue(rpi_gpio_pin_t gpio, rpi_gpio_value_t value);
void RPI_ToggleGpio(rpi_gpio_pin_t gpio);

#endif
