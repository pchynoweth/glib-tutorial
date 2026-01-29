# Makefile for Lesson 3

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = simple_loop timeout_example idle_example multiple_contexts priority_example

.PHONY: all clean

all: $(TARGETS)

simple_loop: simple_loop.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

timeout_example: timeout_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

idle_example: idle_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

multiple_contexts: multiple_contexts.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

priority_example: priority_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
