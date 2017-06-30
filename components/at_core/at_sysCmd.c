
/*
 * File : at_sysCmd.c
 * This file is part of Espressif's AT+ command set program.
 * Copyright (C) 2013 - 2016, Espressif Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "at_version.h"
#include "at.h"

#include "esp_system.h"

uint8_t at_queryCmdSysRam(uint8_t *cmd_name)
{
    uint8_t buffer[32];
    at_sprintf(buffer, "%s:%d",cmd_name,esp_get_free_heap_size());
    at_port_print(buffer);

    return ESP_AT_RESULT_CODE_OK;
}


