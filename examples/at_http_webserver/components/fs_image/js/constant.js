
var LOGIN_USER = "login_user";
var SERVER_HTTP = "";

var SAVE_STOP = false;
var SAVE_STOP_ITEM = "SAVE_STOP";

var DEVICE_STATUS = 1;

var SUCCESS_CODE = 0;

var ENABLE_CODE = 1;

var BROADCAST_LIST = [ { "addr": "24:0a:c4:03:f6:92", "adv_data": "adasdasdasdasdasd" },
    {"addr": "24:0a:c4:03:a2:aa", "adv_data": "dfdfdfdfdfdfdfd" } ];

var STATION_LIST = [];

var TCP_CLASS = {"0": "Client", "1": "Server"}

var ERROR_INFO = {"406": "请求类型错误", "400": "请求参数错误", "500": "服务器执行错误", "600": "密码校验错误"};

var INIT_CHANNEL = [{key: 1, value: 1},{key: 2, value: 2},{key: 3, value: 3}, {key: 4, value: 4},{key: 5, value: 5},
    {key: 6, value: 6},{key: 7, value: 7},{key: 8, value: 8},{key: 9, value: 9},{key: 10, value: 10},
    {key: 11, value: 11},{key: 12, value: 12},{key: 13, value: 13},{key: 14, value: 14}];

var INIT_ENCRYPTION = [{key: 0, value: "OPEN"},{key: 2, value: "WPAPSK"},{key: 3, value: "WPA2PSK"},
    {key: 4, value: "WPAWPA2PSK"}];

var INIT_MACCON = [{key: 1, value: 1},{key: 2, value: 2},{key: 3, value: 3}, {key: 4, value: 4},{key: 5, value: 5},
    {key: 6, value: 6},{key: 7, value: 7},{key: 8, value: 8},{key: 9, value: 9},{key: 10, value: 10}];

