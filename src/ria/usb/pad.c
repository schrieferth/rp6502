/*
 * Copyright (c) 2023 Rumbledethumps
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb/pad.h"
#include "sys/mem.h"

// Sony DS4 report layout
// https://www.psdevwiki.com/ps4/DS4-USB
typedef struct TU_ATTR_PACKED
{
    uint8_t lx, ly, rx, ry; // analog sticks

    struct
    {
        uint8_t dpad : 4; // (8=released, 7=NW, 6=W, 5=SW, 4=S, 3=SE, 2=E, 1=NE, 0=N)
        uint8_t square : 1;
        uint8_t cross : 1;
        uint8_t circle : 1;
        uint8_t triangle : 1;
    };

    struct
    {
        uint8_t l1 : 1;
        uint8_t r1 : 1;
        uint8_t l2 : 1;
        uint8_t r2 : 1;
        uint8_t share : 1;
        uint8_t option : 1;
        uint8_t l3 : 1;
        uint8_t r3 : 1;
    };

    struct
    {
        uint8_t psbtn : 1;
        uint8_t tpadbtn : 1;
        uint8_t counter : 6;
    };

    uint8_t l2_trigger;
    uint8_t r2_trigger;

} sony_ds4_report_t;

#define PAD_TIMEOUT_TIME_MS 10
static absolute_time_t pad_timer;
static uint16_t pad_xram = 0xFFFF;

static void pad_disconnect_check(void)
{
    // Set dpad invalid to indicate no controller detected
    if (absolute_time_diff_us(get_absolute_time(), pad_timer) < 0)
        if (pad_xram != 0xFFFF)
            xram[pad_xram + 4] = 0x0F;
}

void pad_init(void)
{
    pad_stop();
}

void pad_stop(void)
{
    pad_xram = 0xFFFF;
}

void pad_task(void)
{
    pad_disconnect_check();
}

bool pad_xreg(uint16_t word)
{
    if (word != 0xFFFF && word > 0x10000 - sizeof(sony_ds4_report_t))
        return false;
    pad_xram = word;
    pad_disconnect_check();
    return true;
}

void pad_report(uint8_t dev_addr, uint8_t const *report)
{
    (void)dev_addr;
    // We should probably check VIDs/PIDs or something
    if (report[0] == 1)
    {
        pad_timer = make_timeout_time_ms(PAD_TIMEOUT_TIME_MS);
        if (pad_xram != 0xFFFF)
            memcpy(&xram[pad_xram], report + 1, sizeof(sony_ds4_report_t));
    }
}