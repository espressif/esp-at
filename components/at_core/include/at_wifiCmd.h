/*
 * File : at_wifiCmd.h
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
#ifndef __AT_WIFICMD_H
#define __AT_WIFICMD_H

#include "at.h"

#define MAX_AP_RECORD_LENGTH    128
#define MAX_AP_LIST_LENGTH      5000

uint8_t at_testCmdCwmode(uint8_t *cmd_name);
uint8_t at_queryCmdCwmode(uint8_t *cmd_name);
uint8_t at_queryCmdCwmodeDef(uint8_t *cmd_name);
uint8_t at_setupCmdCwmode(uint8_t para_num);
uint8_t at_setupCmdCwmodeDef(uint8_t para_num);

//void at_testCmdCwjap(uint8_t id);
uint8_t at_queryCmdCwjap(uint8_t *cmd_name);
uint8_t at_queryCmdCwjapDef(uint8_t *cmd_name);
uint8_t at_setupCmdCwjap(uint8_t para_num);
uint8_t at_setupCmdCwjapDef(uint8_t para_num);

uint8_t at_setupCmdCwlap(uint8_t para_num);
uint8_t at_exeCmdCwlap(uint8_t *cmd_name);
uint8_t at_setupCmdCwlapOpt(uint8_t para_num);

uint8_t at_testCmdCwqap(uint8_t *cmd_name);
uint8_t at_exeCmdCwqap(uint8_t *cmd_name);

uint8_t at_queryCmdCwsap(uint8_t *cmd_name);
uint8_t at_queryCmdCwsapDef(uint8_t *cmd_name);
uint8_t at_setupCmdCwsap(uint8_t para_num);
uint8_t at_setupCmdCwsapDef(uint8_t para_num);

uint8_t at_exeCmdCwlif(uint8_t *cmd_name);

uint8_t at_queryCmdCwdhcp(uint8_t *cmd_name);
uint8_t at_queryCmdCwdhcpDef(uint8_t *cmd_name);
uint8_t at_setupCmdCwdhcp(uint8_t para_num);
uint8_t at_setupCmdCwdhcpDef(uint8_t para_num);

uint8_t at_queryCmdCipstamac(uint8_t *cmd_name);
uint8_t at_queryCmdCipstamacDef(uint8_t *cmd_name);
uint8_t at_setupCmdCipstamac(uint8_t para_num);
uint8_t at_setupCmdCipstamacDef(uint8_t para_num);

uint8_t at_queryCmdCipapmac(uint8_t *cmd_name);
uint8_t at_queryCmdCipapmacDef(uint8_t *cmd_name);
uint8_t at_setupCmdCipapmac(uint8_t para_num);
uint8_t at_setupCmdCipapmacDef(uint8_t para_num);

uint8_t at_queryCmdCipsta(uint8_t *cmd_name);
uint8_t at_queryCmdCipstaDef(uint8_t *cmd_name);
uint8_t at_setupCmdCipsta(uint8_t para_num);
uint8_t at_setupCmdCipstaDef(uint8_t para_num);

uint8_t at_queryCmdCipap(uint8_t *cmd_name);
uint8_t at_queryCmdCipapDef(uint8_t *cmd_name);
uint8_t at_setupCmdCipap(uint8_t para_num);
uint8_t at_setupCmdCipapDef(uint8_t para_num);

uint8_t at_setupCmdCwAutoConn(uint8_t para_num);

uint8_t at_queryCmdCwAutoConn(uint8_t *cmd_name);

#ifdef AT_SMARTCONFIG_SUPPORT
uint8_t at_setupCmdSmartStart(uint8_t para_num);
uint8_t at_exeCmdSmartStop(uint8_t *cmd_name);
#endif

uint8_t at_queryCmdCwdhcps(uint8_t *cmd_name);
uint8_t at_setupCmdCwdhcps(uint8_t para_num);
uint8_t at_queryCmdCwdhcpsDef(uint8_t *cmd_name);
uint8_t at_setupCmdCwdhcpsDef(uint8_t para_num);

#ifdef AT_WPS_SUPPORT
uint8_t at_setupCmdWps(uint8_t para_num);
#endif

uint8_t at_queryCmdCwHostname(uint8_t *cmd_name);
uint8_t at_setupCmdCwHostname(uint8_t para_num);

uint8_t at_queryCmdCipsntpcfg(uint8_t *cmd_name);
uint8_t at_setupCmdCipsntpcfg(uint8_t para_num);
uint8_t at_exeCmdCipsntpcfg(uint8_t *cmd_name);

uint8_t at_queryCmdCipsntptime(uint8_t *cmd_name);


#endif
