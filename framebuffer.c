#include "framebuffer.h"
#include "mailbox.h"

volatile uint32_t *fb;

int fb_init(void)
{
	int width = 0, height = 0, pitch_bytes = 0;
	rpi_mailbox_property_t *mp;
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_ALLOCATE_BUFFER);
	RPI_PropertyAddTag(TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT);
	RPI_PropertyAddTag(TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT * 2);
	RPI_PropertyAddTag(TAG_SET_DEPTH, SCREEN_DEPTH);
	RPI_PropertyAddTag(TAG_GET_PITCH);
	RPI_PropertyAddTag(TAG_GET_PHYSICAL_SIZE);
	RPI_PropertyAddTag(TAG_GET_DEPTH);
	RPI_PropertyProcess();
	if((mp = RPI_PropertyGet(TAG_GET_PHYSICAL_SIZE)))
	{
		width = mp->data.buffer_32[0];
		height = mp->data.buffer_32[1];
	}

	if((mp = RPI_PropertyGet(TAG_GET_DEPTH)))
	{
		int bpp = mp->data.buffer_32[0];
		if(bpp != SCREEN_DEPTH)
		{
			return -1;
		}
	}

	if((mp = RPI_PropertyGet(TAG_GET_PITCH)))
	{
		pitch_bytes = mp->data.buffer_32[0];
	}

	if((mp = RPI_PropertyGet(TAG_ALLOCATE_BUFFER)))
	{
		fb = (volatile uint32_t *)(mp->data.buffer_32[0] & ~0xC0000000);
	}

	return 0;
}
