/**
 * @file
 *
 * @author Dalian University of Technology
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 Dalian University of Technology
 *
 * This file is part of EDUGUI.
 *
 * EDUGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * EDUGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EDUGUI; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * All rights reserved.
**/

# include "../graph_engine.h"
#include <data_structures.h>

static void draw_cursor(struct screen* s, struct rectangle* rect, si_t x, si_t y, struct cursor *cur, struct screen *dst);

si_t
engine_draw_cursor
(si_t graphics_device_handle,
 si_t x,
 si_t y,
 si_t a,
 si_t b,
 struct cursor *cur,
 struct screen *dst)
{
    struct graphics_device * gd;

    gd = (struct graphics_device *)graphics_device_handle;

	struct rectangle screen_area, result_area;
	screen_area.x = 0;
	screen_area.y = 0;
	screen_area.width = gd->screen.width;
	screen_area.height = gd->screen.height;

	if (area_intersection (&gd->rectangle, &screen_area, &result_area) == -1)
	{
		return -1;
	}

    if(a != 0 && b != 0)
	{
		draw_cursor(dst,&gd->rectangle, x, y, cur, dst);
	}

    return 0;
}

static void draw_cursor(struct screen* s, struct rectangle* rect, si_t x, si_t y, struct cursor *cur, struct screen *dst)
{
	int j;


	unsigned short cl;
	int bit;

	int t = 1;
	int andos=0;
	int xoros=0;
	int i;
	int pixel_matrix[64][64];
	for (t=0; t<1; t++)
	{
		/* 分析and位图数据 */
		for (i = cur->ciHeader[t].ciHeight-1; i >= 0; i--)
		{
			int bits;
			int offset;
			if(cur->ciHeader[t].ciWidth == 16)
			{
				bits = 2;
				offset = 2;
			}
			if(cur->ciHeader[t].ciWidth == 24)
			{
				bits = 3;
				offset = 1;
			}
			if (cur->ciHeader[t].ciWidth == 32)
			{
				bits = 4;
				offset = 0;
			}
			if (cur->ciHeader[t].ciWidth == 48)
			{
				bits = 6;
				offset = 2;
			}
			for ( j = 0; j < bits; j++)
			{
				cl = cur->andData[t][andos++];
				for (bit = 0; bit < 8; bit++)
				{
					if (((cl >> (7-bit)) & 1) == 1)
					{
						pixel_matrix[i][j*8+bit] = 100;
					}
					else
					{
						pixel_matrix[i][j*8+bit] = 0;
					}
				}
			}

			andos += offset;
		}

		/* 分析xor位图数据 */

		/* 24位和32位光标的绘制 */
		if(cur->biHeader[t].biBitCount == 24 ||
				cur->biHeader[t].biBitCount == 32)
		{
			for (i = cur->ciHeader[t].ciHeight-1; i >=0; i--)
			{
				for ( j = 0; j < cur->ciHeader[t].ciWidth; j++)
				{

					struct color c;
					c.r=cur->xorData[t][xoros++];
					c.g=cur->xorData[t][xoros++];
					c.b=cur->xorData[t][xoros++];
					c.a=pixel_matrix[i][j];
					if(cur->biHeader[t].biBitCount == 32)
						xoros++;

					screen_set_pixel_r(s, rect, &c, x+j, y+i , dst);
				}
			}
		}

		if(cur->biHeader[t].biBitCount == 4 ||
				cur->biHeader[t].biBitCount == 8 ||
				cur->biHeader[t].biBitCount == 1)
		{
			int startPos = (1 << cur->biHeader[t].biBitCount) * 4;
			xoros = startPos;
			/* 4位光标绘制  */
			if(cur->biHeader[t].biBitCount == 4)
			{
				unsigned int index;
				for(i = cur->ciHeader[t].ciHeight-1; i >= 0; i--)
				{
					for(j = 0; j < cur->ciHeader[t].ciWidth; j++)
					{
						struct color c;
						index = cur->xorData[t][xoros++];

						int l = ((index>>7)&1)*(1<<3) +
								((index>>6)&1)*(1<<2) +
								((index>>5)&1)*(1<<1) +
								((index>>4)&1)*(1<<0);
						int r = ((index>>3)&1)*(1<<3) +
								((index>>2)&1)*(1<<2) +
								((index>>1)&1)*(1<<1) +
								((index>>0)&1)*(1<<0);

						c.b = cur->xorData[t][l*4];
						c.g = cur->xorData[t][l*4+1];
						c.r = cur->xorData[t][l*4+2];
						c.a = pixel_matrix[i][j];

						screen_set_pixel_r(s, rect, &c, x+j, y+i, dst);
						j++;
						c.b = cur->xorData[t][r*4];
						c.g = cur->xorData[t][r*4+1];
						c.r = cur->xorData[t][r*4+2];
						c.a = pixel_matrix[i][j];
						screen_set_pixel_r(s, rect, &c, x+j, y+i, dst);
					}
				}
			}
			/* 8位光标绘制  */
			if(cur->biHeader[t].biBitCount == 8)
			{
				unsigned int index;
				for(i = cur->ciHeader[t].ciHeight-1; i >= 0; i--)
				{
					for(j = 0; j < cur->ciHeader[t].ciWidth; j++)
					{
						struct color c;
						index = cur->xorData[t][xoros++];

						int l = ((index>>7)&1)*(1<<7) +
								((index>>6)&1)*(1<<6) +
								((index>>5)&1)*(1<<5) +
								((index>>4)&1)*(1<<4) +
								((index>>3)&1)*(1<<3) +
								((index>>2)&1)*(1<<2) +
								((index>>1)&1)*(1<<1) +
								((index>>0)&1)*(1<<0);

						c.b = cur->xorData[t][l*4];
						c.g = cur->xorData[t][l*4+1];
						c.r = cur->xorData[t][l*4+2];
						c.a = pixel_matrix[i][j];
						screen_set_pixel_r(s, rect, &c, x+j, y+i, dst);
					}
				}
			}
			/* 1位光标绘制  */
			if(cur->biHeader[t].biBitCount == 1)
			{
				xoros=0;
				unsigned int index;
				for(i = cur->ciHeader[t].ciHeight-1; i >= 0; i--)
				{
					for(j = 0; j < cur->ciHeader[t].ciWidth; j++)
					{
						struct color c;
						static int seek=7;
						int l;
						index = cur->xorData[t][xoros];
                        l = (index>>seek)&1;

						c.b = cur->xorData[t][l*4];
						c.g = cur->xorData[t][l*4+1];
						c.r = cur->xorData[t][l*4+2];
						c.b = l*255;
						c.g = l*255;
						c.r = l*255;
						c.a = pixel_matrix[i][j];
						screen_set_pixel_r(s, rect, &c, x+j, y+i, dst);

						seek--;
						if(seek == -1)
						{
							seek = 7;
							xoros++;
						}
					}
				}
			}
		}
		xoros=0;
		andos=0;
	}
}


