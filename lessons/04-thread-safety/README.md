# Lesson 4: Thread Safety and GLib Threading

## Overview

GLib provides comprehensive threading support with mutexes, condition variables, thread pools, and thread-safe operations. Understanding GLib's threading model is crucial for building robust multi-threaded applications.

## Key Concepts

### Thread Safety in GLib

- **Main context**: Can be used by only one thread at a time
- **Context pushing**: Use `g_main_context_push_thread_default()` to make a context default for a thread
- **Atomic operations**: GLib provides atomic operations for lock-free programming
- **Thread-local storage**: Per-thread data storage

### Synchronization Primitives

- **GMutex**: Mutual exclusion locks
- **GCond**: Condition variables for thread signaling
- **GRWLock**: Read-write locks
- **GOnce**: One-time initialization
- **GRecMutex**: Recursive mutexes

### Thread Management

- **GThread**: Thread creation and management
- **GThreadPool**: Pool of worker threads
- **Thread-safe queues**: `GAsyncQueue` for producer-consumer patterns

## Examples in This Lesson

1. **basic_threading.c** - Creating and joining threads
2. **mutex_example.c** - Protecting shared data with mutexes
3. **condition_variable.c** - Thread coordination with conditions
4. **thread_pool.c** - Using GThreadPool for parallel work
5. **async_queue.c** - Producer-consumer with GAsyncQueue
6. **context_threading.c** - Main contexts across threads

## Building Examples

```bash
make
```

## Thread Safety Rules

1. **Don't share GMainContext** between threads without proper synchronization
2. **Use atomic operations** for simple shared counters
3. **Protect shared data** with mutexes
4. **Use g_idle_add()** to schedule callbacks in the main thread from worker threads
5. **Always join threads** or use detached threads appropriately

## Common Patterns

### Worker Thread Pattern
- Main thread runs UI/event loop
- Worker threads do heavy computation
- Use `g_idle_add()` to update UI from worker

### Producer-Consumer Pattern
- Multiple threads produce/consume data
- Use `GAsyncQueue` for thread-safe communication
- No explicit locking needed

## Important Notes

- GLib thread functions are thin wrappers around POSIX threads
- Thread creation is relatively expensive - use thread pools for many short tasks
- Be careful about deadlocks when using multiple locks
- Always initialize GLib's thread system (automatic in modern GLib)

## Next Steps

After understanding threading, move to Lesson 5 to learn about memory management and pointer lifetimes in multi-threaded contexts!
