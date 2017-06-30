/***************************************************************************//**
 * @file nanopb.c
 *
 * Created on June 16, 2016, 2:41 PM@n
 * Copyright 2016 SimpliSafe All Rights Reserved@n
 * Author(s): Sasha Rosca
 ******************************************************************************/

#include "nanopb.h"
//#include "mbedtls/md5.h"

#define AUDIO_START_ADDR    0x0
#define FW_START_ADDR       0x400000

#define MD5_STR_LEN  32
#define MD5_BYTE_LEN 16

typedef enum pb_file_type_s
{
    BS,
    AUDIO,
    WIFI,
    KP
} pb_file_type;

typedef struct pb_file_dl_s
{
    pb_file_type type;
    uint32_t fw_progress;
    uint32_t start_addr;
    uint32_t total_size;
    mbedtls_md5_context md5_ctx;
} pb_file_dl;

pb_file_dl dl_table[] = 
{
    {BS,    0,      0x400000,   0,  {{0}}},
    {AUDIO, 0,      0x0,        0,  {{0}}},
    {WIFI,  0,      0,          0,  {{0}}},
    {KP,    0,      0,          0,  {{0}}}
};

pb_file_type dl_table_ind = 0;

uint8_t imsi_temp[16] = "0123456789\0";
uint8_t account[16] = "Sasha Rosca\0";
uint8_t serial[6] = "12345\0";
uint8_t name[16] = "Simpli Sensor\0";
uint8_t build_version[16] = "Whatisthisfor\0";
uint8_t version_major = 1;
uint8_t version_minor = 0;
uint8_t version_build = 1;
uint8_t con[5];

//fw update globals
//Location in buffer of image block
uint16_t img_loc;
//Size if image block in buffer
uint16_t img_block_size;
//md5 sum of total image
uint8_t md5sum[32];

//settings globals
uint8_t got_obiwan_addrA = 0;
uint8_t got_obiwan_addrB = 0;
uint8_t got_sensor_name = 0;

enum 
{
    NANO_OBIADDRA,
    NANO_OBIADDRB,
    NANO_PIN_NAME,
    NANO_SERIAL,
    NANO_SEN_NAME,
    NANO_MD5,
    NANO_ECHO
};

void pb_clear_flags()
{
    got_obiwan_addrA = 0;
    got_obiwan_addrB = 0;
    got_sensor_name = 0;
}

#define RF_SN_SIZE                      4



/***************************************************************************//**
 * 
 * @param stream Stream to encode bytes to
 * @param field Field of which type to encode the bytes
 * @param arg The buffer of bytes to encode
 * @return Success
 ******************************************************************************/

static bool pb_encode_bytes(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;
    return pb_encode_string(stream, (uint8_t*) * arg, strlen(*arg));
}

static bool pb_encode_serial(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;
    return pb_encode_string(stream, (uint8_t*) * arg, RF_SN_SIZE);
}

/*******************************
 * ENCODE MESSAGE
 *******************************/

/***************************************************************************//**
 * 
 * @param buffer Buffer into which to encode the login
 * @return Returns the number of bytes encoded
 ******************************************************************************/

