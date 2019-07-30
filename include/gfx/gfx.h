/*
 * Copyright (c) 2018 naehrwert
 * Copyright (C) 2018 CTCaer
 * Copyright (C) 2018 M4xw
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

#ifndef _GFX_H_
#define _GFX_H_

#include "utils/types.h"

#define RED 0xFF0000FF
#define GREEN 0xFF00FF00
#define BLUE 0xFFFF0000
#define YELLOW 0xFF00FFFF
#define ORANGE 0xFF3891FF
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

#define WIDTH  1280
#define HEIGHT 720
#define STRIDE 720

void gfx_set_pixel(u32 *fb, u32 x, u32 y, u32 color);
void draw_image_32bpp(u32 **framebuf, u32 *image, u32 x, u32 y, u32 width, u32 height);

#endif
