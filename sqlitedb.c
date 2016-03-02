#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "sqlitedb.h"
//static int sn = 0;
#define GATEWAYTABLENAME "gateway"
#define DEVICETABLENAME "device"

struct sqlitedb{ 
	sqlite3 * db;
};

struct sqlitedb * sqlitedb_create(char * filepath){
	struct sqlitedb * sdb = (struct sqlitedb *) malloc(sizeof(struct sqlitedb));
	memset(sdb,0,sizeof(struct sqlitedb));

	char * zErrMsg = 0;
	int rc;

	rc = sqlite3_open(filepath,&sdb->db);
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

static const char create_gateway_table[] = "create table gateway(mac integer primary key, name text);";
static const char create_device_table[] = "create table device(ieee integer primary key,name text, zclversion integer, applicationversion integer, stackversion integer, hwversion integer, manufacturername text,modelidentifier text text,datecode text,endpoint blob);";

void sqlitedb_table_build(char * filepath){
	struct sqlitedb * db = sqlitedb_create(filepath);
	
	if(db){ 
		if(!_sqlitedb_table_exist(db, GATEWAYTABLENAME)){
			sqlite3_exec(db->db,create_gateway_table,NULL,NULL,NULL);
		}

		if(!_sqlitedb_table_exist(db, DEVICETABLENAME)){
			sqlite3_exec(db->db,create_device_table,NULL,NULL,NULL);
		}
	}
	sqlitedb_destroy(db);
}

static const char select_gateway[] = "select name from gateway where mac=%lld";

int sqlitedb_load_gateway_name(char * filepath, unsigned long long mac){
	struct sqlitedb * db = sqlitedb_create(filepath);
	struct sqlite3_stmt * stmt;

	char select_gateway_name[64] = {0};
	sprintf(select_gateway_name, select_gateway, mac);
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

			gateway_init(getgateway(), mac, gatewayname);

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

static const char insert_gateway[256] = "insert into gateway(mac, name)values(%lld, \"%s\");";

int sqlitedb_add_gateway(unsigned long long ieee, char * name){ 

	struct sqlitedb * db = sqlitedb_create(DBPATH);

	char insert_table_network[512] = {0};
	sprintf(insert_table_network, insert_gateway, ieee, name);
	char errmsg[128] = {0};
	int ret = sqlite3_exec(db->db,insert_table_network,NULL,NULL,&errmsg);

	sqlitedb_destroy(db);

	return 0;
}
