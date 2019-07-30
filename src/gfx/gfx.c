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

static u32 *switch_fb(u32 *framebuf)
{
    if(framebuf == (u32 *)0xC0000000)
        return (u32 *)0xC03C0000;
    else
        return (u32 *)0xC0000000;
}

inline void gfx_set_pixel(u32 *framebuf, u32 x, u32 y, u32 color)
{
    framebuf[y + (WIDTH - x) * STRIDE] = color;
}

void draw_image_32bpp(u32 **framebuf, u32 *image, u32 x, u32 y, u32 width, u32 height)
{
    *framebuf = switch_fb(*framebuf);
    memset(*framebuf, 0, 0x3C0000);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            gfx_set_pixel(*framebuf, x + j, y + i, image[i * width + j]);
        }
    }

    set_active_framebuffer(*framebuf);
}