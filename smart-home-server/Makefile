rapidjson_header = rapidjson/document.h rapidjson/reader.h rapidjson/allocators.h \
 rapidjson/rapidjson.h rapidjson/stream.h rapidjson/encodings.h \
 rapidjson/encodedstream.h rapidjson/memorystream.h \
 rapidjson/internal/clzll.h rapidjson/internal/../rapidjson.h \
 rapidjson/internal/meta.h rapidjson/internal/stack.h \
 rapidjson/internal/../allocators.h rapidjson/internal/swap.h \
 rapidjson/internal/strtod.h rapidjson/internal/ieee754.h \
 rapidjson/internal/biginteger.h rapidjson/internal/diyfp.h \
 rapidjson/internal/clzll.h rapidjson/internal/pow10.h \
 rapidjson/error/error.h rapidjson/error/../rapidjson.h \
 rapidjson/internal/strfunc.h rapidjson/internal/../stream.h \
 rapidjson/internal/../rapidjson.h rapidjson/prettywriter.h \
 rapidjson/writer.h rapidjson/internal/dtoa.h rapidjson/internal/itoa.h \
 rapidjson/internal/itoa.h rapidjson/stringbuffer.h
EXE = smart-home-server
CFLAGS = -Wall -g
C++FLAGS = -std=c++11 -Wall -g
ifeq ($(type),openwrt)
	CC = mips-openwrt-linux-gcc
	CC++ = mips-openwrt-linux-g++
	sqlite3_obj = sqlite3_openwrt.o
	CFLAGS += -DBIGENDIAN
	C++FLAGS += -DBIGENDIAN
else
	CC = gcc
	CC++ = g++
	sqlite3_obj = sqlite3.o
endif
objects_without_sqlite3_obj = main.o web_api.o mi_home_aircondition.o mi_home_device.o door.o socket_class.o \
 sql_encap.o sqlite3_class.o httpd.o log_process.o md5.o aes.o type_tools.o
objects = $(objects_without_sqlite3_obj) $(sqlite3_obj)
# -Werror
# -L/mnt/sda5/packages/usr/lib/ -lsqlite3 -pthread
$(EXE): $(objects)
	$(CC++) -o $(EXE) $(objects) -pthread -ldl

$(sqlite3_obj): sqlite/sqlite3.c
	$(CC) -c $(CFLAGS) $< -o $@
#shell.o: sqlite/shell.c sqlite/sqlite3.h
#	$(CC) -c $(CFLAGS) $< -o $@
aes.o: aes.c aes.h
	$(CC) -c $(CFLAGS) $< -o $@
door.o: door.cpp door.h socket_class.h stdafx.h type_tools.h sql_encap.h \
 sqlite3_class.h sqlite/sqlite3.h
	$(CC++) -c $(C++FLAGS) $< -o $@
httpd.o: httpd.cpp httpd.h stdafx.h log_process.h
	$(CC++) -c $(C++FLAGS) $< -o $@
log_process.o: log_process.cpp log_process.h stdafx.h
	$(CC++) -c $(C++FLAGS) $< -o $@
main.o: main.cpp stdafx.h log_process.h door.h socket_class.h \
 mi_home_aircondition.h mi_home_device.h md5.h aes.hpp aes.h \
 $(rapidjson_header) httpd.h sql_encap.h
	$(CC++) -c $(C++FLAGS) $< -o $@
md5.o: md5.cpp md5.h
	$(CC++) -c $(C++FLAGS) $< -o $@
mi_home_aircondition.o: mi_home_aircondition.cpp mi_home_aircondition.h \
 stdafx.h mi_home_device.h socket_class.h md5.h aes.hpp aes.h \
 $(rapidjson_header)
	$(CC++) -c $(C++FLAGS) $< -o $@
mi_home_device.o: mi_home_device.cpp mi_home_device.h stdafx.h \
 socket_class.h md5.h aes.hpp aes.h $(rapidjson_header) type_tools.h
	$(CC++) -c $(C++FLAGS) $< -o $@
socket_class.o: socket_class.cpp socket_class.h stdafx.h log_process.h
	$(CC++) -c $(C++FLAGS) $< -o $@
sql_encap.o: sql_encap.cpp sql_encap.h stdafx.h sqlite3_class.h \
 sqlite/sqlite3.h
	$(CC++) -c $(C++FLAGS) $< -o $@
sqlite3_class.o: sqlite3_class.cpp sqlite3_class.h stdafx.h \
 sqlite/sqlite3.h log_process.h
	$(CC++) -c $(C++FLAGS) $< -o $@
type_tools.o: type_tools.cpp type_tools.h stdafx.h
	$(CC++) -c $(C++FLAGS) $< -o $@
web_api.o: web_api.cpp web_api.h stdafx.h httpd.h mi_home_aircondition.h \
 mi_home_device.h socket_class.h md5.h aes.hpp aes.h $(rapidjson_header) door.h
	$(CC++) -c $(C++FLAGS) $< -o $@

.PHONY : cleanobj clean
cleanobj :
	-rm $(objects_without_sqlite3_obj)
clean :
	-rm $(EXE) $(objects_without_sqlite3_obj)
cleanall :
	-rm $(EXE) $(objects)