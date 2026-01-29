# Lesson 6: Creating and Running User-Defined Tasks

## Overview

This lesson covers how to create custom tasks that integrate with GLib's event loop. You'll learn to create custom GSources, schedule tasks, and use GTask for asynchronous operations.

## Key Concepts

### Custom GSource

- **GSource**: Base type for all event sources
- **Source functions**: Prepare, check, dispatch callbacks
- **Custom sources**: Monitor custom events or conditions
- **Source lifecycle**: Creation, attachment, dispatch, destruction

### GTask for Async Operations

- **GTask**: Modern API for asynchronous operations
- **Thread pool integration**: Run tasks in worker threads
- **Cancellation**: Cancel long-running operations
- **Return values**: Propagate results back to caller
- **Error handling**: Proper error reporting in async contexts

### Task Scheduling

- **Immediate execution**: Run in current iteration
- **Deferred execution**: Schedule for later
- **Periodic tasks**: Repeating operations
- **Priority scheduling**: Control execution order

## Examples in This Lesson

1. **custom_source.c** - Creating a custom GSource
2. **gtask_basic.c** - Basic GTask usage
3. **gtask_thread.c** - GTask with thread pool
4. **cancellable_task.c** - Cancellable operations
5. **task_chain.c** - Chaining asynchronous tasks

## Building Examples

```bash
make
```

## When to Use What

- **Custom GSource**: When you need fine control over event monitoring
- **GTask**: For most asynchronous operations (preferred)
- **Thread pool**: For CPU-intensive work that shouldn't block
- **Cancellable**: For operations users might want to cancel

## Best Practices

1. Always handle cancellation gracefully
2. Report errors properly with GError
3. Use thread pool for blocking operations
4. Clean up resources in finish functions
5. Document ownership of return values

## Next Steps

After mastering user-defined tasks, move to Lesson 7 for more async patterns and Lesson 8 for advanced topics!
