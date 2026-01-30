/*
 * async_timeout.c - Timeouts and cancellation in async operations
 * 
 * Demonstrates how to implement timeouts for async operations
 * using GCancellable and timers.
 */

#include <glib.h>

static GMainLoop *main_loop = NULL;
static GCancellable *cancellable = NULL;
static guint timeout_source_id = 0;

/* Slow task that might timeout */
static void slow_task_func(GTask *task,
                            gpointer source_object,
                            gpointer task_data,
                            GCancellable *task_cancellable)
{
    gint work_time_ms = GPOINTER_TO_INT(task_data);
    
    g_print("[Task] Starting slow operation (%d ms)...\n", work_time_ms);
    
    /* Check periodically for cancellation */
    gint elapsed = 0;
    gint check_interval = 100;  /* Check every 100ms */
    
    while (elapsed < work_time_ms) {
        if (g_cancellable_is_cancelled(task_cancellable)) {
            g_print("[Task] Cancelled after %d ms\n", elapsed);
            g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_CANCELLED,
                                    "Operation timed out after %d ms", elapsed);
            return;
        }
        
        g_usleep(check_interval * 1000);
        elapsed += check_interval;
    }
    
    g_print("[Task] Completed successfully after %d ms\n", work_time_ms);
    g_task_return_boolean(task, TRUE);
}

/* Timeout callback */
static gboolean on_timeout(gpointer user_data)
{
    gint timeout_ms = GPOINTER_TO_INT(user_data);
    
    g_print("\n[Timeout] %d ms timeout reached - cancelling operation!\n\n", 
            timeout_ms);
    
    g_cancellable_cancel(cancellable);
    timeout_source_id = 0;  /* Mark as removed */
    
    return G_SOURCE_REMOVE;  /* Don't repeat */
}

/* Task completion callback */
static void on_task_complete(GObject *source,
                              GAsyncResult *result,
                              gpointer user_data)
{
    const gchar *label = (const gchar *)user_data;
    GError *error = NULL;
    
    /* Remove timeout if it hasn't fired yet */
    if (timeout_source_id != 0) {
        g_source_remove(timeout_source_id);
        timeout_source_id = 0;
    }
    
    gboolean success = g_task_propagate_boolean(G_TASK(result), &error);
    
    if (error) {
        if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
            g_print("[Callback] %s: Operation timed out!\n", label);
        } else {
            g_print("[Callback] %s: Error: %s\n", label, error->message);
        }
        g_error_free(error);
    } else {
        g_print("[Callback] %s: Operation completed successfully!\n", label);
    }
    
    g_main_loop_quit(main_loop);
}

/* Start a task with timeout */
static void run_with_timeout(gint work_time_ms, gint timeout_ms, const gchar *label)
{
    g_print("\n=== %s ===\n", label);
    g_print("Work time: %d ms, Timeout: %d ms\n\n", work_time_ms, timeout_ms);
    
    /* Reset cancellable */
    if (cancellable) {
        g_object_unref(cancellable);
    }
    cancellable = g_cancellable_new();
    
    /* Set up timeout and store the source ID */
    timeout_source_id = g_timeout_add(timeout_ms, on_timeout, GINT_TO_POINTER(timeout_ms));
    
    /* Start the task */
    GTask *task = g_task_new(NULL, cancellable, on_task_complete, (gpointer)label);
    g_task_set_task_data(task, GINT_TO_POINTER(work_time_ms), NULL);
    g_task_run_in_thread(task, slow_task_func);
    g_object_unref(task);
    
    g_main_loop_run(main_loop);
}

int main(void)
{
    g_print("=== Async Timeout Example ===\n");
    
    main_loop = g_main_loop_new(NULL, FALSE);
    
    /* Example 1: Task completes before timeout */
    run_with_timeout(500, 1000, "Fast Task (should succeed)");
    
    /* Small delay between examples */
    g_usleep(100000);
    
    /* Example 2: Task times out */
    run_with_timeout(2000, 500, "Slow Task (should timeout)");
    
    /* Cleanup */
    if (cancellable) {
        g_object_unref(cancellable);
    }
    g_main_loop_unref(main_loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Use GCancellable for cancellation support\n");
    g_print("- Set up timeout with g_timeout_add()\n");
    g_print("- Store timeout source ID and remove with g_source_remove()\n");
    g_print("- Check g_cancellable_is_cancelled() in task loop\n");
    g_print("- Cancel triggers G_IO_ERROR_CANCELLED\n");
    g_print("- Check frequently for responsive cancellation\n");
    
    return 0;
}
