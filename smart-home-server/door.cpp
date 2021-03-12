#include "door.h"
#include "type_tools.h"
#include "sql_encap.h"
#include "sqlite3_class.h"

//TODO: Put your door port here
#define DOOR_UDP_SERVER_PORT   0
//TODO: Put your door ip address here
#define DOOR_IP_ADDRESS  ""

//UdpServer
in_addr_t door_ip_addr = inet_addr(DOOR_IP_ADDRESS);
int door_query_controller_status_record_index;
int door_query_controller_status_record_id;
//UdpClient
DoorUdpClient* door_udp_client;
int door_udp_client_opened = 0;
//mutex
std::mutex mtx_block_thread_starter_until_client_opened;

void OpenUdpClientFromUdpServer(struct sockaddr_in* addr_serv)
{
    door_udp_client_opened = door_udp_client->OpenUdpClient(addr_serv);
    mtx_block_thread_starter_until_client_opened.unlock();
}

//------------------------------------------------------------------------------------------------

void DoorUdpServer::Init()
{
    door_query_controller_status_record_index =
        SqlConfRecord::InsertConfIfNotExist(SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_INDEX, 0);
    door_query_controller_status_record_id =
        SqlConfRecord::InsertConfIfNotExist(SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_ID, 0);
}

void DoorUdpServer::SqlQueryControllerStatusRecordInsert(char* recv_buf)
{
    door_query_controller_status_record_id++;
    SqlConfRecord::UpdateConfVal(SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_ID, 
                                door_query_controller_status_record_id);
    SqlConfRecord::UpdateConfVal(SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_INDEX, 
                                door_query_controller_status_record_index);
    int controller_serial_num;
    Char2Int32LittleEndian(recv_buf, 4, &controller_serial_num);
    int card_num;
    Char2Int32LittleEndian(recv_buf, 16,&card_num);
    int event_time = Bcd2Stamp(recv_buf[20], recv_buf[21], recv_buf[22], recv_buf[23], recv_buf[24], recv_buf[25],
                            recv_buf[26]);
    int controller_time = Bcd2Stamp(0x20, recv_buf[51], recv_buf[52], recv_buf[53], recv_buf[37], recv_buf[38],
                            recv_buf[39]);
    int pkg_serial_num;
    Char2Int32LittleEndian(recv_buf, 40,&pkg_serial_num);
    char* sql = new char[500];
    //char sql[500];
    sprintf(sql, "INSERT INTO door_query_controller_status_record \
            (id,controller_serial_num,record_index,record_type,valid,door_num,in_out,card_num,event_time,\
            reason,sensor_1,sensor_2,sensor_3,sensor_4,btn_1,btn_2,btn_3,btn_4,error_num,controller_time,\
            pkg_serial_num,special_info_1,relay_status) VALUES \
            (%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i);", 
            door_query_controller_status_record_id, controller_serial_num, door_query_controller_status_record_index,
            (int)recv_buf[12], (int)recv_buf[13], (int)recv_buf[14], (int)recv_buf[15], card_num, event_time, 
            (int)recv_buf[27], (int)recv_buf[28], (int)recv_buf[29], (int)recv_buf[30], (int)recv_buf[31], 
            (int)recv_buf[32], (int)recv_buf[33], (int)recv_buf[34], (int)recv_buf[35], (int)recv_buf[36], 
            controller_time, pkg_serial_num, (int)recv_buf[48], (int)recv_buf[49]);
    g_sqlite3_conn.ExecWithErrorThrow(sql, NULL, NULL, 0);
    delete[] sql;
}

void DoorUdpServer::HeartbeatOnReceive(char* recv_buf)
{
    int record_index;
    Char2Int32LittleEndian(recv_buf, 8, &record_index);
    if (door_query_controller_status_record_index != record_index)
    {
        door_query_controller_status_record_index = record_index;
        SqlQueryControllerStatusRecordInsert(recv_buf);
    }
}

int DoorUdpServer::UdpServerOnReceive(int recv_num, struct sockaddr_in* addr_client, char* recv_buf, void* data)
{
//    printf("recieve from unknown: %s:%lu\n", inet_ntoa(addr_client->sin_addr), ntohs(addr_client->sin_port));
    if (addr_client->sin_addr.s_addr == door_ip_addr)//ip address check
    {
        //from door
        if (door_udp_client_opened == 0)
        {
            OpenUdpClientFromUdpServer(addr_client);
        }
        if (recv_buf[0] == 0x17)//magic number check
        {
            if (recv_buf[1] == 0x20)//heartbeat
            {
                HeartbeatOnReceive(recv_buf);
            }
        }
    }
    return 1;
}

void DoorUdpServer::UdpServerStart()
{
    char door_udp_server_recv_buf[64];
    UdpServer::OnReceiveCallbackFunc door_udp_server_callback_func = UdpServerOnReceive;
    UdpServer door_udp_server;
    door_udp_server.OpenUdpServer(DOOR_UDP_SERVER_PORT, door_udp_server_recv_buf, sizeof(door_udp_server_recv_buf), 
                                door_udp_server_callback_func, NULL);
}

int DoorUdpServer::UdpServerInitAndStartInThread(DoorUdpClient& door_udp_client_para)
{
    mtx_block_thread_starter_until_client_opened.lock();
    door_udp_client = &door_udp_client_para;
    Init();
    std::thread door_udp_server_thread(UdpServerStart);
    door_udp_server_thread.detach();
    mtx_block_thread_starter_until_client_opened.lock();
    mtx_block_thread_starter_until_client_opened.unlock();
    return door_udp_client_opened;
}
//------------------------------------------------------------------------------------------------

int DoorUdpClient::OpenUdpClient(struct sockaddr_in* addr_serv)
{
    return udp_client_.OpenUdpClient(addr_serv);
}

int DoorUdpClient::RemoteOpenDoor()
{
    unsigned char send_buf[64] = { 0x17, 0x40, 0, 0, 0x67, 0xc8, 0x73, 0x07, 0x01 };
    char recv_buf[64];
    udp_client_.SendSync((char*)send_buf, 64, recv_buf, 64);
    return (int)recv_buf[8];
}

int DoorUdpClient::SetDoorControlPara(DoorControlPara* door_control_para)
{
    unsigned char send_buf[64] = { 0x17, 0x80, 0, 0, 0x67, 0xc8, 0x73, 0x07, 0x01 };
    send_buf[9] = door_control_para->mode;
    send_buf[10] = door_control_para->delay;
    char recv_buf[64];
    udp_client_.SendSync((char*)send_buf, 64, recv_buf, 64);
    if (recv_buf[8])
    {
        return 1;
    }
    return 0;
}

void DoorUdpClient::GetDoorControlPara(DoorControlPara* door_control_para)
{
    unsigned char send_buf[64] = { 0x17, 0x82, 0, 0, 0x67, 0xc8, 0x73, 0x07, 0x01 };
    char recv_buf[64];
    udp_client_.SendSync((char*)send_buf, 64, recv_buf, 64);
    door_control_para->mode = recv_buf[9];
    door_control_para->delay = recv_buf[10];
}
