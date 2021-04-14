#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "gpio.h"
#include "mailbox.h"

static mailbox_t *rpiMailbox0 = (mailbox_t *)RPI_MAILBOX0_BASE;
static int pt[8192] __attribute__((aligned(16)));
static int pt_index = 0;

void RPI_Mailbox0Write(mailbox0_channel_t channel, int value)
{
	value &= ~(0x0F);
	value |= channel;
	while((rpiMailbox0->Status & ARM_MS_FULL) != 0) ;
	rpiMailbox0->Write = value;
}

int RPI_Mailbox0Read(mailbox0_channel_t channel)
{
	int value = -1;
	while((value & 0xF) != channel)
	{
		while(rpiMailbox0->Status & ARM_MS_EMPTY) ;
		value = rpiMailbox0->Read;
	}

	return value >> 4;
}

void RPI_PropertyInit(void)
{
	pt[PT_OSIZE] = 12;
	pt[PT_OREQUEST_OR_RESPONSE] = 0;
	pt_index = 2;
	pt[pt_index] = 0;
}

void RPI_PropertyAddTag(rpi_mailbox_tag_t tag, ...)
{
	va_list vl;
	va_start(vl, tag);
	pt[pt_index++] = tag;
	switch(tag)
	{
		case TAG_GET_FIRMWARE_VERSION:
		case TAG_GET_BOARD_MODEL:
		case TAG_GET_BOARD_REVISION:
		case TAG_GET_BOARD_MAC_ADDRESS:
		case TAG_GET_BOARD_SERIAL:
		case TAG_GET_ARM_MEMORY:
		case TAG_GET_VC_MEMORY:
		case TAG_GET_DMA_CHANNELS:
			pt[pt_index++] = 8;
			pt[pt_index++] = 0;
			pt_index += 2;
			break;

		case TAG_GET_CLOCKS:
		case TAG_GET_COMMAND_LINE:
			pt[pt_index++] = 256;
			pt[pt_index++] = 0;
			pt_index += 256 >> 2;
			break;

		case TAG_ALLOCATE_BUFFER:
		case TAG_GET_MAX_CLOCK_RATE:
		case TAG_GET_MIN_CLOCK_RATE:
		case TAG_GET_CLOCK_RATE:
			pt[pt_index++] = 8;
			pt[pt_index++] = 0;
			pt[pt_index++] = va_arg(vl, int);
			pt[pt_index++] = 0;
			break;

		case TAG_SET_CLOCK_RATE:
			pt[pt_index++] = 12;
			pt[pt_index++] = 0;
			pt[pt_index++] = va_arg(vl, int);
			pt[pt_index++] = va_arg(vl, int);
			pt[pt_index++] = va_arg(vl, int);
			break;

		case TAG_GET_PHYSICAL_SIZE:
		case TAG_SET_PHYSICAL_SIZE:
		case TAG_TEST_PHYSICAL_SIZE:
		case TAG_GET_VIRTUAL_SIZE:
		case TAG_SET_VIRTUAL_SIZE:
		case TAG_TEST_VIRTUAL_SIZE:
		case TAG_GET_VIRTUAL_OFFSET:
		case TAG_SET_VIRTUAL_OFFSET:
			pt[pt_index++] = 8;
			pt[pt_index++] = 0;
			if((tag == TAG_SET_PHYSICAL_SIZE) ||
				(tag == TAG_SET_VIRTUAL_SIZE) ||
				(tag == TAG_SET_VIRTUAL_OFFSET) ||
				(tag == TAG_TEST_PHYSICAL_SIZE) ||
				(tag == TAG_TEST_VIRTUAL_SIZE))
			{
				pt[pt_index++] = va_arg(vl, int);
				pt[pt_index++] = va_arg(vl, int);
			}
			else
			{
				pt_index += 2;
			}
			break;

		case TAG_GET_ALPHA_MODE:
		case TAG_SET_ALPHA_MODE:
		case TAG_GET_DEPTH:
		case TAG_SET_DEPTH:
		case TAG_GET_PIXEL_ORDER:
		case TAG_SET_PIXEL_ORDER:
		case TAG_GET_PITCH:
			pt[pt_index++] = 4;
			pt[pt_index++] = 0;
			if((tag == TAG_SET_DEPTH) ||
				(tag == TAG_SET_PIXEL_ORDER) ||
				(tag == TAG_SET_ALPHA_MODE))
			{
				pt[pt_index++] = va_arg(vl, int);
			}
			else
			{
				pt_index += 1;
			}
			break;

		case TAG_GET_OVERSCAN:
		case TAG_SET_OVERSCAN:
			pt[pt_index++] = 16;
			pt[pt_index++] = 0;
			if((tag == TAG_SET_OVERSCAN))
			{
				pt[pt_index++] = va_arg(vl, int);
				pt[pt_index++] = va_arg(vl, int);
				pt[pt_index++] = va_arg(vl, int);
				pt[pt_index++] = va_arg(vl, int);
			}
			else
			{
				pt_index += 4;
			}
			break;

		default:
			--pt_index;
			break;
	}

	pt[pt_index] = 0;
	va_end(vl);
}

int RPI_PropertyProcess(void)
{
	int result;
	pt[PT_OSIZE] = (pt_index + 1) << 2;
	pt[PT_OREQUEST_OR_RESPONSE] = 0;
	RPI_Mailbox0Write(MB0_TAGS_ARM_TO_VC, (unsigned int)pt);
	result = RPI_Mailbox0Read(MB0_TAGS_ARM_TO_VC);
	return result;
}

rpi_mailbox_property_t *RPI_PropertyGet(rpi_mailbox_tag_t tag)
{
	static rpi_mailbox_property_t property;
	int *tag_buffer = NULL;
	int index = 2;
	property.tag = tag;
	while(index < (pt[PT_OSIZE] >> 2))
	{
		if(pt[index] == tag)
		{
			tag_buffer = &pt[index];
			break;
		}

		index += (pt[index + 1] >> 2) + 3;
	}

	if(tag_buffer == NULL)
	{
		return NULL;
	}

	property.byte_length = tag_buffer[T_ORESPONSE] & 0xFFFF;
	memcpy(property.data.buffer_8, &tag_buffer[T_OVALUE], property.byte_length);
	return &property;
}

