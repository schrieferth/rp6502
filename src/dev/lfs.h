/*
 * Copyright (c) 2023 Rumbledethumps
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LFS_H_
#define _LFS_H_

#include "hardware/flash.h"
#include "littlefs/lfs.h"

#define _LFS_FILE_CONFIG_NAME(name) lfs_file_config_buffer_##name
#define LFS_FILE_CONFIG(name)                             \
    uint8_t _LFS_FILE_CONFIG_NAME(name)[FLASH_PAGE_SIZE]; \
    struct lfs_file_config lfs_file_config = {            \
        .buffer = _LFS_FILE_CONFIG_NAME(name),            \
    };

void lfs_init();
int lfs_eof(lfs_file_t *file);
int lfs_printf(lfs_t *lfs, lfs_file_t *file, const char *format, ...);
char *lfs_gets(char *str, int n, lfs_t *lfs, lfs_file_t *file);

extern lfs_t lfs_volume;

#endif /* _LFS_H_ */