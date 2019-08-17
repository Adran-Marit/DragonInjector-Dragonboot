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

static u32 *next_fb(u32 *framebuf)
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
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            gfx_set_pixel(*framebuf, x + j, y + i, image[i * width + j]);
        }
    }
}

void draw_payload_screen(u32 **framebuf, u32 x, u32 y, u32 payload_num)
{
    *framebuf = next_fb(*framebuf);
    memset(*framebuf, 0, 0x3C0000);

    u8 char_img_lut[] = {16, 14, 3, 3, 11, 16, 16, 10, 14, 7, 7, 22, 24, 7, 15, 21, 5, 11, 5, 24, 8, 21, 22, 7, 15, 21, 5, 24};
    u8 char_img_number_lut[] = {24, 20, 4, 2, 17. 1, 16, 23, 0};
    u32 chars[25][483];
    memcpy(chars, decomp_char_img, decomp_char_img_size);

    for(int i = 0; i < sizeof(char_img_lut); i++)
    {
        draw_image_32bpp(framebuf, &chars[char_img_lut[i]][0], x, y, 23, 21);
        x += 23;
    }

    draw_image_32bpp(framebuf, &chars[char_img_number_lut[payload_num]][0], x, y, 23, 21);
    draw_image_32bpp(framebuf, logo_white_img, 50, 100, 178, 128);

    set_active_framebuffer(*framebuf);
}