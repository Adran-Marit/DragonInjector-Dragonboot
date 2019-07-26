/*
 * Copyright (c) 2018 naehrwert
 * Copyright (C) 2018 CTCaer
 * Copyright (C) 2018 Guillem96
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gfx/gfx.h"
#include "gfx/di.h"

#include "utils/fs_utils.h"
#include "utils/util.h"
#include "mem/heap.h"

inline void gfx_set_pixel(u32 *framebuf, u32 x, u32 y, u32 color)
{
    framebuf[y + (WIDTH - x) * STRIDE] = color;
}

void draw_image_1bpp(u32 *framebuf, u8 *image, u32 x, u32 y, u32 width, u32 height, u32 back_color, u32 fore_color)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width / 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                u32 color = back_color;
                if(image[i * width / 8 + j] & (0x80 >> k))
                    color = fore_color;
                gfx_set_pixel(framebuf, x + j + k, y + i, color);
            }
        }
    }
}