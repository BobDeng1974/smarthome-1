
SBU_REV= "0.1"

INCLUDE = -I$(PROJ_DIR)../ -I$(PROJ_DIR)../framework/platform/gnu -I$(PROJ_DIR)../framework/rpc/ -I$(PROJ_DIR)../framework/mt/ -I$(PROJ_DIR)../framework/mt/Af -I$(PROJ_DIR)../framework/mt/Zdo -I$(PROJ_DIR)../framework/mt/Sys -I$(PROJ_DIR)../framework/mt/Sapi -I$(PROJ_DIR)../znp/zcl -I$(PROJ_DIR)../znp -I$(PROJ_DIR)../sqlite/include -L$(PROJ_DIR)../sqlite/lib

CC= gcc
#CC=arm-arago-linux-gnueabi-gcc

CFLAGS= -c -Wall -g -std=gnu99
LIBS = -lpthread -lrt -lm -ldl
DEFS += -DxCC26xx
PROJ_DIR=

all: gateway.bin

gateway.bin: main.o rpc.o mtParser.o mtZdo.o mtSys.o mtAf.o mtSapi.o dbgPrint.o rpcTransport.o queue.o appinit.o commands.o zcl.o zcl_ss.o ceconf.o cetimer.o connection.o event.o eventhub.o gateway.o kfifo.o protocol.o rbtree.o reconn.o socket.o toolkit.o parseserver.o znp.o znp_device.o cJSON.o znp_map.o sqlitedb.o sqlite3.o
	$(CC) main.o appinit.o rpc.o mtParser.o mtZdo.o mtSys.o mtAf.o mtSapi.o dbgPrint.o rpcTransport.o queue.o commands.o zcl.o zcl_ss.o ceconf.o cetimer.o connection.o event.o eventhub.o gateway.o kfifo.o protocol.o rbtree.o reconn.o socket.o toolkit.o parseserver.o znp.o znp_device.o cJSON.o znp_map.o  sqlitedb.o sqlite3.o $(LIBS) -o gateway.bin

# rule for file "main.o".
main.o: ../main.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../main.c

# rule for file "rpc.o".
rpc.o: $(PROJ_DIR)../framework/rpc/rpc.h $(PROJ_DIR)../framework/rpc/rpc.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/rpc/rpc.c

# rule for file "mtParser.o".
mtParser.o: $(PROJ_DIR)../framework/mt/mtParser.h $(PROJ_DIR)../framework/mt/mtParser.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/mt/mtParser.c

# rule for file "mtZdo.o".
mtZdo.o: $(PROJ_DIR)../framework/mt/Zdo/mtZdo.h $(PROJ_DIR)../framework/mt/Zdo/mtZdo.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/mt/Zdo/mtZdo.c

# rule for file "mtSys.o".
mtSys.o: $(PROJ_DIR)../framework/mt/Sys/mtSys.h $(PROJ_DIR)../framework/mt/Sys/mtSys.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/mt/Sys/mtSys.c

# rule for file "mtAf.o".
mtAf.o: $(PROJ_DIR)../framework/mt/Af/mtAf.h $(PROJ_DIR)../framework/mt/Af/mtAf.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/mt/Af/mtAf.c

# rule for file "mtSapi.o".
mtSapi.o: $(PROJ_DIR)../framework/mt/Sapi/mtSapi.h $(PROJ_DIR)../framework/mt/Sapi/mtSapi.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/mt/Sapi/mtSapi.c

# rule for file "dbgPrint.o".
dbgPrint.o: $(PROJ_DIR)../framework/platform/gnu/dbgPrint.h $(PROJ_DIR)../framework/platform/gnu/dbgPrint.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/platform/gnu/dbgPrint.c

# rule for file "rpcTransport.o".
rpcTransport.o: $(PROJ_DIR)../framework/platform/gnu/rpcTransport.h $(PROJ_DIR)../framework/platform/gnu/rpcTransport.c $(PROJ_DIR)../framework/platform/gnu/rpcTransportUart.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/platform/gnu/rpcTransport.c

