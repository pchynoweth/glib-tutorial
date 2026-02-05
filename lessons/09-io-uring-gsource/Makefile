# Makefile for Lesson 9

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0` -luring

TARGETS = io_uring_gsource

.PHONY: all clean

all: $(TARGETS)

io_uring_gsource: io_uring_gsource.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
