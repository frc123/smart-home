# Smart Home

This is my smart home solution. 

The server is deployed on openwrt router.

The project is not completed yet. 

Welcome to contribute it. 

Document is updating. 

You may carry out secondary development to apply to your furniture.

## Realized furniture

- Air Condition
	- name: 小米互联网空调X（1.5匹|变频|新一级能效）
	- type: urn:miot-spec-v2:device:air-conditioner:0000A004:xiaomi-mt2:1
	- implement by miio protocol
- Door 
	- access controller: 微耕门禁控制器（WG2051）
	- implement by wiegand SDK

## Start

You need a server to depoly the Smart Home

This is my depoly example

I depoly the smart home server on my router:

- model: NETGEAR WNDR4300
- firmware version: OpenWrt 19.07.5 r11257-5090152ae3

### Set Parameter

1. Go to smart-home-server/main.cpp
```
//TODO: Put your air condition ip address here
#define AIRCONDITION_IP_ADDRESS  ""
//TODO: Put your air condition token here
#define AIRCONDITION_TOKEN ""
```

2. Go to smart-home-server/door.cpp
```
//TODO: Put your door port here
#define DOOR_UDP_SERVER_PORT   0
//TODO: Put your door ip address here
#define DOOR_IP_ADDRESS  ""
```

3. Go to front-end/smart-home-api.php
```
//TODO: put your server api url here
$address = 'http://127.0.0.1:12345/smart-home-api';
```

### Depoly Server

1. Get the cross compiler in openwrt developer tools

https://openwrt.org/

2. Compile back up part

You maybe want to modify makefile
```
make type=openwrt
```

3. Reserve port 12345 from your server

The http server of smart-home-server will listen the port for api

4. Run
```
smart-home-server
```
Maybe you want to use linux screen

5. Set the front-end into a http server such as nginx/apache/etc.

### Security

Security policy relies on firewall of the router

## Code Structure

- fronr-end
	Front End
- smart-home-server
	Back End

## Library

- [Tencent/rapidjson](https://www.runoob.com)
- [JieweiWei/md5](https://github.com/JieweiWei/md5)
- [kokke/tiny-AES-c](https://github.com/kokke/tiny-AES-c)
- [Sqlite3](https://www.sqlite.org/)
- [amazeUI](https://amazeui.shopxo.net/)

## Statement
We do not assume any responsibility