# rule for file "queue.o".
queue.o: $(PROJ_DIR)../framework/rpc/queue.h $(PROJ_DIR)../framework/rpc/queue.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../framework/rpc/queue.c

# rule for file "appinit.o".
appinit.o: $(PROJ_DIR)../znp/appinit.h $(PROJ_DIR)../znp/appinit.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/appinit.c

# rule for file "commands.o".
commands.o: $(PROJ_DIR)../znp/commands.h $(PROJ_DIR)../znp/commands.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/commands.c

# rule for file "zcl.o".
zcl.o: $(PROJ_DIR)../znp/zcl/zcl.h $(PROJ_DIR)../znp/zcl/zcl.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/zcl/zcl.c

# rule for file "zcl_ss.o".
zcl_ss.o: $(PROJ_DIR)../znp/zcl/zcl_ss.h $(PROJ_DIR)../znp/zcl/zcl_ss.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/zcl/zcl_ss.c

# rule for file "ceconf.o".
ceconf.o: $(PROJ_DIR)../ceconf.h $(PROJ_DIR)../ceconf.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../ceconf.c

# rule for file "cetimer.o".
cetimer.o: $(PROJ_DIR)../cetimer.h $(PROJ_DIR)../cetimer.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../cetimer.c

# rule for file "connection.o".
connection.o: $(PROJ_DIR)../connection.h $(PROJ_DIR)../connection.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../connection.c
	
# rule for file "event.o".
event.o: $(PROJ_DIR)../event.h $(PROJ_DIR)../event.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../event.c

# rule for file "eventhub.o".
eventhub.o: $(PROJ_DIR)../eventhub.h $(PROJ_DIR)../eventhub.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../eventhub.c
	
# rule for file "gateway.o".
gateway.o: $(PROJ_DIR)../gateway.h $(PROJ_DIR)../gateway.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../gateway.c

# rule for file "kfifo.o".
kfifo.o: $(PROJ_DIR)../kfifo.h $(PROJ_DIR)../kfifo.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../kfifo.c

# rule for file "protocol.o".
protocol.o: $(PROJ_DIR)../protocol.h $(PROJ_DIR)../protocol.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../protocol.c

# rule for file "rbtree.o".
rbtree.o: $(PROJ_DIR)../rbtree.h $(PROJ_DIR)../rbtree.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../rbtree.c

# rule for file "reconn.o".
reconn.o: $(PROJ_DIR)../reconn.h $(PROJ_DIR)../reconn.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../reconn.c
	
# rule for file "socket.o".
socket.o: $(PROJ_DIR)../socket.h $(PROJ_DIR)../socket.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../socket.c

# rule for file "toolkit.o".
toolkit.o: $(PROJ_DIR)../toolkit.h $(PROJ_DIR)../toolkit.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../toolkit.c

# rule for file "parseserver.o".
parseserver.o: $(PROJ_DIR)../parseserver.h $(PROJ_DIR)../parseserver.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../parseserver.c

# rule for file "znp.o".
znp.o: $(PROJ_DIR)../znp.h $(PROJ_DIR)../znp.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp.c

# rule for file "znp_device.o".
znp_device.o: $(PROJ_DIR)../znp/znp_device.h $(PROJ_DIR)../znp/znp_device.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/znp_device.c

# rule for file "cJSON.o".
cJSON.o: $(PROJ_DIR)../cJSON.h $(PROJ_DIR)../cJSON.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../cJSON.c

# rule for file "znp_map.o".
znp_map.o: $(PROJ_DIR)../znp/znp_map.h $(PROJ_DIR)../znp/znp_map.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../znp/znp_map.c

# rule for file "sqlitedb.o".
sqlitedb.o: $(PROJ_DIR)../sqlitedb.h $(PROJ_DIR)../sqlitedb.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../sqlitedb.c

# rule for file "sqlite3.o".
sqlite3.o: $(PROJ_DIR)../sqlite/include/sqlite3.h $(PROJ_DIR)../sqlite/sqlite3.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)../sqlite/sqlite3.c

# rule for cleaning files generated during compilations.
clean:
	/bin/rm -f gateway.bin *.o
