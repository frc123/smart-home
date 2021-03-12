#pragma once
#include "stdafx.h"

#define SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_INDEX 101
#define SQL_CONF_KEY_DOOR_QUERY_CONTROLLER_STATUS_RECORD_ID 102

void SqlInitCreatTables();

class SqlConfRecord
{
    struct GetConfValResult
    {
        int conf_exist;//should be init to 0
        int conf_val;
    };

    //TODO:回调函数是否必须public
    static int SqlCallbackGetConfValStruct(void* data_pointer, int columen_count, char** column_value, char** column_name);
    static void GetConfValStruct(int conf_key, GetConfValResult* result);
    static void InsertConf(int conf_key, int conf_val);
public:
    static int InsertConfIfNotExist(int conf_key, int conf_val);
    static void UpdateConfVal(int conf_key, int conf_val);
};
