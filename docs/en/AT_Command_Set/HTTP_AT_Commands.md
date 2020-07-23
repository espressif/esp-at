<a name="HTTP-AT"></a>
## HTTP AT Commands 
- [AT+HTTPCLIENT](#cmd-HTTPCLIENT) -  Send HTTP Client Request
- [AT+HTTPGETSIZE](#cmd-HTTPGETSIZE) - Get HTTP Resource Size
- [HTTP AT Error Code](#cmd-HTTPErrCode)
 
<a name="cmd-HTTPCLIENT"></a>
### [AT+HTTPCLIENT](#HTTP-AT)-Send HTTP Client Request
Set Command:  

    AT+HTTPCLIENT=<opt>,<content-type>,[<url>],[<host>],[<path>],<transport_type>,[<data>][,"http_req_header"][,"http_req_header"][...]
Response:

    +HTTPCLIENT:<size>,<data>

    OK
Parameters:

- **\<opt>** : method of HTTP client request
  - 1 :  HEAD     
  - 2 :  GET       
  - 3 :  POST   
  - 4 :  PUT       
  - 5 :  DELETE 
- **\<content-type>** : data type of HTTP client request
  - 0 : `application/x-www-form-urlencoded`
  - 1 : `application/json`
  - 2 : `multipart/form-data`
  - 3 : `text/xml`
- **\<url>** : optional parameter, HTTP URL, The url field can override the host and path parameters if they are null.
- **\<host>**: optional parameter, domain name or IP address
- **\<path>**: optional parameter, HTTP Path 
- **\<transport_type>**： HTTP Client transport type, default type is 1.
  - 1 : `HTTP_TRANSPORT_OVER_TCP`
  - 2 : `HTTP_TRANSPORT_OVER_SSL` 
- **\<data>**: optional parameter. When it is a POST request, `<data>` is the user data sent to HTTP server.
- **\<http_req_header>**: optional parameter. The number of request headers can be customized by the user.

**Note:**

- If `<url>` is omitted, `<host>` and `<path>` must be set.

**Example:**

    //HEAD Request
    AT+HTTPCLIENT=1,0,"http://httpbin.org/get","httpbin.org","/get",1
    AT+HTTPCLIENT=1,0,"http://httpbin.org/get",,,0
    AT+HTTPCLIENT=1,0,,"httpbin.org","/get",1
    //GET Request
    AT+HTTPCLIENT=2,0,"http://httpbin.org/get","httpbin.org","/get",1
    AT+HTTPCLIENT=2,0,"http://httpbin.org/get",,,0
    AT+HTTPCLIENT=2,0,,"httpbin.org","/get",1
    //POST Request
    AT+HTTPCLIENT=3,0,"http://httpbin.org/post","httpbin.org","/post",1,"field1=value1&field2=value2"
    AT+HTTPCLIENT=3,0,"http://httpbin.org/post",,,0,"field1=value1&field2=value
    //HTTP offset continue download
    HTTPCLIENT=2,0,"http://www.baidu.com/img/bdlogo.gif",,,0,"Range: bytes=100-200"

<a name="cmd-HTTPGETSIZE"></a>
### [AT+HTTPGETSIZE](#HTTP-AT)-Get HTTP Resource Size
Set Command:

    AT+HTTPGETSIZE=<url>
Response:

    +HTTPGETSIZE:size

    OK
Parameters:
    - **\<url>** : HTTP URL.

**Example:**

    AT+HTTPGETSIZE="http://www.baidu.com/img/bdlogo.gif"

<a name="cmd-HTTPErrCode"></a>
### [HTTP Error Code](#HTTP-AT)

- HTTP Client:

| HTTP Client Error Code |           Description           |
| :--------------------: | :-----------------------------: |
|         0x7190         |           Bad Request           |
|         0x7191         |          Unauthorized           |
|         0x7192         |        Payment Required         |
|         0x7193         |            Forbidden            |
|         0x7194         |            Not Found            |
|         0x7195         |       Method Not Allowed        |
|         0x7196         |         Not Acceptable          |
|         0x7197         |  Proxy Authentication Required  |
|         0x7198         |         Request Timeout         |
|         0x7199         |            Conflict             |
|         0x719a         |              Gone               |
|         0x719b         |         Length Required         |
|         0x719c         |       Precondition Failed       |
|         0x719d         |    Request Entity Too Large     |
|         0x719e         |      Request-URI Too Long       |
|         0x719f         |     Unsupported Media Type      |
|         0x71a0         | Requested Range Not Satisfiable |
|         0x71a1         |        Eectation Failed         |

- HTTP Server:

| HTTP Server Error Code |        Description         |
| :--------------------: | :------------------------: |
|         0x71f4         |   Internal Server Error    |
|         0x71f5         |      Not Implemented       |
|         0x71f6         |        Bad Gateway         |
|         0x71f7         |    Service Unavailable     |
|         0x71f8         |      Gateway Timeout       |
|         0x71f9         | HTTP Version Not Supported |

- HTTP AT:  
  The error code of command `AT+HTTPCLIENT` will be `0x7000+Standard HTTP Error Code`.   
  For example, if it gets the HTTP error 404 when calling command `AT+HTTPCLIENT`, then the AT will respond error code as `0x7194`, `hex(0x7000+404)=0x7194`.

More details of Standard HTTP/1.1 Error Code are in RFC 2616: https://tools.ietf.org/html/rfc2616