uint8_t pb_encode_login(uint8_t * buffer,pb_protocol_t protocol)
{
    ss_proto_backend_basestation_Login login_formatted = ss_proto_backend_basestation_Login_init_zero;
    uint8_t  mfgSerialNum[] = {0x07,0x01,0x00,0x01};
    
    login_formatted.imsi.arg = imsi_temp;
    login_formatted.imsi.funcs.encode = &pb_encode_bytes;

    login_formatted.has_version_build = true;
    login_formatted.version_build = version_build;

    login_formatted.has_version_major = true;
    login_formatted.version_major = version_major;

    login_formatted.has_version_minor = true;
    login_formatted.version_minor = version_minor;

    login_formatted.has_protocol_version = true;
    login_formatted.protocol_version = ss_proto_backend_basestation_Login_Version_V0;

    login_formatted.build_version.arg = build_version;
    login_formatted.build_version.funcs.encode = &pb_encode_bytes;
    
    login_formatted.has_link_type = true;
    switch(protocol)
    {
    case NANO_WIFI:
        login_formatted.link_type = ss_proto_backend_basestation_LinkType_WIFI;
        break;
    case NANO_CELL:
        login_formatted.link_type = ss_proto_backend_basestation_LinkType_CELLULAR;
        break;
    case NANO_ETHERNET:
        login_formatted.link_type = ss_proto_backend_basestation_LinkType_ETHERNET;
        break;
    }
    
    login_formatted.serial.arg = mfgSerialNum;
    login_formatted.serial.funcs.encode = &pb_encode_serial;

    return pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type_LOGIN, (uint8_t*) & login_formatted, buffer);

}

/***************************************************************************//**
 * 
 * @param data The data from the file being transferred
 * @param type The position in the file (First, Middle, Last) that is being sent
 * @param buffer The buffer into which to encode the transfer
 * @return Returns the number of bytes encoded
 ******************************************************************************/

uint16_t pb_encode_transfer(uint8_t* data, ss_proto_backend_basestation_LogFileTransfer_Type type, uint8_t * buffer)
{

    ss_proto_backend_basestation_LogFileTransfer file_formatted = ss_proto_backend_basestation_LogFileTransfer_init_zero;
    file_formatted.type = type;

    file_formatted.filename.arg = "test.txt";
    file_formatted.filename.funcs.encode = &pb_encode_bytes;

    file_formatted.data.arg = data;
    file_formatted.data.funcs.encode = &pb_encode_bytes;

    return pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type_LOG_FILE, (uint8_t*) & file_formatted, buffer);

}

/***************************************************************************//**
 *
 * @param type Type of message to encode
 * @param field The submessage to be encoded
 * @param buffer Buffer to encode message to
 * @return Returns the number of bytes encoded
 ******************************************************************************/

