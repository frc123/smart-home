#include "sqlite3_class.h"
#include "log_process.h"

///mnt/hgfs/centos7-vs-test/smart_home.db
Sqlite3Conn g_sqlite3_conn("smart_home.db");

Sqlite3Conn::Sqlite3Conn(const char* filename)
{
	int rc;
	rc = sqlite3_open(filename, &db_);
	if (rc)
	{
		ErrorThrow(611, "sqlite3 open fail", sqlite3_errmsg(db_));
		ExitProcess(EXIT_FAILURE);
	}

}

Sqlite3Conn::~Sqlite3Conn()
{
	sqlite3_close(db_);
}

int Sqlite3Conn::Exec(const char* sql, SqliteExecCallbackFunc sqlite_callback_func, void * data_pointer, char* zErrMsg)
{
	int rc;
	rc = sqlite3_exec(db_, sql, sqlite_callback_func, data_pointer, &zErrMsg);
	return (rc == SQLITE_OK);
}

int Sqlite3Conn::ExecWithErrorThrow(const char* sql, SqliteExecCallbackFunc sqlite_callback_func,
									void* data_pointer, int fail_then_exit)
{
	char* zErrMsg = 0;
	int ret = Exec(sql, sqlite_callback_func, data_pointer, zErrMsg);
	if (ret == 0)
	{
		ErrorThrow(612, "sqlite3 exec error", zErrMsg);
		sqlite3_free(zErrMsg);
		if (fail_then_exit)
		{
			ExitProcess(EXIT_FAILURE);
		}
	}
	return ret;
}


