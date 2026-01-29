# Lesson 5: Memory Management and Pointer Lifetimes

## Overview

Proper memory management is critical in C programming. GLib provides utilities and patterns that make memory management safer and more predictable. Understanding pointer lifetimes prevents memory leaks and use-after-free bugs.

## Key Concepts

### GLib Memory Functions

- **g_malloc() / g_free()**: Basic allocation (abort on failure)
- **g_try_malloc()**: Allocation that returns NULL on failure
- **g_new() / g_new0()**: Type-safe allocation macros
- **g_malloc0()**: Zero-initialized allocation
- **g_realloc()**: Resize allocation
- **g_memdup()**: Duplicate memory block

### Reference Counting

- **Ownership transfer**: Who's responsible for freeing?
- **Reference counting**: Shared ownership model
- **Floating references**: One-time ownership patterns
- **Weak references**: Non-owning pointers

### Memory Ownership Patterns

1. **Caller owns**: Caller must free the returned value
2. **Callee owns**: Function retains ownership, don't free
3. **Transfer ownership**: Ownership passes from caller to callee
4. **Shared ownership**: Reference counting for multiple owners

### Memory Slices (deprecated but educational)

- Historical GLib optimization for small allocations
- Now just aliases to malloc/free in modern GLib
- Understanding shows evolution of GLib

## Examples in This Lesson

1. **basic_allocation.c** - GLib memory allocation functions
2. **ownership_patterns.c** - Different ownership scenarios
3. **reference_counting.c** - Simulating reference counting
4. **memory_pools.c** - Memory pool pattern
5. **common_mistakes.c** - Common memory errors and how to avoid them

## Building Examples

```bash
make
```

## Memory Safety Rules

1. **Every allocation needs a free**: Track ownership carefully
2. **Don't free twice**: Causes crashes
3. **Don't use after free**: Leads to corruption
4. **Initialize pointers to NULL**: Makes debugging easier
5. **Clear pointers after free**: Prevents use-after-free

## Tools for Memory Debugging

- **Valgrind**: Detect memory leaks and errors
- **AddressSanitizer**: Compile-time memory error detection
- **GLib memory profiler**: `G_SLICE` environment variable

```bash
# Run with valgrind
valgrind --leak-check=full ./your_program

# Compile with AddressSanitizer
gcc -fsanitize=address ...
```

## Common Pitfalls

- **Memory leaks**: Allocated memory never freed
- **Double free**: Freeing same memory twice
- **Use after free**: Using pointer after it's freed
- **Buffer overflow**: Writing beyond allocated memory
- **Uninitialized memory**: Reading before writing

## Best Practices

1. Keep ownership clear and documented
2. Use g_autoptr when available (GLib 2.44+)
3. Free memory in reverse order of allocation
4. Use destructors/cleanup functions consistently
5. Validate pointers before use
6. Consider using memory pools for many small allocations

## Next Steps

After mastering memory management, proceed to Lesson 6 to learn about creating user-defined tasks!
