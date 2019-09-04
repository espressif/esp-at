$(function() {
    isLogin();
    eventBind();
    var status = sessionStorage.getItem(SAVE_STOP_ITEM);
    if (status == "true") {
        AT.bleStop("broadcast-stop");
    }
})
function isLogin() {
    var userInfo = sessionStorage.getItem(LOGIN_USER);
    if (isEmpty(userInfo)) {
        window.location = window.location.origin + "/login.html";
    }
}
function logout() {
    sessionStorage.clear();
    window.location = "/login.html";
}
function eventBind() {
    $(document).on("click", "#softap-btn", function() {
        AT.setSoftap();
    });
    $(document).on("click", "#station-btn", function() {
        AT.setStation();
    });
    $(document).on("click", "#disconnect-ap", function() {
        AT.disconnectAp();
    });
    $(document).on("click", "#wifi-btn", function() {
        AT.setWifi();
    });
    $(document).on("click", "#dhcp-btn", function() {
        AT.setDhcp();
    });
    $(document).on("click", "#tcp-btn", function() {
        AT.setTcp();
    });
    $(document).on("click", "#ble-connect", function() {
        AT.bleConnect();
    });
    $(document).on("click", "#ble-btn", function() {
        AT.setBle();
    });
    $(document).on("click", "a.user-logout", function() {
        logout();
    });
    $(document).on("click", "button.broadcast-btn", function() {
        var doc = $(this),
            id = doc.attr("data-id");
        doc.text("Rescan");
        AT.bleScan(doc.attr("data-value"), id);
    });
    $(document).on("click", "#restart-btn", function() {
        AT.restartDevice();
    });
    $(document).on("click", "#search-ram", function() {
        AT.getRamSize();
    });
    $(document).on("click", "#ping-btn", function() {
        AT.pingIp();
    });
    $(document).on("click", "#reset-btn", function() {
        AT.restoreDevice();
    });
    $(document).on("click", "#password-btn", function() {
        AT.changePwd();
    });
    $(document).on("click", "button.btn-tcp", function() {
        AT.disconnectTcp($(this).attr("data-id"));
    });
    $(document).on("click", "button.btn-ble", function() {
        AT.closeBle($(this).attr("data-id"));
    });
    $(document).on("click", "a.advanced-options", function() {
        var doc = $(this).parent();
        if (doc.hasClass("active")) {
            doc.removeClass("active").siblings().addClass("hidden");
        } else {
            doc.addClass("active").siblings().removeClass("hidden");
        }
    });
    $(document).on("click", "#dhcp-server input", function() {
        var val = $(this).val();
        if (val == DEVICE_STATUS) {
            $("#dhcp-server-ip").removeClass("hidden");
        } else {
            $("#dhcp-server-ip").addClass("hidden");
        }
    });
    $(document).on("click", "#keep-alive input", function() {
        var val = $(this).val();
        if (val == DEVICE_STATUS) {
            $("#keep-alive-time").removeClass("hidden");
        } else {
            $("#keep-alive-time").addClass("hidden");
        }
    });
    $(document).on("click", "#select-type .input-radio", function() {
        var id = $(this).find("input").attr("data-id");
        $("#" + id).removeClass("hidden").siblings().addClass("hidden");
    });
    $("#connection-type").change(function() {
        var dataValue = $(this).siblings("ul").find("li.selected").attr("data-value");
        console.log(dataValue);
        $("div[id^='select-connection']").addClass("hidden");
        $("#select-connection-" + dataValue).removeClass("hidden");
    });
    $("#device-type").change(function() {
        var dataValue = $(this).siblings("ul").find("li.selected").attr("data-value");
        if (dataValue == "disconnect") {
            $("#no-disconnect").addClass("hidden");
            $("#broadcast-set").addClass("hidden");

        } else {
            if (dataValue == "client") {
                AT.getBleClient();
                $("#broadcast-set").addClass("hidden");
            } else {
                AT.getBleServer();
                $("#broadcast-set").removeClass("hidden");
            }
            $("#no-disconnect").removeClass("hidden");
            $("div[id^='select-device']").addClass("hidden");
            $("#select-device-" + dataValue).removeClass("hidden");
        }
        var status = sessionStorage.getItem(SAVE_STOP_ITEM);
        if (status) {
            AT.bleStop("broadcast-stop");
        }
    });
    $(document).on("change", "#select-mac", function() {
        var val = $(this).val();
        $.each(BROADCAST_LIST, function(i, item) {
            if (item.addr == val) {
                $("#adv-data").val(item.adv_data);
            }
        })
    });
    $(document).on("change", "#ssid", function() {
        var val = $(this).val();
        $.each(STATION_LIST, function(i, item) {
            if (item.ssid == val) {
                $("#bssid").val(item.mac);
            }
        })
    });
    $("#disconnect").click(function() {
        var doc = $(this);
        if (doc.hasClass("off")) {
            doc.addClass("on").removeClass("off");
            doc.attr("data-value", "1");
        } else {
            doc.addClass("off").removeClass("on");
            doc.attr("data-value", "0");
        }
    });
    $(document).on("click", "div.btn-broadcast button", function() {
        var val = $(this).attr("id");
        if (val == "broadcast-start") {
            AT.bleStart(val);
        } else {
            AT.bleStop(val);
        }
    });
}
var AT = {
    returnHome: function() {
        window.location = window.location.origin + "/home.html";
    },
    getRouteinfo: function() {
        $.getJSON("/routeinfo", function(resp) {
            if (resp.state == SUCCESS_CODE) {
                $("#ap-ip").val(resp.apip);
                $("#ap-mac").val(resp.apmac);
                $("#ap-gateway").val(resp.apgateway);
                $("#ap-netmask").val(resp.apnetmask);
                $("#sta-ip").val(resp.staip);
                $("#sta-mac").val(resp.stamac);
                $("#sta-gateway").val(resp.stagateway);
                $("#sta-netmask").val(resp.stanetmask);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    getConfigapinfo: function() {
        var seft = this;
        $.getJSON("/configapinfo", function(resp) {
            if (resp.state == SUCCESS_CODE) {
                $("#ssid").val(resp.ssid);
                $("#password").val(resp.pwd);
                $("#hide-ssid input:radio[value='" + resp.ssidhidden + "']").attr('checked','true');
                seft._initSelect("connection", INIT_MACCON, "key", "value", resp.maxconn, true);
                seft._initSelect("channel", INIT_CHANNEL, "key", "value", resp.chl, true);
                seft._initSelect("encryption", INIT_ENCRYPTION, "key", "value", resp.ecn, true);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    setSoftap: function(){
        var self = this;
        var ssid = $("#ssid").val();
        var password = $("#password").val();
        var channel = $("#channel").val();
        var encryption = self._getSelect("encryption");
        var connection = $("#connection").val();
        var hideSsid = $("#hide-ssid input[type='radio']:checked").val();
        if (isEmpty(ssid)) {
            toastr.warning("Please enter the ssid!");
            return false;
        }
        if (isEmpty(password)) {
            toastr.warning("Please enter the password!");
            return false;
        }
        if (isEmpty(channel)) {
            toastr.warning("Please choose the channel!");
            return false;
        }
        if (isEmpty(encryption)) {
            toastr.warning("Please choose the encryption way!");
            return false;
        }
        if (isEmpty(connection)) {
            toastr.warning("Please choose the maxinum connection!");
            return false;
        }
        if (isEmpty(hideSsid)) {
            toastr.warning("Please choose the hide ssid!");
            return false;
        }
        var data = "ssid="+ ssid +"&pwd=" + password + "&chl=" + channel + "&ecn=" + encryption + "&maxconn=" + connection
            + "&ssidhidden=" + hideSsid;
        console.log(data);
        $.ajax({
            type: "post",
            url: "/setap",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/index.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getScanap: function (ssid) {
        var seft = this;
        $.getJSON("/scanap", function(resp) {
            seft.removeLoad();
            if (resp.state == SUCCESS_CODE) {
                STATION_LIST = resp.wifilist;
                seft._initSelect("ssid", STATION_LIST, "ssid", "ssid", ssid, false);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    getStation: function () {
        var seft = this;
        seft.load();
        $.getJSON("/showapinfo", function(resp) {
            if (resp.state == SUCCESS_CODE) {
                seft.getScanap(resp.ssid);
                $("#password").val(resp.pwd);
                $("#bssid").val(resp.bssid);
                $("#auto-content input:radio[value='" + resp.autoconn + "']").attr('checked','true');
            } else {
                seft.removeLoad();
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    setStation: function(){
        var ssid = $("#ssid").val(),
            password = $("#password").val(),
            bssid = $("#bssid").val(),
            autoContnet = $("#auto-content input[type='radio']:checked").val();
        if (isEmpty(ssid)) {
            toastr.warning("Please enter the ssid!");
            return false;
        }
        if (isEmpty(password)) {
            toastr.warning("Please enter the password!");
            return false;
        }
        var data = "ssid="+ ssid +"&pwd=" + password + "&bssid=" + bssid + "&autoconn=" + autoContnet;
        console.log(data);
        $.ajax({
            type: "post",
            url: "/connap",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/index.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    disconnectAp: function(){
        $.ajax({
            type: "post",
            url: "/closeap",
            headers: {
                "Content-Type": "application/json",
            },
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/index.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getSoftapDevice: function(){
        $.getJSON("/softapdevice", function(resp){
            if (resp.state == SUCCESS_CODE) {
                var doc = $("#softap-device"),
                    devices = resp.connlist, html = "";
                doc.empty();

                $.each(devices, function(i, item) {
                    html += '<div class="col-md-4 col-sm-6 col-xs-12"><div class="item">' +
                        '<div class="item-icon"><i class="icon-device"></i></div>' +
                        '<div class="item-content"><div class="content"><span>IP</span>' +
                        '<span>' + item.connip + '</span></div><div class="content">' +
                        '<span>Mac</span><span>' + item.connmac + '</span></div>' +
                        '</div></div></div>';
                })
                doc.append(html);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    setWifi: function() {
        var softapMac = $("#softap-mac").val(),
            softapIp = $("#softap-ip").val(),
            softapNetmask = $("#softap-netmask").val(),
            softapGateway = $("#softap-gateway").val(),
            stationMac = $("#station-mac").val(),
            stationIp = $("#station-ip").val(),
            stationNetmask = $("#station-netmask").val(),
            stationGateway = $("#station-gateway").val(),
            selectType = $("#select-type input[type='radio']:checked").val(),
            data = "", url = "";
        if (selectType == "softapIp") {
            if (isEmpty(softapIp)) {
                toastr.warning("Please enter the Softap IP!");
                return false;
            }
            url = "/setapipinfo";
            data = "apip=" + softapIp + "&apgateway=" + softapGateway + "&ap_netmask="+ softapNetmask;
        } else if (selectType == "stationIp") {
            if (isEmpty(stationIp)) {
                toastr.warning("Please enter the Station IP!");
                return false;
            }
            url = "/setstaipinfo";
            data = "staip=" + stationIp + "&stagateway=" + stationGateway + "&sta_netmask="+ stationNetmask;
        } else if (selectType == "softapMac") {
            if (isEmpty(softapMac)) {
                toastr.warning("Please enter the Softap MAC!");
                return false;
            }
            url = "/setapmac";
            data = "mac=" + softapMac;
        } else if (selectType == "stationMac") {
            if (isEmpty(stationMac)) {
                toastr.warning("Please enter the Station MAC!");
                return false;
            }
            url = "/setstamac";
            data = "mac=" + stationMac;
        }
        console.log(data + url);
        $.ajax({
            type: "post",
            url: url,
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/index.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getDhcp: function () {
        $.getJSON("/showdhcpinfo", function(resp) {
            if (resp.state == SUCCESS_CODE) {
                $("#dhcp-client input:radio[value='" + resp.dhcpstatus + "']").attr('checked','true');
                $("#dhcp-server input:radio[value='" + resp.enable + "']").attr('checked','true');
                $("#lease-time").val(resp.lease);
                $("#start-ip").val(resp.startip);
                $("#end-ip").val(resp.endip);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    setDhcp: function() {
        var dhcpClient = $("#dhcp-client input[type='radio']:checked").val(),
            dhcpServer = $("#dhcp-server input[type='radio']:checked").val(),
            leaseTime = $("#lease-time").val(),
            startIp = $("#start-ip").val(),
            endIp = $("#end-ip").val(),
            data = "";
        if (dhcpServer == DEVICE_STATUS) {
            if (isEmpty(leaseTime)) {
                toastr.warning("Please enter the Lease time!");
                return false;
            }
            if (isEmpty(startIp)) {
                toastr.warning("Please enter the Start IP!");
                return false;
            }
            if (isEmpty(endIp)) {
                toastr.warning("Please enter the End IP!");
                return false;
            }
            data = "dhcpclient="+dhcpClient+"&dhcpserver="+dhcpServer+"&lease="+leaseTime+"&startip="+startIp+
                "&endip="+endIp;
        } else {
            data = "dhcpclient="+dhcpClient+"&dhcpserver="+dhcpServer;
        }
        console.log(data);
        $.ajax({
            type: "post",
            url: "/setdhcp",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/index.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    setTcp: function() {
        var self = this;
        var keepAlive = $("#keep-alive input[type='radio']:checked").val(),
            connectionType = $("#connection-type").val(),
            connectionId = $("#connection-id").val(),
            remoteIp = $("#remote-ip").val(),
            remotePort = $("#remote-port").val(),
            localPort = $("#local-port").val(),
            udpMode = self._getSelect("udp-mode"),
            keepTime = $("#keep-time").val(),
            data = "";
        if (isEmpty(remoteIp)) {
            toastr.warning("Please enter the Remote IP!");
            return false;
        }
        if (isEmpty(remotePort)) {
            toastr.warning("Please enter the Remote Port!");
            return false;
        }
        if (connectionType == "UDP") {
            data = "linkid="+connectionId+"&type="+connectionType+"&remoteip="+remoteIp+"&remoteport="+remotePort+
                "&localport="+localPort+"&udpmode="+udpMode;
        } else {
            if (keepAlive == DEVICE_STATUS) {
                if (isEmpty(keepTime)) {
                    toastr.warning("Please enter the Keep Alive Time!");
                    return false;
                }
                data = "linkid="+connectionId+"&type="+connectionType+"&remoteip="+remoteIp+"&remoteport="+remotePort+
                    "&keepalive="+keepTime;
            } else {
                data = "linkid="+connectionId+"&type="+connectionType+"&remoteip="+remoteIp+"&remoteport="+remotePort;
            }
        }
        console.log(data);
        $.ajax({
            type: "post",
            url: "/connip",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/pages/tcp/info.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getTcp: function() {
        $.getJSON("/getipstatus", function (resp) {
            if (resp.state == SUCCESS_CODE) {
                var list = resp.iplist,
                    doc = $("#tcp-connect-info"),
                    html = "";
                doc.empty();
                $.each(list, function (i, item) {
                    html += '<div class="col-md-4 col-sm-6 col-xs-12"> <div class="item">' +
                        '<div class="item-icon"><i class="icon-device"></i></div>' +
                        '<div class="item-content"><div class="item-wrapper"> <div class="item-info">' +
                        '<span>IP</span><span>' + item.remoteip + '</span></div></div>' +
                        '<div class="item-wrapper"><div class="item-info"><span>Class</span><span>' + TCP_CLASS[item.tetype] +
                        '</span></div></div><div class="item-wrapper"><div class="item-info">' +
                        '<span>Connect ID</span><span>' + item.linkid + '</span></div></div>' +
                        '<div class="item-wrapper"><div class="item-info"><span>Connect Type</span><span>' + item.type +
                        '</span></div></div><div class="item-wrapper"><div class="item-info">' +
                        '<span>Remote Port</span><span>' + item.remoteport + '</span></div></div>' +
                        '<div class="item-wrapper"><div class="item-info"><span>Local Port</span><span>' + item.localport +
                        '</span></div></div></div><div class="item-btn"><button data-id="' + item.linkid +
                        '" class="btn btn-sm btn-tcp">Disconnect</button></div></div></div>';
                });
                doc.append(html);
            }
        });
    },
    disconnectTcp: function(link_id) {
        var selt = this;
        $.ajax({
            type: "post",
            url: "/closeip",
            headers: {
                "Content-Type": "application/json",
            },
            data: "link_id=" + link_id,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    selt.getTcp();
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getBleClient: function () {
        var self = this;
        $.getJSON("/getbleset", function (resp) {
            if (resp.state == SUCCESS_CODE) {
                //$("#device-name").val(resp.devicename);
                self._setSelect("scanning-way", resp.scantype);
                self._setSelect("adr-type", resp.addrtype);
                self._setSelect("filter-policy", resp.filterpolicy);
                $("#scanning-interval").val(resp.scaninterval);
                $("#scanning-window").val(resp.scanwindow);
            }
        });
    },
    getBleServer: function () {
        var self = this;
        $.getJSON("/getbleadvset", function (resp) {
            if (resp.state == SUCCESS_CODE) {
                //$("#device-name").val(resp.devicename);
                $("#min-spacing").val(resp.advintmin);
                self._setSelect("adr-type", resp.addrtype);
                $("#max-spacing").val(resp.advintmax);
                self._setSelect("broadcast-type", resp.advtype);
                self._setSelect("broadcast-channel", resp.channel);
            }
        });
    },
    bleStart: function (obj) {
        var broadcastData = $("#broadcast-data").val();
        if (isEmpty(broadcastData)) {
            toastr.warning("Please enter the Broadcast Data!");
            return false;
        }
        $.ajax({
            type: "post",
            url: "/bleadvstart",
            headers: {
                "Content-Type": "application/json",
            },
            data: "advdata="+broadcastData,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    toastr.success("Success!");
                    $("#" + obj).addClass("hidden").siblings().removeClass("hidden");
                    sessionStorage.setItem(SAVE_STOP_ITEM, true);
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    bleStop: function (obj) {
        sessionStorage.setItem(SAVE_STOP_ITEM, false);
        $.ajax({
            type: "post",
            url: "/bleadvstop",
            headers: {
                "Content-Type": "application/json",
            },
            data: "",
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    toastr.success("Success!");

                    $("#" + obj).addClass("hidden").siblings().removeClass("hidden");
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    setBle: function() {
        var self = this;
        var modeType = self._getSelect("device-type"),
            //deviceName = $("#device-name").val(),
            scanWay = self._getSelect("scanning-way"),
            adrType = self._getSelect("adr-type"),
            filterPolicy = self._getSelect("filter-policy"),
            scanInterval = $("#scanning-interval").val(),
            scanWindow = $("#scanning-window").val(),
            maxInterval = $("#max-spacing").val(),
            minInterval = $("#min-spacing").val(),
            broadcastType = self._getSelect("broadcast-type"),
            broadcastChannel = self._getSelect("broadcast-channel"),
            data = "", url = "/bleset";
        if (modeType == 1) {
            data = "init_mode="+modeType+"&addrtype="+adrType+"&scantype="+scanWay+
                "&filterpolicy="+filterPolicy+"&scaninterval="+scanInterval+"&scanwindow="+scanWindow;
        } else if (modeType == 2) {
            url = "/bleadvset";
            data = "init_mode="+modeType+"&addrtype="+adrType+"&advintmin="+minInterval+
                "&advintmax="+maxInterval+"&advtype="+broadcastType+"&channel="+broadcastChannel;
        } else {
            data = "init_mode="+modeType;
        }
        console.log(data);
        $.ajax({
            type: "post",
            url: url,
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    toastr.success("Success!");
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    bleConnect: function () {
        var connId = $("#conn-id").val(),
            enable = $(".broadcast-btn.hidden").attr("data-value"),
            addr = $("#select-mac").val(), data = "";
        if (isEmpty(connId)) {
            toastr.warning("Please enter the Conn_ID!");
            return false;
        }
        if (isEmpty(addr)) {
            toastr.warning("Please choose the Mac!");
            return false;
        }
        data = "connindex=" + connId + "&remoteaddress=" + addr;
        $.ajax({
            type: "post",
            url: "/bleconnect",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/pages/ble/info.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    bleScan: function(enable, id) {
        $("#" + id).removeClass("hidden").siblings().addClass("hidden");
        $.ajax({
            type: "post",
            url: "/blescan",
            headers: {
                "Content-Type": "application/json",
            },
            data: "enable="+enable+"&interval=1",
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    var doc = $("#select-mac"),
                        docUl = doc.siblings("ul"),
                        docDesc = $("#adv-data");
                    doc.empty();
                    BROADCAST_LIST = resp.blelist;
                    var options = "";
                    $.each(BROADCAST_LIST, function(i, item) {
                        var addr = item.addr;
                        if (i == 0) {
                            docDesc.val(item.adv_data);
                            doc.val(addr);
                            options += "<li class='es-visible selected' value='" + addr + "'>" + addr + "</li>";
                        } else {
                            options += "<li class='es-visible'  value='" + addr + "'>" + addr + "</li>";
                        }
                    })
                    docUl.append(options);
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });

    },
    getBleconnect: function() {
        $.getJSON("/getbleconnect", function(resp){
            if (resp.state == SUCCESS_CODE) {
                var list = resp.connlist,
                    doc = $("#ble-connect-info"),
                    html = "";
                doc.empty();
                $.each(list, function (i, item) {
                    html += '<div class="col-md-4 col-sm-6 col-xs-12"> <div class="item">' +
                        '<div class="item-icon"><i class="icon-IP"></i></div>' +
                        '<div class="item-content"><div class="item-wrapper"> <div class="item-info">' +
                        '<span>Conn_ID</span><span>' + item.connid + '</span></div></div>' +
                        '<div class="item-wrapper"><div class="item-info"><span>Mac</span><span>' + item.addr +
                        '</span></div></div></div><div class="item-btn"><button data-id="' + item.connid +
                        '" class="btn btn-sm btn-ble">Disconnect</button></div></div></div>';
                });
                doc.append(html);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    closeBle: function(connid) {
        var data = "conn_index="+connid,
            selt = this;
        console.log(data);
        $.ajax({
            type: "post",
            url: "/blecloseconnect",
            headers: {
                "Content-Type": "application/json",
            },
            data: data,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    selt.getBleconnect();
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    getRamSize: function() {
        $.getJSON("/getramsize", function(resp){
            if (resp.state == SUCCESS_CODE) {
                $("#ram-size").text(resp.sysram);
            } else {
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        })
    },
    pingIp: function() {
        var ip = $("#ping-ip").val();
        if (isEmpty(ip)) {
            toastr.warning("Please enter Ip!");
            return false;
        }
        $.ajax({
            type: "post",
            url: "/ping",
            headers: {
                "Content-Type": "application/json",
            },
            data: "ping=" + ip,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    if (resp.ping == 65535) {
                        toastr.error("ping time out!");
                    } else {
                        toastr.warning("Success, time: " +resp.ping);
                    }
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    restartDevice: function() {
        $.ajax({
            type: "post",
            url: "/rst",
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){

                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    restoreDevice: function() {
        $.ajax({
            type: "post",
            url: "/restore",
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){

                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    changePwd: function() {
        var oldPwd = $("#old-password").val();
        var newPwd = $("#new-password").val();
        if (isEmpty(oldPwd)) {
            toastr.warning("Please enter the old password!");
            return false;
        }
        if (isEmpty(newPwd)) {
            toastr.warning("Please enter the new password!");
            return false;
        }
        $.ajax({
            type: "post",
            url: "/changeloginpwd",
            data: "oldpwd="+oldPwd+"&newpwd="+newPwd,
            dataType: "json",
            success: function(resp){
                if(resp.state == SUCCESS_CODE){
                    window.location = "/login.html";
                }else{
                    toastr.error(ERROR_INFO[resp.error_code]);
                }
            },
            error: function(resp){
                toastr.error(ERROR_INFO[resp.error_code]);
            }
        });
    },
    _initSelect: function(id, data, key, value, val, flag) {
        var doc = $("#" +id),
            html = "";
        doc.empty();
        $.each(data, function(i, item) {
            if (!isEmpty(val)) {
                if (item[key] == val) {
                    html += "<option selected value='" + item[key] + "'>" + item[value] + "</option>";
                } else {
                    html += "<option value='" + item[key] + "'>" + item[value] + "</option>";
                }
            } else {
                if (i == 0) {
                    val = item[key];
                    html += "<option selected value='" + item[key] + "'>" + item[value] + "</option>";
                } else {
                    html += "<option value='" + item[key] + "'>" + item[value] + "</option>";
                }
            }


        });
        doc.append(html);
        doc.editableSelect({
            filter: false
        });
        if (flag) {
            $("#" +id).attr("readonly", true);
        }
        this._setSelect(id, val);
    },
    _setSelect: function (id, val) {
        var doc = $("#" + id).siblings("ul").find("li[value='" + val + "']");
        doc.addClass("selected");
        $("#" + id).val(doc.text());
    },
    _getSelect: function (id) {
        var val = $("#" + id).siblings("ul").find("li.selected").attr("value");
        return val;
    },
    load: function () {
        var doc = $("body");
        console.log(doc);
        var html = "<div class='load-mask'><div class='mask'></div>" +
            "<i class='icon-loading mask-icon fa-pulse'></i>" +
            "<p>Scanning available WiFi...</p></div>";
        doc.append(html);
    },
    removeLoad: function () {
        $("body").find("div.load-mask").remove();
    }


}

function isEmpty(str) {
    if (str === "" || str === null || str === undefined) {
        return true;
    } else {
        return false;
    }
}