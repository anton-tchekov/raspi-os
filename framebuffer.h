#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <stddef.h>
#include <stdint.h>

#define SCREEN_WIDTH  1366
#define SCREEN_HEIGHT  768
#define SCREEN_DEPTH    32

#define SCREEN_STRIDE 1376

extern volatile uint32_t *fb;

int fb_init(void);

#endif

