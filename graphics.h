#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdint.h>

void g_init(void);
void g_color(uint8_t r, uint8_t g, uint8_t b);
void g_font(int id);
void g_pixel(int x, int y);
void g_rect(int x, int y, int w, int h);
int g_char(int x, int y, int c);
int g_string_width(const char *s, int font_id);
int g_string(int x, int y, const char *s);
void g_image(int x, int y, int w, int h, const uint8_t *data);

#endif
