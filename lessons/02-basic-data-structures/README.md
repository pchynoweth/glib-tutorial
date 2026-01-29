# Lesson 2: Basic Data Structures

## Overview

GLib provides a rich set of data structures that are essential for C programming. These are type-safe, well-tested, and commonly used throughout the GLib ecosystem.

## Data Structures Covered

### Lists
- **GList**: Doubly-linked list - allows traversal in both directions
- **GSList**: Singly-linked list - more memory efficient, forward traversal only

### Hash Tables
- **GHashTable**: Hash table with automatic resizing and collision handling

### Arrays
- **GArray**: Dynamic array of fixed-size elements
- **GPtrArray**: Dynamic array of pointers

### Strings
- **GString**: Mutable string buffer with automatic memory management

### Queues
- **GQueue**: Double-ended queue (built on GList)

## Key Concepts

1. **Memory Ownership**: GLib follows consistent patterns for memory management
2. **Destroy Functions**: Many structures accept a `GDestroyNotify` for cleanup
3. **Deep vs Shallow**: Understanding when data is copied vs referenced
4. **Iteration**: Different ways to traverse structures

## Examples in This Lesson

1. **glist_example.c** - Working with doubly-linked lists
2. **hash_table_example.c** - Hash tables and key-value storage
3. **array_example.c** - Dynamic arrays
4. **string_example.c** - String manipulation with GString
5. **queue_example.c** - Queue operations

## Building Examples

```bash
make
```

## Memory Management Tips

- Always free structures when done: `g_list_free()`, `g_hash_table_destroy()`, etc.
- Use `_full` variants to free data: `g_list_free_full()`, `g_hash_table_destroy_full()`
- Be careful about ownership - who's responsible for freeing?

## Next Steps

After this lesson, move to Lesson 3 to learn about the event loop and contexts!
