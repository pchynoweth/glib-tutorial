# Makefile for Lesson 7

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0 gio-2.0`
LIBS = `pkg-config --libs glib-2.0 gio-2.0`

TARGETS = async_file_io parallel_async async_timeout error_handling

.PHONY: all clean

all: $(TARGETS)

async_file_io: async_file_io.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

parallel_async: parallel_async.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

async_timeout: async_timeout.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

error_handling: error_handling.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
