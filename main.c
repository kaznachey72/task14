#include "db.h"

#include <stdio.h>
#include <stdlib.h>


static void show_dbinfo(const char *db_name, const char *db_table, const char *db_column)
{
    const char *aggr_names[] = {"SUM", "AVG", "MIN", "MAX", "VAR_SAMP", NULL};

    db_conn *conn = db_ctor(db_name);
    for (size_t i=0; aggr_names[i]; ++i) {
        char *val = db_getval(conn, aggr_names[i], db_table, db_column);
        printf("%8s: %s\n", aggr_names[i], val);
        free(val);
    }
    db_dtor(conn);
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "USAGE: %s db_name table column\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    show_dbinfo(argv[1], argv[2], argv[3]);

    return EXIT_SUCCESS;
}
