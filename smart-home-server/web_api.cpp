#include "web_api.h"
#include "httpd.h"
#include "mi_home_aircondition.h"
#include "door.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
using namespace rapidjson;

extern MiHomeAircondition* aircondition_pointer;
extern DoorUdpClient* door_pointer;

enum RequestDataDevice
{
    DOOR = 1, AIRCONDITION
};

enum RequestDataDoorAction
{
    REMOTE_OPEN_DOOR = 1, SET_DOOR_CONTROL_PARA, GET_DOOR_CONTROL_PARA
};

enum RequestDataMiHomeAirconditionAction
{
    GET_ALL_PROPERTIES = 1, GET_TEMPERATURE, GET_RUNNING_DURATION, SET_POWER, SET_MODE, SET_TARGET_TEMPERATURE,
    SET_ECO, SET_HEATER, SET_DRYER, SET_SLEEP_MODE, SET_FAN_SPEED, SET_VERTICAL_SWING, SET_BUZZER,
    SET_LED, SET_FAN_SPEED_PERCENT, SET_TIMER
};

enum ResponceDataCode
{
    SUCCESS = 0,
    UNAUTHORIZED_TOKEN = -10,
    UNAUTHORIZED_IP = -11,
    DEVICE_NOT_EXIST = -20,
    DEVICE_RESPONCE_FAILED = -21,
    ACTION_NOT_EXIST = -22,
};

struct RequestData
{
    int token;
    int device;
    int action;
    int value_1;
    int value_2;
};

/*  Parameters: request_data: reference of request data (in para)
                ip_addr: reference of client ip address string (in para)
                port: client port (in para)
                http_method: 1-GET  2-POST
                responce_data: reference of responce data string (out para),
    Return:     1:  200 OK
                -1: 400 BAD REQUEST */
