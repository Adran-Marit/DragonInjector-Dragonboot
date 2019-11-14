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
#include "soc/t210.h"
#include "soc/pmc.h"

#include "core/launcher.h"

#include "utils/util.h"
#include "utils/fs_utils.h"
#include "utils/btn.h"

extern void pivot_stack(u32 stack_top);
extern u8 get_payload_num(void);

static inline void setup_gfx()
{
    u32 *fb = display_init_framebuffer();
    gfx_init_ctxt(&g_gfx_ctxt, fb, 1280, 720, 720);
    gfx_clear_buffer(&g_gfx_ctxt);
    gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
    gfx_con_setcol(&g_gfx_con, 0xFFCCCCCC, 1, BLACK);
}

void find_and_launch_payload(const char *folder)
{
    DIR dir;
    FILINFO finfo;
    FRESULT res = f_findfirst(&dir, &finfo, folder, "*.bin");
    if(res == FR_OK)
    {
        gfx_printf(&g_gfx_con, "Launching %s/%s\n", folder, finfo.fname);
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
    else if(res == FR_NO_FILE)
    {
        gfx_printf(&g_gfx_con, "No payload found in %s!\n", folder);
    }
    else if(res == FR_NO_PATH)
    {
        gfx_printf(&g_gfx_con, "Dragonboot folder not found!\nPlease create %s on your SD card and add your payload to it.\n", folder);
    }
    gfx_printf(&g_gfx_con, "FatFs error code %d\n", res);
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

        gfx_printf(&g_gfx_con, "Got payload number: %d\n", payload_num);

        find_and_launch_payload(folder);
    }
    else
    {
        gfx_printf(&g_gfx_con, "SD card not inserted!\n");
    }

    gfx_printf(&g_gfx_con, "Press any button to reboot into RCM.\n");
    btn_wait();
    gfx_printf(&g_gfx_con, "Rebooting to RCM in 5 seconds...\n");
    msleep(5000);
    PMC(APBDEV_PMC_SCRATCH0) |= 2;
    PMC(APBDEV_PMC_CNTRL) = PMC_CNTRL_MAIN_RST;
    while(1);
}