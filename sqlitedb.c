#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "sqlitedb.h"
#include "gateway.h"
#include "mtZdo.h"
//static int sn = 0;
#define GATEWAYTABLENAME "gateway"
#define DEVICETABLENAME "device"


struct sqlitedb{ 
	sqlite3 * db;
};

struct sqlitedb * sqlitedb_create(char * filepath){
	struct sqlitedb * sdb = (struct sqlitedb *) malloc(sizeof(struct sqlitedb));
	memset(sdb,0,sizeof(struct sqlitedb));


	//rc = sqlite3_open(filepath,&sdb->db);
	int rc = sqlite3_open_v2(filepath, &sdb->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc) {
		fprintf(stderr,"can't open database%s\n",sqlite3_errmsg(sdb->db));
		sqlite3_close(sdb->db);
		free(sdb);
		return NULL;
	}

	return sdb;
}

void sqlitedb_destroy(struct sqlitedb * db){
	sqlite3_close(db->db);
	free(db);
}

void sqlitedb_exec(struct sqlitedb * sdb, char *sql){
	char * zErrMsg;
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
}


void sqlitedb_close_table(struct sqlitedb * sdb)
{
	sqlite3_close(sdb->db);
}


void sqlitedb_insert_record(struct sqlitedb *sdb,char * table,int DID,int CID,char * name,char *end)
{
	char * sql;
	char * zErrMsg = NULL;

	sql = sqlite3_mprintf("insert into %s values(%d,'%d','%s','%s')",table,DID,CID,name,end);
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}


char **sqlitedb_search_all(struct sqlitedb *sdb,char * table)
{
	char * sql;
	char * zErrMsg = 0;
	int i,offset = 0;
	char * buf,* tmp;
	char zSql[256];
	char **datatable; /* the result table*/
	int nRow; /* the number of rows in the result */
	int nColumn; /* the number of columns of result */
	int rc;
	datatable = (char**)malloc(100*sizeof(char*));

	sqlite3_snprintf(sizeof(zSql), zSql,
			"select * from %s", table);

	rc=sqlite3_get_table(sdb->db, zSql,

			&datatable, &nRow, &nColumn, &zErrMsg);

	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		exit(1);
	} 


	return datatable;

}    

char **sqlitedb_search_id(struct sqlitedb *sdb, char * table,int id)
{
	char zSql[256];
	char *szError = NULL,**dbResult;
	int row,col;
	dbResult = (char**)malloc(100*sizeof(char*));
	sqlite3_snprintf(sizeof(zSql), zSql,
			"select * from %s where DeviceID=%d",table,id);


	int result = sqlite3_get_table( sdb->db,zSql,&dbResult,&row,&col,&szError );
	if( result!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", szError);
		exit(1);
	}
	return dbResult;



}

void sqlitedb_delete_id(struct sqlitedb *sdb,char * table,int id)
{
	int rc ;
	char * sql;
	char * zErrMsg = 0;
	sql = sqlite3_mprintf("delete from %s where DeviceID=%d",table,id);
	rc = sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}

void sqlitedb_delete_all(struct sqlitedb *sdb,char * table)
{
	char * sql;
	char * zErrMsg = 0;

	sql = sqlite3_mprintf("delete from %s",table);
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}

