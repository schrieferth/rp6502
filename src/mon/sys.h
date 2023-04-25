/*
 * Copyright (c) 2023 Rumbledethumps
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SYS_H_
#define _SYS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void sys_binary(const char *args, size_t len);
void sys_address(const char *args, size_t len);
void sys_reboot(const char *args, size_t len);
void sys_reset_6502(const char *args, size_t len);
bool sys_rx_handler();
void sys_task();
void sys_keep_alive();
bool sys_is_active();
bool sys_is_rx_binary();
void sys_reset();

#endif /* _SYS_H_ */