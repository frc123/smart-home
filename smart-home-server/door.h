#pragma once
#include "socket_class.h"
#include "stdafx.h"

struct DoorControlPara
{
	u_int8_t mode;
	u_int8_t delay;
};

class DoorUdpClient
{
	UdpClient udp_client_;
	int OpenUdpClient(struct sockaddr_in* addr_serv);
public:
	friend void OpenUdpClientFromUdpServer(struct sockaddr_in* addr_serv);
	int RemoteOpenDoor();
	int SetDoorControlPara(DoorControlPara* door_control_para);
	void GetDoorControlPara(DoorControlPara* door_control_para);
};

class DoorUdpServer
{
	static void SqlQueryControllerStatusRecordInsert(char* recv_buf);
	static void HeartbeatOnReceive(char* recv_buf);
	static int UdpServerOnReceive(int recv_num, struct sockaddr_in* addr_client, char* recv_buf, void* data);
	static void Init();
	static void UdpServerStart();
public:
	static int UdpServerInitAndStartInThread(DoorUdpClient& door_udp_client_para);
};



