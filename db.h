#ifndef DB_H
#define DB_H

struct pg_conn;
typedef struct pg_conn db_conn;

db_conn *db_ctor(const char *db_name);
char *db_getval(db_conn *conn, const char *aggr_name, const char *db_table, const char *db_column);
void db_dtor(db_conn *conn);

#endif // DB_H
