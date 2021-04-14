#include "board.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "mailbox.h"
#include "interrupts.h"

static const char *processors[] =
{
	"BCM2835",
	"BCM2836",
	"BCM2837",
	"BCM2711"
};

static const char *rpi_types[] =
{
	"1A",
	"1B",
	"1A+",
	"1B+",
	"2B",
	"ALPHA",
	"CM1",
	"{7}",
	"3B",
	"Zero",
	"CM3",
	"{11}",
	"Zero W",
	"3B+",
	"3A+",
	"-",
	"CM3+",
	"4B"
};

static const char *rpi_memories[] =
{
	"256MB",
	"512MB",
	"1GiB",
	"2GiB",
	"4GiB",
	"8GiB"
};

static const char *rpi_manufacturers[] =
{
	"Sony UK",
	"Egoman",
	"Embest",
	"Sony Japan",
	"Embest",
	"Stadium"
};

static const char* rpi_models[] =
{
	"-",
	"-",
	"RPI1B 1.0 256MB Egoman",
	"RPI1B 1.0 256MB Egoman",
	"RPI1B 2.0 256MB Sony UK",
	"RPI1B 2.0 256MB Qisda",
	"RPI1B 2.0 256MB Egoman",
	"RPI1A 2.0 256MB Egoman",
	"RPI1A 2.0 256MB Sony UK",
	"RPI1A 2.0 256MB Qisda",
	"RPI1B 2.0 512MB Egoman",
	"RPI1B 2.0 512MB Sony UK",
	"RPI1B 2.0 512MB Egoman",
	"RPI1B+ 1.2 512MB Sony UK",
	"CM1 1.0 512MB Sony UK",
	"RPI1A+ 1.1 256MB Sony UK",
	"RPI1B+ 1.2 512MB Embest",
	"CM1 1.0 512MB Embest",
	"RPI1A+ 1.1 256MB/512MB Embest"
};

int board_init(void)
{
	rpi_mailbox_property_t *mp;
	uint16_t prescales[] = { 1, 16, 256, 1 };
	uint32_t core_frequency, timer_load;

	RPI_SetGpioPinFunction(LED_GPIO, FS_OUTPUT);
	LED_ON();

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_CLOCK_RATE, TAG_CLOCK_CORE);
	RPI_PropertyProcess();
	mp = RPI_PropertyGet(TAG_GET_CLOCK_RATE);
	core_frequency = mp->data.buffer_32[1];

	timer_load = (1.0 / 2) / (1.0 / (core_frequency / (get_arm_timer()->PreDivider + 1) * (prescales[(get_arm_timer()->Control & 0xC) >> 2])));
	get_arm_timer()->Load = timer_load;
	get_arm_timer()->Control = (ARMTIMER_CTRL_23BIT | ARMTIMER_CTRL_ENABLE | ARMTIMER_CTRL_INT_ENABLE);
	RPI_EnableARMTimerInterrupt();
	_enable_interrupts();

	uart_init(115200, 8);

	/* Clock rate */
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_MAX_CLOCK_RATE, TAG_CLOCK_ARM);
	RPI_PropertyProcess();
	mp = RPI_PropertyGet(TAG_GET_MAX_CLOCK_RATE);
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_SET_CLOCK_RATE, TAG_CLOCK_ARM, mp->data.buffer_32[1]);
	RPI_PropertyProcess();
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_CLOCK_RATE, TAG_CLOCK_ARM);
	RPI_PropertyProcess();
	if((mp = RPI_PropertyGet(TAG_GET_CLOCK_RATE)))
	{
		/* printf("ARM Frequency: %dMHz\r\n", (mp->data.buffer_32[1] / 1000000)); */
	}

	/* Board Rev */
	if((mp = RPI_PropertyGet(TAG_GET_BOARD_REVISION)))
	{
		uint32_t revision = mp->data.value_32;
		/* printf("Board Revision: 0x%8.8x", mp->data.value_32); */
		if(revision & (1 << 23))
		{
			/* New style revision code */
			/* printf(" rpi-%s", rpi_types[(revision & (0xFF << 4)) >> 4]);
			printf(" %s", processors[(revision & (0xF << 12)) >> 12]);
			printf(" %s", rpi_memories[(revision & (0x7 << 20)) >> 20]);
			printf(" %s", rpi_manufacturers[(revision & (0xF << 16)) >> 16]); */
		}
		else
		{
			/* printf(" %s", rpi_models[revision]); */
		}

	}

	if((mp = RPI_PropertyGet(TAG_GET_FIRMWARE_VERSION)))
	{
		/* printf("Firmware Version: %d\r\n", mp->data.value_32); */
	}

	if((mp = RPI_PropertyGet(TAG_GET_BOARD_MAC_ADDRESS)))
	{
		/* printf("MAC Address: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\r\n",
			mp->data.buffer_8[0], mp->data.buffer_8[1], mp->data.buffer_8[2],
			mp->data.buffer_8[3], mp->data.buffer_8[4], mp->data.buffer_8[5]); */
	}

	if((mp = RPI_PropertyGet(TAG_GET_BOARD_SERIAL)))
	{
		/* printf("Serial Number: %8.8X%8.8X\r\n", mp->data.buffer_32[0], mp->data.buffer_32[1]); */
	}

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_BOARD_REVISION);
	RPI_PropertyAddTag(TAG_GET_FIRMWARE_VERSION);
	RPI_PropertyAddTag(TAG_GET_BOARD_MAC_ADDRESS);
	RPI_PropertyAddTag(TAG_GET_BOARD_SERIAL);
	RPI_PropertyProcess();
}
