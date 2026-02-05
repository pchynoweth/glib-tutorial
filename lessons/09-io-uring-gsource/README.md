# Lesson 9: io_uring Integration with GLib GSource

## Overview

This lesson demonstrates how to integrate Linux's high-performance io_uring I/O interface with the GLib main loop using a custom GSource. This allows you to leverage io_uring's async I/O capabilities while maintaining compatibility with GLib-based applications.

## Key Concepts

- **io_uring**: Modern Linux kernel interface for asynchronous I/O operations
- **Custom GSource**: Integrating external event sources with GLib main loop
- **Submission Queue (SQ)**: Where I/O operations are queued
- **Completion Queue (CQ)**: Where completed operations are reported
- **Event Loop Integration**: Using GPollFD to monitor io_uring file descriptor

## What This Example Does

The example program:
1. Creates a custom GSource that wraps an io_uring instance
2. Integrates io_uring's ring_fd with GLib's polling mechanism
3. Submits a write operation to write buffer contents to a file
4. Processes completions through the GLib main loop
5. Demonstrates proper cleanup and resource management

## Building the Example

### Prerequisites

You'll need:
- GLib 2.0 development files (`libglib2.0-dev` on Debian/Ubuntu)
- liburing development files (`liburing-dev` on Debian/Ubuntu)
- Linux kernel 5.1+ (for io_uring support)

### Installation on Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install libglib2.0-dev liburing-dev
```

### Build Command

```bash
make
```

This will compile the example using pkg-config for GLib and link with liburing.

## Running the Example

```bash
./io_uring_gsource
```

The program will:
- Initialize io_uring with a custom GSource
- Submit an async write operation
- Process the completion in the GLib main loop
- Write output to `/tmp/io_uring_test.txt`

## Expected Output

```
=== io_uring GLib GSource Integration ===

[Init] io_uring initialized successfully
[Main] Submitting write operation...
[Submit] Opening file: /tmp/io_uring_test.txt (fd=X)
[Submit] Write operation prepared (XXX bytes)
[Submit] Submitted 2 operation(s)
[Main] Running main loop, waiting for completions...

[io_uring] Operation completed: XXX bytes
[Callback] Operation 1 completed
[io_uring] Operation completed: 0 bytes
[Callback] Operation 2 completed
[Callback] All operations completed, quitting main loop
[Cleanup] Finalizing io_uring source

=== Key Points ===
- io_uring provides high-performance async I/O
- Custom GSource integrates io_uring with GLib main loop
- GSource polls io_uring's ring_fd for completions
- Operations are submitted via submission queue (SQE)
- Completions are retrieved via completion queue (CQE)
- File written to: /tmp/io_uring_test.txt
```

## Architecture

### Custom GSource Implementation

The `IoUringSource` structure combines:
- A `GSource` base structure
- An `io_uring` instance
- A `GPollFD` for monitoring the ring file descriptor

### GSource Callbacks

1. **prepare**: Always returns FALSE, relies on polling
2. **check**: Returns TRUE when io_uring has completions ready
3. **dispatch**: Processes all available completions
4. **finalize**: Cleans up io_uring resources

### Integration Pattern

```
    GLib Main Loop
         |
         v
    Custom GSource
         |
         v
      GPollFD (monitors ring_fd)
         |
         v
     io_uring
    (SQ → Kernel → CQ)
```

## Use Cases

This integration pattern is useful for:
- High-performance file I/O in GLib applications
- Network I/O with io_uring's async operations
- Storage applications requiring low-latency I/O
- Systems programming with GLib event loop
- Building responsive applications with efficient I/O

## Advantages of io_uring

- **Zero-copy**: Reduces data copying between kernel and userspace
- **Batching**: Submit multiple operations at once
- **Polling**: Optional kernel polling for ultra-low latency
- **Flexibility**: Supports many operation types (read, write, fsync, etc.)
- **Performance**: Significantly faster than traditional async I/O

## Next Steps

- Experiment with different io_uring operations (read, fsync, etc.)
- Try batching multiple operations
- Explore io_uring's advanced features (polling, buffers)
- Integrate with real-world GLib applications

## Further Reading

- [io_uring documentation](https://kernel.dk/io_uring.pdf)
- [liburing API reference](https://github.com/axboe/liburing)
- [GLib GSource documentation](https://docs.gtk.org/glib/struct.Source.html)

## Troubleshooting

### "Failed to initialize io_uring"
- Ensure your kernel version is 5.1 or newer
- Check if io_uring is enabled in your kernel config
- Run `uname -r` to verify kernel version

### "liburing not found"
- Install liburing development package
- On Ubuntu/Debian: `sudo apt-get install liburing-dev`

### "Package glib-2.0 was not found"
- Install GLib development package
- On Ubuntu/Debian: `sudo apt-get install libglib2.0-dev`

### Permission Issues
- The example writes to `/tmp/io_uring_test.txt`
- Ensure you have write permissions to `/tmp`
