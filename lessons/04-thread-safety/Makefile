# Makefile for Lesson 4

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = basic_threading mutex_example async_queue context_threading

.PHONY: all clean

all: $(TARGETS)

basic_threading: basic_threading.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

mutex_example: mutex_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

async_queue: async_queue.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

context_threading: context_threading.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