int WebApiCalled(const RequestData& request_data, const string& ip_addr, int port, 
    int http_method, string& responce_data)
{
    /*
    code:   0:      ok
            -10:    unauthorized token
            -11:    unauthorized ip
            -20:    action not exist
            -21:    device responce failed
    result: result
    */
    StringBuffer str_buf;
    Writer<StringBuffer> writer(str_buf);
    int ret;
    DoorControlPara door_control_para;
    MiHomeAircondition::PropertiesValues aircondition_properties_values;
    int temp_int;
    float temp_float;
    
    writer.StartObject();
    writer.Key("code");
    switch (request_data.device)
    {
        case RequestDataDevice::DOOR:
            switch (request_data.action)
            {
                case RequestDataDoorAction::REMOTE_OPEN_DOOR:
                    ret = door_pointer->RemoteOpenDoor();
                    if(ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataDoorAction::SET_DOOR_CONTROL_PARA:
                    door_control_para.mode = (u_int8_t)request_data.value_1;
                    door_control_para.delay = (u_int8_t)request_data.value_2;
                    ret = door_pointer->SetDoorControlPara(&door_control_para);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataDoorAction::GET_DOOR_CONTROL_PARA:
                    door_pointer->GetDoorControlPara(&door_control_para);
                    writer.Int(ResponceDataCode::SUCCESS);
                    writer.Key("result");
                    writer.StartObject();
                    writer.Key("mode");     writer.Int(door_control_para.mode);
                    writer.Key("delay");     writer.Int(door_control_para.delay);
                    writer.EndObject();
                    break;
                default:
                    writer.Int(ResponceDataCode::ACTION_NOT_EXIST);
            }
            break;
        case RequestDataDevice::AIRCONDITION:
            switch (request_data.action)
            {
                case RequestDataMiHomeAirconditionAction::GET_ALL_PROPERTIES:
                    ret = aircondition_pointer->GetAllProperties
                        (aircondition_properties_values);
                    if (ret)
                    {
                        writer.Int(ResponceDataCode::SUCCESS);
                        writer.Key("result");
                        writer.StartObject();
                        writer.Key("power");
                        writer.Int(aircondition_properties_values.power);
                        writer.Key("mode");
                        writer.Int(aircondition_properties_values.mode);
                        writer.Key("target_temperature");
                        writer.Double(aircondition_properties_values.target_temperature);
                        writer.Key("eco");
                        writer.Int(aircondition_properties_values.eco);
                        writer.Key("heater");
                        writer.Int(aircondition_properties_values.heater);
                        writer.Key("dryer");
                        writer.Int(aircondition_properties_values.dryer);
                        writer.Key("sleep_mode");
                        writer.Int(aircondition_properties_values.sleep_mode);
                        writer.Key("fan_speed");
                        writer.Int(aircondition_properties_values.fan_speed);
                        writer.Key("vertical_swing");
                        writer.Int(aircondition_properties_values.vertical_swing);
                        writer.Key("temperature");
                        writer.Int(aircondition_properties_values.temperature);
                        writer.Key("buzzer");
                        writer.Int(aircondition_properties_values.buzzer);
                        writer.Key("led");
                        writer.Int(aircondition_properties_values.led);
                        writer.Key("running_duration");
                        writer.Double(aircondition_properties_values.running_duration);
                        writer.Key("fan_speed_percent");
                        writer.Int(aircondition_properties_values.fan_speed_percent);
                        writer.Key("timer_minutes_set");
                        writer.Int(aircondition_properties_values.timer_minutes_set);
                        writer.Key("timer_minutes_remain");
                        writer.Int(aircondition_properties_values.timer_minutes_remain);
                        writer.Key("timer_delay_on");
                        writer.Int(aircondition_properties_values.timer_delay_on);
                        writer.EndObject();
                    }
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::GET_TEMPERATURE:
                    ret = aircondition_pointer->GetTemperature(temp_int);
                    if (ret)
                    {
                        writer.Int(ResponceDataCode::SUCCESS);
                        writer.Key("result");       writer.Int(temp_int);
                    }
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::GET_RUNNING_DURATION:
                    ret = aircondition_pointer->GetRunningDuration(temp_float);
                    if (ret)
                    {
                        writer.Int(ResponceDataCode::SUCCESS);
                        writer.Key("result");       writer.Double(temp_float);
                    }
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_POWER:
                    ret = aircondition_pointer->SetPower(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_MODE:
                    ret = aircondition_pointer->SetMode
                        ((MiHomeAircondition::ModeValue)request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_TARGET_TEMPERATURE:
                    temp_float = (float)(request_data.value_1);
                    if (request_data.value_2)
                        temp_float = (float)(temp_float + 0.5);
                    ret = aircondition_pointer->SetTargetTemperature(temp_float);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_ECO:
                    ret = aircondition_pointer->SetEco(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_HEATER:
                    ret = aircondition_pointer->SetHeater(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_DRYER:
                    ret = aircondition_pointer->SetDryer(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_SLEEP_MODE:
                    ret = aircondition_pointer->SetSleepMode(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_FAN_SPEED:
                    ret = aircondition_pointer->SetFanSpeed
                    ((MiHomeAircondition::FanSpeedValue)request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_VERTICAL_SWING:
                    ret = aircondition_pointer->SetVerticalSwing(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_BUZZER:
                    ret = aircondition_pointer->SetBuzzer(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_LED:
                    ret = aircondition_pointer->SetLed(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_FAN_SPEED_PERCENT:
                    ret = aircondition_pointer->SetFanSpeedPercent(request_data.value_1);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                case RequestDataMiHomeAirconditionAction::SET_TIMER:
                    ret = aircondition_pointer->SetTimer(request_data.value_1, request_data.value_2);
                    if (ret)
                        writer.Int(ResponceDataCode::SUCCESS);
                    else
                        writer.Int(ResponceDataCode::DEVICE_RESPONCE_FAILED);
                    break;
                default:
                    writer.Int(ResponceDataCode::ACTION_NOT_EXIST);
            }
            break;
        default:
            writer.Int(ResponceDataCode::DEVICE_NOT_EXIST);
    }
    writer.EndObject();
    responce_data = str_buf.GetString();
    return 1;
}

/*  Parameters: point to http request string (the function will break this string)
                RequestData (out para)  */
void ParseQueryString(char* data_string, RequestData& data_struct)
{
    char* char_key = data_string;
    char* char_value;
    char* char_key_original;
    int value;
    memset(&data_struct, 0, sizeof(RequestData));
    while (1)
    {
        char_value = strchr(char_key, 0x3d);
        if (!char_value)
            break;
        *char_value = 0;
        char_value++;
        char_key_original = char_key;
        char_key = strchr(char_value, 0x26);
        if (char_key)
        {
            *char_key = 0;
            char_key++;
        }
        value = atoi(char_value);
        if (!strcmp(char_key_original, "token"))
            data_struct.token = value;
        else if (!strcmp(char_key_original, "device"))
            data_struct.device = value;
        else if (!strcmp(char_key_original, "action"))
            data_struct.action = value;
        else if (!strcmp(char_key_original, "value1"))
            data_struct.value_1 = value;
        else if (!strcmp(char_key_original, "value2"))
            data_struct.value_2 = value;
        if (!char_key)
            break;
    }
}

void WebApiResponce(int client, const char* responce_data, int len)
{
    send(client, "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n", 44, 0);
    send(client, responce_data, len, 0);
}

void WebApiRequest(int client, struct sockaddr_in& client_name, const char* method, 
    const char* query_string)
{
    int method_num = 0;
    char buf[1024];
    char* data_str;
    int numchars = 1;
    int content_length = -1;
    int data_len;
    RequestData data_struct;
    string client_ip;
    string responce_data;
    int ret;
    if (strcasecmp(method, "GET") == 0)
        method_num = 1;
    else if (strcasecmp(method, "POST") == 0)
        method_num = 2;
    else
    {
        bad_request(client);
        return;
    }
    buf[0] = 'A'; buf[1] = '\0';
    if (method_num == 1)
    {
        //GET
        while ((numchars > 0) && strcmp("\n", buf))//read & discard headers
            numchars = get_line(client, buf, sizeof(buf));
        data_len = (int)strlen(query_string);
        data_str = new char[data_len];
        memcpy(data_str, query_string, data_len);
    }
    else
    {
        //POST
        numchars = get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1) {
            bad_request(client);
            return;
        }
        data_len = content_length;
        data_str = new char[data_len];
        recv(client, data_str, data_len, 0);
    }
    ParseQueryString(data_str, data_struct);
    delete[] data_str;
    client_ip = inet_ntoa(client_name.sin_addr);
    ret = WebApiCalled(data_struct, client_ip, ntohs(client_name.sin_port), 
        method_num, responce_data);
    if (ret == 1)
    {
        WebApiResponce(client, responce_data.c_str(), (int)responce_data.length());
    }
    else if (ret == -1)
    {
        bad_request(client);
    }
}




