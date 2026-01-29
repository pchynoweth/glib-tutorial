# Makefile for Lesson 1

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = hello_glib version_check basic_types

.PHONY: all clean

all: $(TARGETS)

hello_glib: hello_glib.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

version_check: version_check.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

basic_types: basic_types.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
