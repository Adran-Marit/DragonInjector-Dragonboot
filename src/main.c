/*
 * Copyright (c) 2018 Guillem96
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

#include <string.h>

#include "gfx/di.h"
#include "gfx/gfx.h"

#include "mem/heap.h"

#include "soc/hw_init.h"

#include "core/launcher.h"

#include "utils/util.h"
#include "utils/fs_utils.h"
#include "utils/btn.h"

#include "test_image.h"

extern void pivot_stack(u32 stack_top);
extern u8 get_payload_num(void);


u32 *fb;

static inline void setup_gfx()
{
    fb = display_init_framebuffer();
}

void find_and_launch_payload(const char *folder)
{
    DIR dir;
    FILINFO finfo;
    FRESULT res = f_findfirst(&dir, &finfo, folder, "*.bin");
    if(res == FR_OK)
    {
        //gfx_printf(&g_gfx_con, "Launching %s/%s\n", folder, finfo.fname);
        size_t path_size = strlen(finfo.fname) + strlen(folder) + 2;
        char *payload_path = malloc(path_size);
        if(payload_path != NULL)
        {
            memset(payload_path, 0, path_size);
            strcpy(payload_path, folder);
            strcat(payload_path, "/");
            strcat(payload_path, finfo.fname);
            msleep(5000);
            launch_payload(payload_path);
        }
    }
    else
    {
        //gfx_printf(&g_gfx_con, "FatFs error code %d\n", res);
    }
}

void image_convert(u8 *in, u32 *out, size_t size, u32 back_color, u32 fore_color)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            u32 color = back_color;
            if(in[i] & (0x80 >> j))
                color = fore_color;

            out[i * 8 + j] = color;
        }
    }
}

void ipl_main()
{
    config_hw();

    /* Init the stack and the heap */
    pivot_stack(0x90010000);
    heap_init(0x90020000);

    /* Init display and gfx module */
    display_init();
    setup_gfx();
    display_backlight_pwm_init();
    display_backlight_brightness(100, 1000);

    u32 *final_image = malloc(test_image_len * 0x20);

    memset(final_image, 0, test_image_len * 0x20);

    image_convert(test_image, final_image, test_image_len, BLACK, BLUE);

    draw_image_32bpp(&fb, final_image, 1, 1, 391, 245);

    u8 payload_num = get_payload_num() + 1;

    if(sd_mount())
    {
        if(payload_num == 0)
        {
            find_and_launch_payload("dragonboot");
        }
        char folder[] = "dragonboot/00";

        const char num_table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8' };
        folder[sizeof(folder) - 2] = num_table[payload_num];

        //gfx_printf(&g_gfx_con, "Got payload number: %d\n", payload_num);

        find_and_launch_payload(folder);
    }
}