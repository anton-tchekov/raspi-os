
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "gic-400.h"
#include "timer.h"
#include "gpio.h"
#include "interrupts.h"
#include "mailbox.h"
#include "graphics.h"
#include "framebuffer.h"
#include "board.h"

#include "img_loading.c"

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
	board_init();
	g_init();

	g_color(255, 255, 255);
	g_string(10, 10, "Hello World!");

	g_color(255, 0, 0);
	g_rect(10, 20, 100, 2);

	g_string(10, 20, "This is a test.");

	g_image(SCREEN_WIDTH / 2 - img_loading.width / 2, SCREEN_HEIGHT / 2 - img_loading.height / 2, img_loading.width, img_loading.height, img_loading.pixel_data);

	for(;;) ;
}
