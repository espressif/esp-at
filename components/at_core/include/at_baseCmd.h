/*
 * File : at_baseCmd.h
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
#ifndef __AT_BASECMD_H
#define __AT_BASECMD_H
bool at_get_echo_flag(void);
void at_set_echo_flag(bool enable);
void at_set_custom_version(const uint8_t *info);

uint8_t at_exeCmdNull(uint8_t *cmd_name);
uint8_t at_exeCmdE0(uint8_t *cmd_name);
uint8_t at_exeCmdE1(uint8_t *cmd_name);
uint8_t at_exeCmdRst(uint8_t *cmd_name);
uint8_t at_exeCmdGmr(uint8_t *cmd_name);

uint8_t at_setupCmdIpr(uint8_t para_num);

uint8_t at_setupCmdGslp(uint8_t para_num);

uint8_t at_execCmdRestore(uint8_t *cmd_name);

#ifdef AT_SLEEP_SUPPORT
uint8_t at_setupCmdSleep(uint8_t para_num);
uint8_t at_queryCmdSleep(uint8_t *cmd_name);
#endif
#endif
