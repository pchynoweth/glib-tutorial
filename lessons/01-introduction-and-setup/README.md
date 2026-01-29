# Lesson 1: Introduction and Setup

## What is GLib?

GLib is a powerful, general-purpose utility library written in C. It provides:

- **Data structures**: Lists, hash tables, trees, arrays, and more
- **Event loop**: For event-driven and asynchronous programming
- **Threading utilities**: Cross-platform threading support
- **Memory management**: Allocation functions and reference counting
- **Portability**: Cross-platform abstractions for file operations, string handling, etc.
- **Type system**: Foundation for GObject object-oriented programming

GLib is used by major projects like GTK, GNOME, GIMP, and many others.

## Why Use GLib?

1. **Cross-platform**: Write once, run on Linux, Windows, macOS, and more
2. **Well-tested**: Decades of production use in major applications
3. **Rich functionality**: Avoid reinventing common utilities
4. **Event-driven architecture**: Built-in support for modern async patterns
5. **Active development**: Regular updates and strong community

## Setting Up

### Installation

Make sure GLib is installed (see main README for installation instructions).

### Verify Installation

```bash
pkg-config --modversion glib-2.0
pkg-config --cflags --libs glib-2.0
```

## Examples in This Lesson

1. **hello_glib.c** - Your first GLib program
2. **version_check.c** - Check GLib version at compile and runtime
3. **simple_compilation.c** - Demonstrates proper compilation

## Building Examples

```bash
make
```

Or manually:

```bash
gcc `pkg-config --cflags glib-2.0` hello_glib.c -o hello_glib `pkg-config --libs glib-2.0`
```

## Key Concepts

- Including GLib headers: `#include <glib.h>`
- Using pkg-config for compilation flags
- GLib naming conventions: `g_function_name()`
- Type naming: `GType`, `gint`, `guint`, `gchar`, etc.

## Next Steps

After completing this lesson, you'll understand:
- How to set up a GLib development environment
- How to compile GLib programs
- Basic GLib conventions

Move on to Lesson 2 to learn about GLib's data structures!
