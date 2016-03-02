#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "eventhub.h"
#include "connection.h"
#include "toolkit.h"
#include "cetimer.h"
#include "socket.h"
#include "ceconf.h"
#include "connection.h"
#include "reconn.h"
#include "gateway.h"
#include "znp.h"
#include "sqlitedb.h"

int main(){
        ceconf_load();	

	sqlitedb_table_build(DBPATH);

	unsigned long long mac = toolkit_getmac();
	if(sqlitedb_load_gateway_name(DBPATH, mac)){ 
		gateway_init(getgateway(), mac, "网关", 1, 1);
		sqlitedb_add_gateway(mac, "网关"); 
	}

	// create pipe for timer to main
	int wfd;
	struct connection * readconn = createpipe(&wfd);

	// create pipe for timer to reconnect
	int reconnrfd, reconnwfd;
	reconnrfd = createpipe2(&reconnwfd);

	//  create pipe for reconnect to main
	int rmwfd;
	struct connection * mrreadconn = createpipe(&rmwfd);

	// create timer
	struct cetimer * timer = cetimer_create(5, 1, wfd, reconnwfd);
	cetimer_start(timer);

	// create eventhub 
	struct eventhubconf hubconf;
	memset(&hubconf, 0, sizeof(struct eventhubconf));
	memcpy(&hubconf.port, ceconf_getlistenport(),strlen(ceconf_getlistenport())); 
	struct eventhub * hub = eventhub_create(&hubconf);

	// start reconn thread
	reconn_start(reconnrfd, rmwfd,  hub);
	
	// add cmd pipe
	if(readconn){
		eventhub_register(hub, connection_getfd(readconn));
	}

	// add reconn to main rfd
	if( mrreadconn ){
		eventhub_register(hub, connection_getfd(mrreadconn));
	}

	// create pipe for znp to main
	int mainrfd, znpwfd;
	mainrfd = createpipe2(&znpwfd);
	struct connection * znpconnection = freeconnlist_getconn();
	connection_init(znpconnection, mainrfd, CONNZNP);

	// create pipe for main to znp
	int mainwfd, znprfd;
	znprfd = createpipe2(&mainwfd);
	make_socket_non_blocking(mainwfd);

	// open serial port
	if(znp_start(znpwfd, znprfd, ceconf_getserialport()) == -1){
		return 1;
	}
	if(znpconnection){
		eventhub_register(hub, connection_getfd(znpconnection));
		connrbtree_insert(znpconnection);
	}

	eventhub_start(hub);
}
