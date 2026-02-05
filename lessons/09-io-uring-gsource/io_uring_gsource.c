/*
 * io_uring_gsource.c - io_uring integration with GLib GSource
 * 
 * Demonstrates how to integrate io_uring with the GLib main loop
 * by creating a custom GSource that monitors io_uring completion events.
 * This example writes buffer contents to a file using io_uring.
 */

#include <glib.h>
#include <liburing.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* Structure to hold io_uring and GSource together */
typedef struct {
    GSource source;
    struct io_uring ring;
    GPollFD poll_fd;
} IoUringSource;

/* Global main loop */
static GMainLoop *main_loop = NULL;

/* Callback when io_uring has completions ready */
static gboolean io_uring_source_prepare(GSource *source, gint *timeout)
{
    /* Never timeout, we rely on poll */
    *timeout = -1;
    return FALSE;
}

static gboolean io_uring_source_check(GSource *source)
{
    IoUringSource *uring_source = (IoUringSource *)source;
    
    /* Check if there are completions ready */
    return (uring_source->poll_fd.revents & G_IO_IN) != 0;
}

static gboolean io_uring_source_dispatch(GSource *source,
                                          GSourceFunc callback,
                                          gpointer user_data)
{
    IoUringSource *uring_source = (IoUringSource *)source;
    struct io_uring_cqe *cqe;
    unsigned head;
    unsigned count = 0;
    
    /* Process all available completions */
    io_uring_for_each_cqe(&uring_source->ring, head, cqe) {
        count++;
        
        if (cqe->res < 0) {
            g_print("[io_uring] Operation failed: %s\n", strerror(-cqe->res));
        } else {
            g_print("[io_uring] Operation completed: %d bytes\n", cqe->res);
        }
        
        /* Get user_data to identify the operation type */
        gpointer op_data = io_uring_cqe_get_data(cqe);
        
        /* Free buffer only if user_data is a heap-allocated buffer.
         * We distinguish between buffer pointers and fd values by checking
         * if the value looks like a small integer (fd) or a heap pointer.
         * This is a simplified check for this example.
         */
        if (op_data && GPOINTER_TO_INT(op_data) > 1024) {
            /* Likely a heap pointer (buffer), free it */
            g_free(op_data);
        }
        /* If op_data is small (< 1024), it's likely an fd value, don't free */
    }
    
    /* Mark all CQEs as seen */
    io_uring_cq_advance(&uring_source->ring, count);
    
    /* Call the user callback if set */
    if (callback) {
        return callback(user_data);
    }
    
    return G_SOURCE_CONTINUE;
}

static void io_uring_source_finalize(GSource *source)
{
    IoUringSource *uring_source = (IoUringSource *)source;
    
    g_print("[Cleanup] Finalizing io_uring source\n");
    io_uring_queue_exit(&uring_source->ring);
}

static GSourceFuncs io_uring_source_funcs = {
    io_uring_source_prepare,
    io_uring_source_check,
    io_uring_source_dispatch,
    io_uring_source_finalize,
    NULL, /* closure_callback */
    NULL  /* closure_marshal */
};

/* Create a new io_uring GSource */
static IoUringSource *io_uring_source_new(void)
{
    IoUringSource *uring_source;
    struct io_uring_params params;
    int ret;
    
    /* Create the GSource */
    uring_source = (IoUringSource *)g_source_new(&io_uring_source_funcs,
                                                  sizeof(IoUringSource));
    
    /* Initialize io_uring with 8 queue entries */
    memset(&params, 0, sizeof(params));
    ret = io_uring_queue_init_params(8, &uring_source->ring, &params);
    if (ret < 0) {
        g_printerr("[Error] Failed to initialize io_uring: %s\n", strerror(-ret));
        g_source_unref((GSource *)uring_source);
        return NULL;
    }
    
    g_print("[Init] io_uring initialized successfully\n");
    
    /* Get the ring file descriptor for polling */
    int ring_fd = uring_source->ring.ring_fd;
    
    /* Set up the poll FD to monitor io_uring completions */
    uring_source->poll_fd.fd = ring_fd;
    uring_source->poll_fd.events = G_IO_IN;
    uring_source->poll_fd.revents = 0;
    
    g_source_add_poll((GSource *)uring_source, &uring_source->poll_fd);
    
    return uring_source;
}

