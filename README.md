# GLib Tutorial Series

A comprehensive tutorial series for learning GLib with practical C examples.

## About GLib

GLib is a low-level core library that forms the basis of GTK and GNOME. It provides data structure handling for C, portability wrappers, and interfaces for such runtime functionality as an event loop, threads, dynamic loading, and an object system.

## Prerequisites

- C compiler (gcc or clang)
- GLib development libraries
- Basic C programming knowledge

### Installing GLib

**Ubuntu/Debian:**
```bash
sudo apt-get install libglib2.0-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install glib2-devel
```

**macOS:**
```bash
brew install glib
```

## Tutorial Structure

This tutorial is organized into progressive lessons, each building on the previous ones:

1. **[Lesson 1: Introduction and Setup](lessons/01-introduction-and-setup/)**
   - What is GLib and why use it
   - Setting up your development environment
   - Your first GLib program
   - Basic compilation with pkg-config

2. **[Lesson 2: Basic Data Structures](lessons/02-basic-data-structures/)**
   - GList (doubly linked lists)
   - GSList (singly linked lists)
   - GHashTable (hash tables)
   - GArray and GPtrArray
   - String handling with GString

3. **[Lesson 3: Main Event Loop and Contexts](lessons/03-main-loop-and-contexts/)**
   - Understanding GMainContext
   - Creating and running GMainLoop
   - Event sources and priorities
   - Multiple contexts and context ownership
   - Idle functions and timeouts

4. **[Lesson 4: Thread Safety and GLib Threading](lessons/04-thread-safety/)**
   - GLib's thread support
   - GMutex and GCond
   - GThread creation and management
   - Thread-safe operations
   - Context switching between threads

5. **[Lesson 5: Memory Management and Pointer Lifetimes](lessons/05-memory-management/)**
   - GLib's memory functions
   - Reference counting with GObject
   - Weak references
   - Memory pools and slices
   - Common memory pitfalls and best practices

6. **[Lesson 6: Creating and Running User-Defined Tasks](lessons/06-user-defined-tasks/)**
   - Creating custom GSource objects
   - Task scheduling and execution
   - Integration with the main loop
   - Cancellable operations with GCancellable
   - Using GTask for asynchronous operations

7. **[Lesson 7: Asynchronous Operations](lessons/07-async-operations/)**
   - Asynchronous I/O with GIO
   - Callback patterns
   - Promise-style operations with GTask
   - Async ready callbacks
   - Error handling in async code

8. **[Lesson 8: Advanced Topics](lessons/08-advanced-topics/)**
   - Custom data structures
   - GVariant for serialization
   - Signal system basics
   - Performance optimization tips
   - Debugging GLib applications

## Building Examples

Each lesson contains a `Makefile` for building the examples. You can also use the top-level Makefile:

```bash
# Build all examples
make all

# Build specific lesson
make lesson01

# Clean build artifacts
make clean
```

Or compile manually using pkg-config:

```bash
gcc `pkg-config --cflags glib-2.0` example.c -o example `pkg-config --libs glib-2.0`
```

## Running Examples

Each example includes comments explaining what it does. Run examples like:

```bash
cd lessons/01-introduction-and-setup
make
./hello_glib
```

## Learning Path

For beginners, follow the lessons in order. Each lesson builds on concepts from previous ones.

If you're already familiar with C and want to focus on specific topics:
- **Event-driven programming**: Start with Lesson 3
- **Multi-threading**: Start with Lesson 4, review Lesson 3
- **Memory management**: Lesson 5 is comprehensive
- **Async tasks**: Lessons 6 and 7

## Additional Resources

- [GLib Official Documentation](https://docs.gtk.org/glib/)
- [GLib Reference Manual](https://developer.gnome.org/glib/)
- [GIO Reference Manual](https://developer.gnome.org/gio/)

## License

This tutorial is provided under the MIT License. See LICENSE file for details.

## Contributing

Found an error or want to improve an example? Contributions are welcome!

## Getting Help

If you get stuck:
1. Check the comments in the example code
2. Review the lesson README
3. Consult the official GLib documentation
4. Look at other examples in the same lesson

Happy learning!