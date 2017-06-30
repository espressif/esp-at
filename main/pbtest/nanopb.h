/**********************************************************************
 * File:   nanopb.h
 *
 * Created on June 16, 2016, 2:41 PM
 *
 * Copyright 2016 SimpliSafe All Rights Reserved
 *********************************************************************/

#ifndef _NANOPB_H_
#define _NANOPB_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "backend_basestation.pb.h"

#define PB_BUFFER_SIZE 2048

#define PB_GROUP_ADMIN      0x1
#define PB_GROUP_PINS       0x2
#define PB_GROUP_SENSORS    0x4
#define PB_GROUP_NORMAL     0x8

//Debug obiwan IP
#ifdef DEVELOPMENT_ENV_BACKEND
#define DEBUG_OBIWAN_A_IP "bb1.qa.simplisafe.com"
#define DEBUG_OBIWAN_B_IP "bb2.qa.simplisafe.com"
#elif defined(DEVELOPMENT_ENV_BRANAM)
#define DEBUG_OBIWAN_A_IP "52.87.240.137"
#define DEBUG_OBIWAN_B_IP "0.0.0.0"
#else
#define DEBUG_OBIWAN_A_IP "bb1.qa.simplisafe.com"
#define DEBUG_OBIWAN_B_IP "bb2.qa.simplisafe.com"
#endif

typedef struct pb_alert 
{
    uint8_t code;
    uint8_t sensor;
    uint8_t *serial;
    char *name;
} pb_alert_t;

typedef enum pb_protocol
{
    NANO_WIFI,
    NANO_ETHERNET,
    NANO_CELL
} pb_protocol_t;

enum
{
    NANO_INIT,
    NANO_UPDATE,
    NANO_FINISH
};

enum
{
    NANO_GET,    // Synchronous reponse to GET request.
    NANO_SET,    // Synchronous reponse to SET request.
    NANO_NOTIFY  // Asynchronous notification.
};

extern uint32_t fw_progress;

//***ENCODE***
//Alarm
uint16_t pb_encode_alert(pb_alert_t*,uint8_t*);

//State
uint16_t pb_encode_state(uint8_t, uint8_t*,uint8_t);

//Message to Backend
uint16_t pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type,uint8_t*,uint8_t*);

//Login
uint8_t pb_encode_login(uint8_t*,pb_protocol_t);

bool pb_encode_status_sensors(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
uint16_t pb_encode_status(uint8_t * buffer);

//***DECODE***
//Upload
uint8_t pb_decode_upload(ss_proto_backend_basestation_Upload,uint8_t*,uint8_t*,uint16_t*);

//Message to Basestation
uint8_t pb_decode_message(pb_istream_t,uint8_t*,uint8_t*, uint16_t*);

uint16_t pb_encode_settings(uint8_t,uint8_t*,uint32_t);

//Sensors
bool pb_encode_sensors(pb_ostream_t*, const pb_field_t*, void * const *);
bool pb_decode_sensors(pb_istream_t*,const pb_field_t*, void**);

void pb_test(void);
bool pb_scratch();

#endif