/* Submit a write operation using io_uring */
static gboolean submit_write_operation(IoUringSource *uring_source,
                                        const char *filename,
                                        const char *data,
                                        size_t length)
{
    struct io_uring_sqe *sqe;
    int fd;
    gchar *buffer;
    
    /* Open the file */
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        g_printerr("[Error] Failed to open file: %s\n", strerror(errno));
        return FALSE;
    }
    
    g_print("[Submit] Opening file: %s (fd=%d)\n", filename, fd);
    
    /* Copy data to a buffer that will persist until completion */
    buffer = g_strdup(data);
    
    /* Get a submission queue entry */
    sqe = io_uring_get_sqe(&uring_source->ring);
    if (!sqe) {
        g_printerr("[Error] Failed to get SQE\n");
        close(fd);
        g_free(buffer);
        return FALSE;
    }
    
    /* Prepare the write operation
     * The offset parameter (0) specifies where in the file to write.
     * For sequential writes, use the current file position or track offset.
     */
    io_uring_prep_write(sqe, fd, buffer, length, 0);
    
    /* Set user data to track this operation and the buffer to free */
    io_uring_sqe_set_data(sqe, buffer);
    
    /* Link this write with a close operation so close executes after write.
     * Note: IOSQE_IO_LINK means the next operation only executes if this succeeds.
     * For this simple example, this is acceptable behavior.
     */
    sqe->flags |= IOSQE_IO_LINK;
    
    /* Get another SQE for close operation to clean up the file descriptor */
    sqe = io_uring_get_sqe(&uring_source->ring);
    if (sqe) {
        io_uring_prep_close(sqe, fd);
        /* Mark close with special user_data (cast fd to pointer) to distinguish it */
        io_uring_sqe_set_data(sqe, GINT_TO_POINTER(fd));
    } else {
        /* Fallback: if we can't queue the close, do it synchronously */
        g_print("[Warning] Could not queue close operation, closing synchronously\n");
        close(fd);
    }
    
    g_print("[Submit] Write operation prepared (%zu bytes)\n", length);
    
    /* Submit the operation */
    int submitted = io_uring_submit(&uring_source->ring);
    if (submitted < 0) {
        g_printerr("[Error] Failed to submit: %s\n", strerror(-submitted));
        close(fd);
        g_free(buffer);
        return FALSE;
    }
    
    g_print("[Submit] Submitted %d operation(s)\n", submitted);
    
    return TRUE;
}

/* Structure to track completion state */
typedef struct {
    int expected_completions;
    int completed_count;
    GMainLoop *main_loop;
} CompletionState;

/* Callback for when operations complete */
static gboolean on_operation_complete(gpointer user_data)
{
    CompletionState *state = (CompletionState *)user_data;
    state->completed_count++;
    
    g_print("[Callback] Operation %d/%d completed\n",
            state->completed_count, state->expected_completions);
    
    /* Quit after all expected operations are done */
    if (state->completed_count >= state->expected_completions) {
        g_print("[Callback] All operations completed, quitting main loop\n");
        g_main_loop_quit(state->main_loop);
        return G_SOURCE_REMOVE;
    }
    
    return G_SOURCE_CONTINUE;
}

int main(int argc, char *argv[])
{
    IoUringSource *uring_source;
    GSource *source;
    const gchar *test_content;
    const gchar *filename = "/tmp/io_uring_test.txt";
    CompletionState completion_state;
    
    g_print("=== io_uring GLib GSource Integration ===\n\n");
    
    /* Create the main loop */
    main_loop = g_main_loop_new(NULL, FALSE);
    
    /* Initialize completion state - expecting 2 operations: write + close */
    completion_state.expected_completions = 2;
    completion_state.completed_count = 0;
    completion_state.main_loop = main_loop;
    
    /* Create the io_uring source */
    uring_source = io_uring_source_new();
    if (!uring_source) {
        g_printerr("Failed to create io_uring source\n");
        g_main_loop_unref(main_loop);
        return 1;
    }
    
    source = (GSource *)uring_source;
    
    /* Set the callback for completions, passing our state */
    g_source_set_callback(source, on_operation_complete, &completion_state, NULL);
    
    /* Attach to the default main context */
    g_source_attach(source, NULL);
    
    /* Prepare test content */
    test_content = "Hello from io_uring integrated with GLib!\n"
                   "This demonstrates using io_uring for async I/O\n"
                   "while maintaining compatibility with the GLib main loop.\n"
                   "io_uring provides high-performance async I/O capabilities.\n";
    
    g_print("[Main] Submitting write operation...\n");
    
    /* Submit a write operation */
    if (!submit_write_operation(uring_source, filename,
                                 test_content, strlen(test_content))) {
        g_printerr("Failed to submit write operation\n");
        g_source_unref(source);
        g_main_loop_unref(main_loop);
        return 1;
    }
    
    g_print("[Main] Running main loop, waiting for completions...\n\n");
    
    /* Run the main loop */
    g_main_loop_run(main_loop);
    
    /* Cleanup */
    g_source_unref(source);
    g_main_loop_unref(main_loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- io_uring provides high-performance async I/O\n");
    g_print("- Custom GSource integrates io_uring with GLib main loop\n");
    g_print("- GSource polls io_uring's ring_fd for completions\n");
    g_print("- Operations are submitted via submission queue (SQE)\n");
    g_print("- Completions are retrieved via completion queue (CQE)\n");
    g_print("- File written to: %s\n", filename);
    
    return 0;
}
