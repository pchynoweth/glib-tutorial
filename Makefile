# Top-level Makefile for GLib Tutorial

.PHONY: all clean lesson01 lesson02 lesson03 lesson04 lesson05 lesson06 lesson07 lesson08 lesson09

all: lesson01 lesson02 lesson03 lesson04 lesson05 lesson06 lesson07 lesson08 lesson09

lesson01:
	@echo "Building Lesson 1..."
	@$(MAKE) -C lessons/01-introduction-and-setup

lesson02:
	@echo "Building Lesson 2..."
	@$(MAKE) -C lessons/02-basic-data-structures

lesson03:
	@echo "Building Lesson 3..."
	@$(MAKE) -C lessons/03-main-loop-and-contexts

lesson04:
	@echo "Building Lesson 4..."
	@$(MAKE) -C lessons/04-thread-safety

lesson05:
	@echo "Building Lesson 5..."
	@$(MAKE) -C lessons/05-memory-management

lesson06:
	@echo "Building Lesson 6..."
	@$(MAKE) -C lessons/06-user-defined-tasks

lesson07:
	@echo "Building Lesson 7..."
	@$(MAKE) -C lessons/07-async-operations

lesson08:
	@echo "Building Lesson 8..."
	@$(MAKE) -C lessons/08-advanced-topics

lesson09:
	@echo "Building Lesson 9..."
	@$(MAKE) -C lessons/09-io-uring-gsource

clean:
	@echo "Cleaning all lessons..."
	@$(MAKE) -C lessons/01-introduction-and-setup clean
	@$(MAKE) -C lessons/02-basic-data-structures clean
	@$(MAKE) -C lessons/03-main-loop-and-contexts clean
	@$(MAKE) -C lessons/04-thread-safety clean
	@$(MAKE) -C lessons/05-memory-management clean
	@$(MAKE) -C lessons/06-user-defined-tasks clean
	@$(MAKE) -C lessons/07-async-operations clean
	@$(MAKE) -C lessons/08-advanced-topics clean
	@$(MAKE) -C lessons/09-io-uring-gsource clean
