# Makefile for Lesson 5

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = basic_allocation

.PHONY: all clean

all: $(TARGETS)

basic_allocation: basic_allocation.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
