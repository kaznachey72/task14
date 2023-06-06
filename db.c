#include "db.h"

#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--- PostreSQL API -----------------------------------------------------------

static void exit_nicely(PGconn *conn, PGresult *res)
{   
    if (res) {
        PQclear(res);
    }
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

static PGconn *get_conn(const char *db_name)
{
    const char *conn_template = "host=localhost port=5432 user=pguser password=pgpswd dbname=%s";
    size_t conn_size = snprintf(NULL, 0, conn_template, db_name) + sizeof('\0');
    char *conn_info = malloc(conn_size);
    if (!conn_info) {
        fprintf(stderr, "error: bad alloc\n");
        exit(EXIT_FAILURE);
    }
    snprintf(conn_info, conn_size, conn_template, db_name);
    conn_info[conn_size - 1] = '\0';

    PGconn *conn = PQconnectdb(conn_info);
    free(conn_info);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "CONNECTION %s\n", PQerrorMessage(conn));
        exit_nicely(conn, NULL);
    }

    return conn;
}

static char *get_val(PGconn *conn, const char *aggr_name, const char *db_table, const char *db_column)
{
    const char *query_tempalte = "SELECT %s(%s) FROM %s";
    size_t query_size = snprintf(NULL, 0, query_tempalte, aggr_name, db_column, db_table) + sizeof('\0');
    char *query = malloc(query_size);
    if (!query) {
        fprintf(stderr, "error: bad alloc\n");
        exit_nicely(conn, NULL);
    }
    snprintf(query, query_size, query_tempalte, aggr_name, db_column, db_table);
    query[query_size - 1] = '\0';

    PGresult *res = PQexec(conn, query);
    free(query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "QUERY %s\n", PQerrorMessage(conn));
        exit_nicely(conn, res);
    }

    char *value = NULL;
    if (PQntuples(res) > 0) {
        char *field = PQgetvalue(res, 0, 0);
        value = strdup(field);
    }

    PQclear(res);
    return value;
}

static void fin_conn(PGconn *conn)
{
    PQfinish(conn);
}

//--- PUBLIC API --------------------------------------------------------------

db_conn *db_ctor(const char *db_name)
{
    return get_conn(db_name);
}

char *db_getval(db_conn *conn, const char *aggr_name, const char *db_table, const char *db_column)
{
    return get_val(conn, aggr_name, db_table, db_column);
}

void db_dtor(db_conn *conn)
{
    fin_conn(conn);
}

