# Lesson 3: Main Event Loop and Contexts

## Overview

The GLib main event loop is the heart of event-driven programming in GLib. It's used extensively in GTK applications, network servers, and any program that needs to respond to events asynchronously.

## Key Concepts

### GMainContext

A **GMainContext** represents a set of event sources. It's the structure that monitors sources and dispatches events when they're ready.

- **Default context**: Most applications use a single, default context
- **Multiple contexts**: Advanced scenarios can use multiple contexts
- **Thread ownership**: A context can be owned by a specific thread

### GMainLoop

A **GMainLoop** is a wrapper around a context that provides a simple way to run it. It continues running until explicitly stopped.

### Event Sources

Sources represent things to monitor:
- **Timeout sources**: Execute after a delay
- **Idle sources**: Execute when nothing else is happening
- **I/O sources**: Monitor file descriptors
- **Custom sources**: User-defined event sources

### Priority

Sources have priorities that control execution order:
- `G_PRIORITY_HIGH` (-100): High priority
- `G_PRIORITY_DEFAULT` (0): Normal priority
- `G_PRIORITY_HIGH_IDLE` (100): High idle priority
- `G_PRIORITY_DEFAULT_IDLE` (200): Normal idle priority
- `G_PRIORITY_LOW` (300): Low priority

## Examples in This Lesson

1. **simple_loop.c** - Basic main loop usage
2. **timeout_example.c** - Timeout sources
3. **idle_example.c** - Idle functions
4. **multiple_contexts.c** - Working with multiple contexts
5. **context_ownership.c** - Context ownership and thread safety
6. **priority_example.c** - Source priorities

## Building Examples

```bash
make
```

## When to Use the Main Loop

- GUI applications (GTK automatically uses it)
- Network servers handling multiple connections
- Applications responding to timers or signals
- Any event-driven architecture

## Important Notes

- The main loop blocks in `g_main_loop_run()` until stopped
- Callbacks execute in the thread that runs the loop
- Long-running operations should not block the main loop
- Use threads or async operations for intensive tasks

## Next Steps

After understanding the main loop, proceed to Lesson 4 to learn about thread safety and using the main loop across threads!