uint16_t pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type type, uint8_t* field, uint8_t* buffer)
{
    //printf("pb_encode_message()\n");
    ss_proto_backend_basestation_MessageToBackend message = ss_proto_backend_basestation_MessageToBackend_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, PB_BUFFER_SIZE);

    message.type = type;

    switch (type)
    {
    case ss_proto_backend_basestation_MessageToBackend_Type_KEEP_ALIVE_REPLY:
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_SETUP:
        message.has_setup = true;
        message.setup = *(ss_proto_backend_basestation_Setup*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_LOGIN:
        message.has_login = true;
        message.login = *(ss_proto_backend_basestation_Login*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_GET_STATUS_REPLY:
        message.has_getStatusReply = true;
        message.getStatusReply = *(ss_proto_backend_basestation_GetStatusReply*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_GET_STATE_REPLY:
        message.has_state = true;
        message.state = *(ss_proto_backend_basestation_State*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_SET_STATE_REPLY:
        message.has_state = true;
        message.state = *(ss_proto_backend_basestation_State*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_GET_SETTINGS_REPLY:
        message.has_settings = true;
        message.settings = *(ss_proto_backend_basestation_Settings*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_SET_SETTINGS_REPLY:
        message.has_settings = true;
        message.settings = *(ss_proto_backend_basestation_Settings*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_STATE_CHANGE_EVENT:
        message.has_state = true;
        message.state = *(ss_proto_backend_basestation_State*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_ALARM:
        message.has_alarm = true;
        message.alarm = *(ss_proto_backend_basestation_Alarm*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_GET_CONNECTIVITY_REPLY:
        message.has_connectivity = true;
        message.connectivity = *(ss_proto_backend_basestation_Connectivity*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_FIRMWARE_UPDATE_REPLY:
        message.has_update = true;
        message.update = *(ss_proto_backend_basestation_FirmwareUpdateReply*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_UPLOAD_REPLY:
        message.has_upload = true;
        message.upload = *(ss_proto_backend_basestation_UploadReply*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_LOG_FILE:
        message.has_logFileTransfer = true;
        message.logFileTransfer = *(ss_proto_backend_basestation_LogFileTransfer*) field;
        break;
    case ss_proto_backend_basestation_MessageToBackend_Type_ECHO_REPLY:
        message.has_echo = true;
        message.echo = *(ss_proto_backend_basestation_EchoReply*) field;
        break;
    default:
        break;
    }

    if (!pb_encode(&stream, ss_proto_backend_basestation_MessageToBackend_fields, &message))
    {
        printf("Encode failed: %d\n", (unsigned int) stream.bytes_written);
        return 0;
    }
    else
    {
        //printf("Encode succeeded: %d\n",(unsigned int)stream.bytes_written);
        return (uint16_t) stream.bytes_written;
    }
    return 0;
}

/*******************************************************************************
 * DECODE MESSAGE
 ******************************************************************************/

/***************************************************************************//**
 * 
 * @param stream Stream to decode bytes from
 * @param field Unused
 * @param arg Unused
 * @return True if successful, false if unsuccessful
 ******************************************************************************/

bool pb_decode_bytes(pb_istream_t *stream, const pb_field_t *field, void** arg)
{
    uint8_t buf[64] = {0};
    size_t len = stream->bytes_left;

    if (len > sizeof (buf) - 1 || !pb_read(stream, buf, len))
        return false;
        
    if(*arg == (void*)NANO_MD5)
    {
        strncpy((char*)md5sum,(char*)buf,MD5_STR_LEN);
    }
    
    if(*arg == (void*)NANO_ECHO)
    {
        
    }
    
    return true;
}
/***************************************************************************//**
 * Callback to grab fw block from protobuf message
 * @param stream
 * @param field
 * @param arg
 * @return 
 ******************************************************************************/
bool pb_decode_fw(pb_istream_t *stream, const pb_field_t *field, void** arg)
{
   img_block_size = stream->bytes_left;
   img_loc = stream->ptrcallback(img_block_size);
   stream->bytes_left -= img_block_size;
   //printf("Got image: %d\n",img_block_size);
   return true;
}

/***************************************************************************//**
 * 
 * @param buffer Buffer from which to decode transfer
 * @param len Size of transfer
 * @return 
 ******************************************************************************/

uint8_t pb_decode_transfer(uint8_t* buffer, uint16_t len)
{

    pb_istream_t stream = pb_istream_from_buffer(buffer, len);

    ss_proto_backend_basestation_MessageToBackend message = ss_proto_backend_basestation_MessageToBackend_init_zero;

    message.logFileTransfer.filename.funcs.decode = &pb_decode_bytes;
    message.logFileTransfer.data.funcs.decode = &pb_decode_bytes;

    if (!pb_decode(&stream, ss_proto_backend_basestation_MessageToBackend_fields, &message))
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return 0;
    }

    printf("File type: %d\n", message.logFileTransfer.type);

    return 1;
}

/***************************************************************************//**
 * 
 * @param upload Upload struct to decode
 * @param send Pointer to send function to be used for replies
 * @param reply_buf Bufer into which to encode the reply
 * @return Success
 ******************************************************************************/

uint8_t pb_decode_upload(ss_proto_backend_basestation_Upload upload, uint8_t* send_flag, uint8_t* reply_buf,uint16_t* reply_size)
{

    //printf("Upload-");
    switch (upload.type)
    {
    case ss_proto_backend_basestation_Upload_Type_SS_LOG_FILE:
        /*printf("LogFile\n");
        *reply_size = pb_encode_transfer((uint8_t*) "", ss_proto_backend_basestation_LogFileTransfer_Type_FIRST_PART, reply_buf);
        *send_flag = 1;
        *reply_size = pb_encode_transfer((uint8_t*) "Hello World!", ss_proto_backend_basestation_LogFileTransfer_Type_MIDDLE_PART, reply_buf);
        *send_flag = 1;
        *reply_size = pb_encode_transfer((uint8_t*) "", ss_proto_backend_basestation_LogFileTransfer_Type_LAST_PART, reply_buf);
        *send_flag = 1;*/
        break;
    case ss_proto_backend_basestation_Upload_Type_CORE_DUMP:
        break;
    case ss_proto_backend_basestation_Upload_Type_WIFI_LOG:
        break;
    case ss_proto_backend_basestation_Upload_Type_DMESG:
        break;
    case ss_proto_backend_basestation_Upload_Type_PROCS:
        break;
    default:
        break;
    }

    return 1;
}

/***************************************************************************//**
 * @param echo Incoming echo message
 * @return 
 ******************************************************************************/

uint8_t pb_decode_echo(ss_proto_backend_basestation_EchoRequest echo, uint8_t* reply_buf)
{
    //printf("pb_decode_echo()\n");
    ss_proto_backend_basestation_EchoReply reply;
    
    reply.has_number = true;
    reply.number = echo.number;
    reply.status = ss_proto_backend_basestation_EchoReply_Status_STATUS_OK;
    
    reply.data.arg = "bs";
    reply.data.funcs.encode = &pb_encode_bytes;
    
    return pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type_ECHO_REPLY,(uint8_t*)&reply,reply_buf);
}

/***************************************************************************//**
 * @param in_stream Stream from which to decode message
 * @param send Pointer to send function to be used for replies
 * @param reply_buf Bufer into which to encode the reply
 * @return 
 ******************************************************************************/

uint8_t pb_decode_message(pb_istream_t in_stream, uint8_t* send_flag, uint8_t* reply_buf, uint16_t* reply_size)
{
    ss_proto_backend_basestation_MessageToBasestation message = ss_proto_backend_basestation_MessageToBasestation_init_zero;
    
    //printf("pb_decode_message()\n");
    
    //Setup the decoding callbacks
    message.settings.admin.obiwanA.funcs.decode = &pb_decode_bytes;
    message.settings.admin.obiwanA.arg = (void*)NANO_OBIADDRA;
    
    message.settings.admin.obiwanB.funcs.decode = &pb_decode_bytes;
    message.settings.admin.obiwanB.arg = (void*)NANO_OBIADDRB;
    
    //pin_count = 0;
    //message.settings.pins.pins.funcs.decode = &pb_decode_pins;
    
    //message.settings.sensors.funcs.decode = &pb_decode_sensors;
    
    //message.update.data.funcs.decode = &pb_decode_fw;
    
    message.update.md5sum.funcs.decode = &pb_decode_bytes;
    message.update.md5sum.arg = (void*)NANO_MD5;
    
    message.echo.data.funcs.decode = &pb_decode_bytes;
    message.echo.data.arg = (void*)NANO_ECHO;
    
    if (!pb_decode(&in_stream, ss_proto_backend_basestation_MessageToBasestation_fields, &message))
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&in_stream));
        return 0;
    }

    //(*callback)();

    switch (message.type)
    {
    /*case ss_proto_backend_basestation_MessageToBasestation_Type_KEEP_ALIVE:
        printf("Keep Alive\n");
        *reply_size = pb_encode_message(ss_proto_backend_basestation_MessageToBackend_Type_KEEP_ALIVE_REPLY, NULL, reply_buf);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_SETUP_REPLY:
        printf("Setup Reply\n");
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_LOGIN_REPLY:
        printf("Login Reply\n");
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_GET_STATUS_REQ:
        printf("Get Status\n");
        *reply_size = pb_encode_status(reply_buf);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_GET_STATE:
        printf("Get State\n");
        *reply_size = pb_encode_state(systemMode,reply_buf,NANO_GET);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_SET_STATE:
        printf("Set State\n");
        pb_decode_state(message.state);
        *reply_size = pb_encode_state(systemMode,reply_buf,NANO_SET);
        *send_flag = 1;
        return 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_GET_SETTINGS:
        printf("Get Settings\n");
        *reply_size = pb_encode_settings(ss_proto_backend_basestation_MessageToBasestation_Type_GET_SETTINGS, reply_buf, message.settings.groups);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_SET_SETTINGS:
        printf("Set Settings\n");
        *reply_size = pb_decode_settings(message.settings,reply_buf);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_ADD_SENSOR:
        printf("Add sensor\n");
        pb_decode_addsensor(message.addSensor);
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_ACK_ALARM:
        printf("Ack Alarm\n");
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_TRIGGER_ALARM:
        printf("Trigger Alarm\n");
        pb_decode_alarm(message.alarm);
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_GET_CONNECTIVITY:
        *reply_size = pb_encode_conn(reply_buf);
        *send_flag = 1;
        printf("Get Connectivity\n");
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_FIRMWARE_UPDATE:
        //printf("Firmware Update-");
        *reply_size = pb_decode_update(&in_stream,message.update,reply_buf);
        *send_flag = 1;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_UPLOAD:
        pb_decode_upload(message.upload, send_flag, reply_buf, reply_size);
        return 2;
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_DISCONNECT:
        printf("Disconnect\n");
        break;
    case ss_proto_backend_basestation_MessageToBasestation_Type_ECHO_REQUEST:
        *reply_size = pb_decode_echo(message.echo, reply_buf);
        *send_flag = 1;
        break;*/
    default:
        break;
    }

    return 1;
}

/******************************
 * TEST SECTION
 ******************************/
void pb_test()
{
    uint8_t buffer[128];
    ss_proto_backend_basestation_Settings set = ss_proto_backend_basestation_Settings_init_zero;
    ss_proto_backend_basestation_Settings set_ret = ss_proto_backend_basestation_Settings_init_zero;

    set.sensors.arg = false;
    set.sensors.funcs.encode = NULL;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof (buffer));

    if (!pb_encode(&stream, ss_proto_backend_basestation_Settings_fields, &set))
    {
        printf("Encode failed: %d\n", (unsigned int) stream.bytes_written);
    }
    else
    {
        printf("Encode succeeded: %d Size: %d\n", (unsigned int) stream.bytes_written, sizeof (buffer));
    }

    pb_istream_t in_stream = pb_istream_from_buffer(buffer, sizeof (buffer));

    set_ret.sensors.funcs.decode = &pb_decode_sensors;

    printf("Bytes left: %d\n", (unsigned int) in_stream.bytes_left);

    if (!pb_decode(&in_stream, ss_proto_backend_basestation_Settings_fields, &set_ret))
    {
        printf("Decode failed: %s\n", PB_GET_ERROR(&in_stream));
        return;
    }
    else
    {
        printf("Decode succeeded: %d\n", (unsigned int) in_stream.bytes_left);
    }

    return;
}

bool pb_scratch()
{
    uint8_t buffer[128];
    memset(buffer, 0, 128);

    ss_proto_backend_basestation_MessageToBasestation message = ss_proto_backend_basestation_MessageToBasestation_init_zero;
    message.type = ss_proto_backend_basestation_MessageToBasestation_Type_KEEP_ALIVE;
    message.has_alarm = false;
    message.has_settings = false;
    message.has_state = false;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof (buffer));

    if (!pb_encode(&stream, ss_proto_backend_basestation_MessageToBasestation_fields, &message))
    {
        printf("Encode failed: %d\n", (unsigned int) stream.bytes_written);
    }
    else
    {
        printf("Encode succeeded: %s\n", buffer);
        int i;
        for (i = 0; i < 10; i++)
        {
            printf("Char %u\n", (uint8_t) buffer[i]);
        }
    }

    return true;
}