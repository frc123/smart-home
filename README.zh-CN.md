# Smart Home

[English Version 英文版](https://github.com/frc123/smart-home/blob/master/README.md)

***

这是我的智能家具解决方案。

服务器部分会部署在OpenWrt固件的路由器上。

该项目还没有完成。

欢迎进行贡献。

文档正在更新中。

您可以根据您的家具进行二次开发。

## Demo

![front-end-door-img](https://github.com/frc123/smart-home/blob/master/demo-img/door.png)
![front-end-aircondition-img](https://github.com/frc123/smart-home/blob/master/demo-img/air-condition.png)

## 已经实现的家具

- 空调（Air Condition）
	- 小米互联网空调X（1.5匹|变频|新一级能效）
	- type: urn:miot-spec-v2:device:air-conditioner:0000A004:xiaomi-mt2:1
	- 通过miio协议实现
- 门禁（Door）
	- 控制器: 微耕门禁控制器（WG2051）
	- 通过微耕提供的二次开发工具实现

## 开始

您需要一个服务器来部署Smart Home

以下是我的部署方案

我将服务器部署在了家用路由器上

- 型号: 美国网件 NETGEAR WNDR4300
- 固件版本: OpenWrt 19.07.5 r11257-5090152ae3

### 设置参数

顺便说下，直接把参数设置在源代码中不太好。我将会在之后进行优化。

1. 前往 smart-home-server/main.cpp
```
//TODO: Put your air condition ip address here
#define AIRCONDITION_IP_ADDRESS  ""
//TODO: Put your air condition token here
#define AIRCONDITION_TOKEN ""
```

2. 前往 smart-home-server/door.cpp
```
//TODO: Put your door port here
#define DOOR_UDP_SERVER_PORT   0
//TODO: Put your door ip address here
#define DOOR_IP_ADDRESS  ""
```

3. 前往 front-end/smart-home-api.php
```
//TODO: put your server api url here
$address = 'http://127.0.0.1:12345/smart-home-api';
```

### 部署服务器

1. 前往OpenWrt获取交叉编译等工具

[openwrt developer guide](https://openwrt.org/docs/guide-developer/start)

2. 将后端部分进行编译

你也许需要修改makefile文件
```
make type=openwrt
```

3. 在服务器上保留12345端口

后端部分的http服务器将会监听这个端口来支持api

4. 运行
```
smart-home-server
```
也许你可以使用screen来进行后台运行

5. 将前端部署在http服务器上，例如 nginx/apache 等.

### 安全

通过路由器防火墙策略来实现。

计划将会在下个版本中完成账户系统。

## 代码结构

（待完善）

- front-end
	前端
- smart-home-server
	后端

## 二次开发

对于更多米家设备，您可以在 smart-home-server/mi-home-device 的基础上进行开发

[Mi Home Device Protocal Documentaion](https://iot.mi.com/new/doc/design/spec/overall) 将会提供帮助。

我将会在之后给米家c++协议实现（及说明）发布一个新的仓库

## 使用的库

- [Tencent/rapidjson](https://www.runoob.com)
- [EZLippi/Tinyhttpd](https://github.com/EZLippi/Tinyhttpd)
- [JieweiWei/md5](https://github.com/JieweiWei/md5)
- [kokke/tiny-AES-c](https://github.com/kokke/tiny-AES-c)
- [Sqlite3](https://www.sqlite.org/)
- [amazeUI](https://amazeui.shopxo.net/)
- [jquery](https://jquery.com/)

## 声明

我们不承担任何责任

## 联系方式

Blog: https://frc6.com

Email: i@frc6.com

QQ: 2093003592

