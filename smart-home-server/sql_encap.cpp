#include "sql_encap.h"
#include "sqlite3_class.h"

/*
int SqlCallback(void* data_pointer, int columen_count, char** column_value, char** column_name)
{
    int i;
    for (i = 0; i < columenCount; i++) {
        printf("%s = %s\n", columnName[i], columnValue[i] ? columnValue[i] : "NULL");
    }
    printf("\n");
    return 0;
}
*/

void SqlInitCreatTables()
{
    const char* sql;
    sql = "CREATE TABLE IF NOT EXISTS door_query_controller_status_record \
        (id integer primary key, controller_serial_num integer, record_index integer, \
        record_type integer, valid integer, door_num integer, in_out integer, \
        card_num integer, event_time integer, reason integer, sensor_1 integer, \
        sensor_2 integer, sensor_3 integer, sensor_4 integer, btn_1 integer, \
        btn_2 integer, btn_3 integer, btn_4 integer, error_num integer, \
        controller_time integer, pkg_serial_num integer, special_info_1 integer, \
        relay_status integer);";
    g_sqlite3_conn.ExecWithErrorThrow(sql, NULL, 0, 1);
    sql = "CREATE TABLE IF NOT EXISTS config \
        (conf_key integer, conf_val integer);";
    g_sqlite3_conn.ExecWithErrorThrow(sql, NULL, 0, 1);
}

//------------------------------------------------------------------------------------------------

int SqlConfRecord::SqlCallbackGetConfValStruct(void* data_pointer, int columen_count, char** column_value, char** column_name)
{
    GetConfValResult* result = (GetConfValResult*)data_pointer;
    result->conf_exist = 1;
    result->conf_val = atoi(column_value[0]);
    return 0;
}

void SqlConfRecord::GetConfValStruct(int conf_key, GetConfValResult* result)
{
    char sql[80];
    sprintf(sql, "SELECT conf_val FROM config WHERE conf_key == %i;", conf_key);
    g_sqlite3_conn.ExecWithErrorThrow(sql, SqlCallbackGetConfValStruct, result, 0);
}

void SqlConfRecord::InsertConf(int conf_key, int conf_val)
{
    char sql[80];
    sprintf(sql, "INSERT INTO config (conf_key,conf_val) VALUES (%i,%i);", conf_key, conf_val);
    g_sqlite3_conn.ExecWithErrorThrow(sql, NULL, NULL, 0);
}

//if config record does not exit, insert config with conf_val = conf_val_init
//return conf_val in db
int SqlConfRecord::InsertConfIfNotExist(int conf_key, int conf_val_init)
{
    GetConfValResult result;
    memset(&result, 0, sizeof(result));
    GetConfValStruct(conf_key, &result);
    if (result.conf_exist)
    {
        return result.conf_val;
    }
    else
    {
        InsertConf(conf_key, conf_val_init);
        return conf_val_init;
    }
}

//this function does NOT check if config record exists
void SqlConfRecord::UpdateConfVal(int conf_key, int conf_val)
{
    char sql[80];
    sprintf(sql, "UPDATE config set conf_val = %i WHERE conf_key == %i;", conf_val, conf_key);
    g_sqlite3_conn.ExecWithErrorThrow(sql, NULL, NULL, 0);
}


