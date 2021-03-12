#include "stdafx.h"
#include "log_process.h"
#include "door.h"
#include "mi_home_aircondition.h"
#include "httpd.h"
#include "sql_encap.h"

//TODO: Put your air condition ip address here
#define AIRCONDITION_IP_ADDRESS  ""
//TODO: Put your air condition token here
#define AIRCONDITION_TOKEN ""

MiHomeAircondition* aircondition_pointer;
DoorUdpClient* door_pointer;

extern int* httpd_socket_fd_pointer;

void OnExit()
{
    if (*httpd_socket_fd_pointer != -1)
        close(*httpd_socket_fd_pointer);
}

int main()
{
    int ret;
    atexit(OnExit);
    //sql init
    SqlInitCreatTables();
    //aircondition init
    MiHomeAircondition aircondition;
    aircondition_pointer = &aircondition;
    ret = aircondition.Init(AIRCONDITION_IP_ADDRESS, AIRCONDITION_TOKEN);
    printf("aircondition init ret:%i\n", ret);
    //start door udp server to listen heartbeat
    DoorUdpClient door_udp_client;
    door_pointer = &door_udp_client;
    ret = DoorUdpServer::UdpServerInitAndStartInThread(door_udp_client);
    printf("door init ret:%i\n", ret);
    //exec
    openhttpd(12345);
    ExitProcess(EXIT_SUCCESS);
    return 0;

}