void sqlitedb_updata_data(struct sqlitedb *sdb,int terminalID,char *terminalname)
{
	char *errmsg = NULL;

	char *sql=sqlite3_mprintf("update student set Name='%s' where DeviceID='%d'",terminalname,terminalID);

	if(sqlite3_exec(sdb->db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		perror("sqlite3_exec_update");
		exit(-1);
	}
	else
		printf("update success!!\n");

}

static const char table_exist[] = "SELECT name FROM sqlite_master WHERE type='table' AND name='%s';";

int _sqlitedb_table_exist(struct sqlitedb * db, char * tablename){ 
	struct sqlite3_stmt * stmt;
	const char pztile[256];
	char _table_exist[128]={0};
	sprintf(_table_exist, table_exist, tablename);
	int ret = sqlite3_prepare_v2(db->db, _table_exist, 128, &stmt, &pztile);

	int result = 0;
	if (ret==SQLITE_OK){
		int r = sqlite3_step(stmt);
		if(r == SQLITE_DONE){
		}else if(r == SQLITE_ROW){
			result = 1;
		}else{
			assert(0);
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);

	return result;

}

static const char sql_create_gateway_table[] = "create table gateway(mac integer primary key, name text);";
static const char sql_create_device_table[] = "create table device(ieee integer primary key,name text, status integer,zclversion integer, applicationversion integer, stackversion integer, hwversion integer, manufacturername text,modelidentifier text,datecode text,endpoint blob);";

void sqlitedb_table_build(char * filepath){
	struct sqlitedb * db = sqlitedb_create(filepath);
	
	if(db){ 
		if(!_sqlitedb_table_exist(db, GATEWAYTABLENAME)){
			sqlite3_exec(db->db,sql_create_gateway_table,NULL,NULL,NULL);
		}

		if(!_sqlitedb_table_exist(db, DEVICETABLENAME)){
			sqlite3_exec(db->db,sql_create_device_table,NULL,NULL,NULL);
		}
	}
	sqlitedb_destroy(db);
}

static const char sql_select_gateway[] = "select name from gateway where mac=%lld";

int sqlitedb_load_gateway_name(char * filepath, unsigned long long mac){
	struct sqlitedb * db = sqlitedb_create(filepath);
	struct sqlite3_stmt * stmt;

	char select_gateway_name[64] = {0};
	sprintf(select_gateway_name, sql_select_gateway, mac);
	const char pztile[256];
	int ret = sqlite3_prepare_v2(db->db, select_gateway_name, sizeof(select_gateway_name), &stmt, &pztile);

	int result = 1;
	if (ret==SQLITE_OK){
		int r = sqlite3_step(stmt);
		if(r == SQLITE_DONE){ 
		}else if(r == SQLITE_ROW){
			char gatewayname[256] = {0};
			int bytes;
		        const unsigned char * text;
			bytes = sqlite3_column_bytes(stmt, 0);
		        text  = sqlite3_column_text (stmt, 0);
			memcpy(gatewayname, text, bytes);

			gateway_init(getgateway(), mac, gatewayname, 1, 1);

			result = 0;
		}else{
			assert(0);
		}

		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);

	sqlitedb_destroy(db);

	return result;
}

static const char sql_insert_gateway[256] = "insert into gateway(mac, name)values(%lld, \"%s\");";

int sqlitedb_add_gateway(unsigned long long ieee, char * name){ 

	struct sqlitedb * db = sqlitedb_create(DBPATH);

	char insert_table_gateway[512] = {0};
	sprintf(insert_table_gateway, sql_insert_gateway, ieee, name);
	char errmsg[128] = {0};
	int ret = sqlite3_exec(db->db,insert_table_gateway,NULL,NULL,&errmsg);

	sqlitedb_destroy(db);

	return 0;
}

void _sqlite3_gettext(sqlite3_stmt *stmt, int col, char * value){
	int bytes;
	const unsigned char * text;
	bytes = sqlite3_column_bytes(stmt, col);
	text  = sqlite3_column_text (stmt, col);
	memcpy(value, text, bytes);
}

void _sqlite3_load_device(sqlite3_stmt *stmt, int col, struct device *d){
	int bytes;
	const unsigned char * blob;
	bytes = sqlite3_column_bytes(stmt, col);
	if(bytes < sizeof(ActiveEpRspFormat_t)){
		return;
	}
	blob = sqlite3_column_blob(stmt, col);
	ActiveEpRspFormat_t activeep;
	memset(&activeep, 0, sizeof(ActiveEpRspFormat_t));
	memcpy(&activeep, blob, sizeof(ActiveEpRspFormat_t));
	blob += sizeof(ActiveEpRspFormat_t);
	device_setep(d, &activeep);

	struct endpoint * ep;
	SimpleDescRspFormat_t simpledesc;
	unsigned char i;
	for(i = 0; i < activeep.ActiveEPCount; i++){ 
		memcpy(&simpledesc, blob, sizeof(SimpleDescRspFormat_t));
		blob += sizeof(SimpleDescRspFormat_t);
		ep = endpoint_create(&simpledesc);
		device_addendpoint(d, ep);
	}


}

static const char sql_select_device[] = "select ieee ,name , status ,zclversion , applicationversion , stackversion , hwversion , manufacturername ,modelidentifier ,datecode ,endpoint from device;";
void sqlitedb_load_device(){ 
	struct sqlitedb * db = sqlitedb_create(DBPATH);

	struct sqlite3_stmt * stmt;
	const char pztile[256];
	int ret = sqlite3_prepare_v2(db->db, sql_select_device, sizeof(sql_select_device), &stmt, &pztile);

	unsigned long long ieee;
	char devicename[MAXNAMELEN]; 
	unsigned char status;
	unsigned char zclversion;
	unsigned char applicationversion;
	unsigned char stackversion;
	unsigned char hwversion;
	char manufacturername[33];
	char modelidentifier[33];
	char datecode[17];

	if (ret==SQLITE_OK){
		while(sqlite3_step(stmt) == SQLITE_ROW){
			ieee = sqlite3_column_int64(stmt,0);
			memset(devicename, 0, MAXNAMELEN);
			_sqlite3_gettext(stmt, 1, devicename);
			status = sqlite3_column_int(stmt,2);
			zclversion = sqlite3_column_int(stmt,3);
			applicationversion = sqlite3_column_int(stmt,4);
			stackversion = sqlite3_column_int(stmt,5);
			hwversion = sqlite3_column_int(stmt,6);
			memset(manufacturername, 0, 33);
			_sqlite3_gettext(stmt,7,manufacturername);
			memset(modelidentifier, 0, 33);
			_sqlite3_gettext(stmt,8,modelidentifier);
			memset(datecode, 0, 17);
			_sqlite3_gettext(stmt,9,datecode);
			struct device * d = device_create2(ieee, devicename, status, zclversion, applicationversion, stackversion, hwversion, manufacturername, modelidentifier, datecode);
			_sqlite3_load_device(stmt,10,d);
			gateway_adddevice(getgateway(),d);

		}
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);

	sqlitedb_destroy(db);
}

static const char sql_insert_device_ieee[] = "insert into device(ieee) values(%lld);";
int sqlitedb_insert_device_ieee(unsigned long long ieee){
	struct sqlitedb * db = sqlitedb_create(DBPATH);

	char insert_device[128] = {0};
	sprintf(insert_device, sql_insert_device_ieee, ieee);
	char errmsg[128] = {0};
	sqlite3_exec(db->db,insert_device,NULL,NULL,&errmsg);

	sqlitedb_destroy(db);

	return 0;
}

int sqlitedb_update_device_endpoint(struct device * d){
	struct sqlitedb * db = sqlitedb_create(DBPATH);
	if(db){ 
		sqlite3_blob * blob = NULL;
		int ret = sqlite3_blob_open(db->db, 
				"main",
				"device",
				"endpoint",
				d->ieeeaddr,
				1,
				&blob);
		const char* result = sqlite3_errmsg(db->db);
		fprintf(stdout, "------------------ %s \n", result);
		if(ret == SQLITE_OK){
			int cursor = 0;
			sqlite3_blob_write(blob, &d->activeep, sizeof(ActiveEpRspFormat_t),cursor);
			cursor += sizeof(ActiveEpRspFormat_t);
			struct endpoint * ep;
			struct list_head * pos, *n;
			list_for_each_safe(pos, n, &d->eplisthead){ 
				ep = list_entry(pos, struct endpoint, list); 
				sqlite3_blob_write(blob, &ep->simpledesc, sizeof(SimpleDescRspFormat_t), cursor);
				cursor += sizeof(SimpleDescRspFormat_t);
			}

		}

		sqlite3_blob_close(blob);

	}
	sqlitedb_destroy(db);
}