#ifndef CGIAT_H
#define CGIAT_H

#include "esp_log.h"
#include "httpd.h"
#include "esp_parse_at.h"

#define MAX_SHOW_SCAN_AP 10

int cgiConnAp(HttpdConnData *connData);

int cgiSetAp(HttpdConnData *connData);

int getRamSize(HttpdConnData *connData);

int getConnDev(HttpdConnData *connData);

int cgiping(HttpdConnData *connData);

int cgiloginpwd(HttpdConnData *connData);

int cgiCloseAp(HttpdConnData *connData);

int cgiSetDhcp(HttpdConnData *connData);

int cgiSetIpInfo(HttpdConnData *connData);

int cgiChangeLoginPwd(HttpdConnData *connData);

int cgirst(HttpdConnData *connData);

int cgirestore(HttpdConnData *connData);

int cgiConnIp(HttpdConnData *connData);

int cgiCloseIp(HttpdConnData *connData);

int cgiGetIpStatus(HttpdConnData *connData);

int cgiBleSet(HttpdConnData *connData);

int cgiBleScan(HttpdConnData *connData);

int cgiSetStaIpInfo(HttpdConnData *connData);

int cgiSetApIpInfo(HttpdConnData *connData);

int cgiSetStaMac(HttpdConnData *connData);

int cgiSetApMac(HttpdConnData *connData);

int cgiRouteInfo(HttpdConnData* connData);

int cgiShowApInfo(HttpdConnData* connData);

int cgiConfigApInfo(HttpdConnData* connData);

int cgiShowDhcp(HttpdConnData* connData);

int cgiGetBleSet(HttpdConnData* connData);

int cgiBleConn(HttpdConnData* connData);

int cgiGetBleConn(HttpdConnData* connData);

int cgiBleCloseConn(HttpdConnData* connData);

int cgiScanAp(HttpdConnData* connData);

int cgiBleAdvSet(HttpdConnData* connData);

int cgiGetBleAdvSet(HttpdConnData* connData);

int cgiBleAdvStart(HttpdConnData* connData);

int cgiBleAdvStop(HttpdConnData* connData);

#endif
