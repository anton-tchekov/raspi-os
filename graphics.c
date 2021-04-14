#include "graphics.h"
#include "framebuffer.h"
#include "font.h"

static uint32_t _color;
static int _font_id;

void g_init(void)
{
	fb_init();
}

void g_color(uint8_t r, uint8_t g, uint8_t b)
{
	_color = (r << 16) | (g << 8) | (b);
}

void g_font(int id)
{
	_font_id = id;
}

void g_pixel(int x, int y)
{
	fb[y * SCREEN_STRIDE + x] = _color;
}

void g_rect(int x, int y, int w, int h)
{
	int x0, h0 = y + h, w0 = x + w;
	for(; y < h0; ++y)
	{
		for(x0 = x; x0 < w0; ++x0)
		{
			g_pixel(x0, y);
		}
	}
}

int g_char(int x, int y, int c)
{
	int width = 0;
	if(c >= 32 && c <= 126)
	{
		int col, row, data;
		c -= 32;
		width = font_default_widths[c];
		for(row = 0; row < FONT_HEIGHT; ++row)
		{
			data = font_default_chars[c * FONT_HEIGHT + row];
			for(col = 0; col < width; ++col)
			{
				if((data >> col) & 1)
				{
					g_pixel(x + col, y + row);
				}
			}
		}
	}

	return width;
}

int g_string_width(const char *s, int font_id)
{
	int width, c;
	for(; (c = *s); ++s)
	{
		if(c >= 32 && c <= 126)
		{
			int col, row, data;
			c -= 32;
			width += font_default_widths[c];
		}
	}

	return width;
}

int g_string(int x, int y, const char *s)
{
	int c;
	for(; (c = *s); ++s)
	{
		x += g_char(x, y, c);
	}

	return x;
}

void g_image(int x, int y, int w, int h, const uint8_t *data)
{
	const uint8_t *p = data;
	uint8_t r, g, b;
	int x0, y0, x1, y1;
	x1 = x + w;
	y1 = y + h;
	for(y0 = y; y0 < y1; ++y0)
	{
		for(x0 = x; x0 < x1; ++x0)
		{
			r = *p++;
			g = *p++;
			b = *p++;
			fb[y0 * SCREEN_STRIDE + x0] = (r << 16) | (g << 8) | (b);
			++p;
		}
	}
}

