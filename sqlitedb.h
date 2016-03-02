#ifndef __CE_SQLITE_H_H
#define __CE_SQLITE_H_H

#define DBPATH "./gateway.db"

struct sqlitedb;
struct sqlitedb * sqlitedb_create(char * filepath);
void sqlitedb_exec(struct sqlitedb *, char *sql);
void sqlitedb_close_table(struct sqlitedb * sdb);
void sqlitedb_insert_record(struct sqlitedb *,char * table,int DID,int  CID,char * name,char *end);
char **sqlitedb_search_all(struct sqlitedb *,char * table);
char **sqlitedb_search_id(struct sqlitedb *,char * table,int id);
void sqlitedb_delete_id(struct sqlitedb *,char * table,int id);
void sqlitedb_delete_all(struct sqlitedb *,char * table);
//void updata_data(sqlite3 * db);
void sqlitedb_updata_data(struct sqlitedb *,int terminalID,char *terminalname);


void sqlitedb_table_build(char * filepath);
int sqlitedb_add_gateway(unsigned long long ieee, char * name);
int sqlitedb_load_gateway_name(char * filepath, unsigned long long mac);

#endif
