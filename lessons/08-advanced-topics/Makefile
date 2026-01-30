# Makefile for Lesson 8

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs glib-2.0`

TARGETS = gvariant_example custom_data_structure debugging_example performance_tips

.PHONY: all clean

all: $(TARGETS)

gvariant_example: gvariant_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

custom_data_structure: custom_data_structure.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

debugging_example: debugging_example.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

performance_tips: performance_tips.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
