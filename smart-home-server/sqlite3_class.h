#pragma once
#include "stdafx.h"
extern "C"
{
#include "sqlite/sqlite3.h"
}

class Sqlite3Conn
{
	sqlite3* db_;

public:

	typedef int (*SqliteExecCallbackFunc)(void*, int, char**, char**);

	Sqlite3Conn(const char* filename);
	~Sqlite3Conn();
	int Exec(const char* sql, SqliteExecCallbackFunc sqlite_callback_func, void* data_pointer, char* zErrMsg);
	int ExecWithErrorThrow(const char* sql, SqliteExecCallbackFunc sqlite_callback_func,
							void* data_pointer, int fail_then_exit);

};

extern Sqlite3Conn g_sqlite3_conn;


