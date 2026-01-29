# Makefile for Lesson 2

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = glist_example hash_table_example array_example string_example queue_example

.PHONY: all clean

all: $(TARGETS)

glist_example: glist_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

hash_table_example: hash_table_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

array_example: array_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

string_example: string_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

queue_example: queue_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
