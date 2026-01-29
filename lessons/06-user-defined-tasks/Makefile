# Makefile for Lesson 6

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = custom_source gtask_basic

.PHONY: all clean

all: $(TARGETS)

custom_source: custom_source.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

gtask_basic: gtask_basic.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
