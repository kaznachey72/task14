CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -D_GNU_SOURCE -g
LFLAGS = $(shell pkg-config --libs libpq)
PG_INC = $(shell pg_config --includedir)

ex14: main.o db.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^ -o $@

db.o: db.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(PG_INC)

clean:
	rm -f ex14 *.o
