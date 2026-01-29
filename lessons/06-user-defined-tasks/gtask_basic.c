/*
 * gtask_basic.c - Basic GTask usage
 * 
 * GTask is the modern GLib API for asynchronous operations.
 * It provides a clean way to run operations in the background
 * and deliver results back to the caller.
 */

#include <glib.h>

/* Simulates a synchronous operation that takes time */
static gint compute_fibonacci(gint n)
{
    if (n <= 1) return n;
    return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
}

/* Task function that runs in a thread pool */
static void fibonacci_task_func(GTask *task,
                                gpointer source_object,
                                gpointer task_data,
                                GCancellable *cancellable)
{
    gint n = GPOINTER_TO_INT(task_data);
    
    g_print("[Task] Computing fibonacci(%d)...\n", n);
    
    /* Check if cancelled before starting */
    if (g_task_return_error_if_cancelled(task)) {
        return;
    }
    
    /* Do the computation */
    gint result = compute_fibonacci(n);
    
    /* Check if cancelled after computation */
    if (g_task_return_error_if_cancelled(task)) {
        return;
    }
    
    g_print("[Task] Result: %d\n", result);
    
    /* Return the result */
    g_task_return_int(task, result);
}

/* Async function that starts the task */
static void compute_fibonacci_async(gint n,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GTask *task = g_task_new(NULL, cancellable, callback, user_data);
    
    /* Store the input parameter */
    g_task_set_task_data(task, GINT_TO_POINTER(n), NULL);
    
    /* Run in thread pool */
    g_task_run_in_thread(task, fibonacci_task_func);
    
    g_object_unref(task);
}

/* Finish function to get the result */
static gint compute_fibonacci_finish(GAsyncResult *result, GError **error)
{
    return g_task_propagate_int(G_TASK(result), error);
}

/* Callback when task completes */
static void on_fibonacci_complete(GObject *source,
                                  GAsyncResult *result,
                                  gpointer user_data)
{
    GMainLoop *loop = (GMainLoop *)user_data;
    GError *error = NULL;
    
    gint fib_result = compute_fibonacci_finish(result, &error);
    
    if (error) {
        g_print("[Callback] Error: %s\n", error->message);
        g_error_free(error);
    } else {
        g_print("[Callback] Received result: %d\n", fib_result);
    }
    
    g_main_loop_quit(loop);
}

/* Example with immediate return */
static void immediate_task_func(GTask *task,
                                gpointer source_object,
                                gpointer task_data,
                                GCancellable *cancellable)
{
    const gchar *message = (const gchar *)task_data;
    g_print("[Immediate Task] %s\n", message);
    g_task_return_pointer(task, g_strdup(message), g_free);
}

static void run_immediate_task_async(const gchar *message,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data)
{
    GTask *task = g_task_new(NULL, NULL, callback, user_data);
    g_task_set_task_data(task, (gpointer)message, NULL);
    g_task_run_in_thread(task, immediate_task_func);
    g_object_unref(task);
}

static gchar *run_immediate_task_finish(GAsyncResult *result, GError **error)
{
    return g_task_propagate_pointer(G_TASK(result), error);
}

static void on_immediate_complete(GObject *source,
                                  GAsyncResult *result,
                                  gpointer user_data)
{
    GError *error = NULL;
    gchar *message = run_immediate_task_finish(result, &error);
    
    if (error) {
        g_print("[Immediate Callback] Error: %s\n", error->message);
        g_error_free(error);
    } else {
        g_print("[Immediate Callback] Got message: %s\n", message);
        g_free(message);
    }
}

int main(void)
{
    g_print("=== GTask Basic Example ===\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    /* Example 1: Simple async computation */
    g_print("1. Starting fibonacci computation for n=35:\n");
    compute_fibonacci_async(35, NULL, on_fibonacci_complete, loop);
    
    /* Example 2: Multiple immediate tasks */
    g_print("\n2. Starting multiple immediate tasks:\n");
    run_immediate_task_async("Task 1", on_immediate_complete, NULL);
    run_immediate_task_async("Task 2", on_immediate_complete, NULL);
    run_immediate_task_async("Task 3", on_immediate_complete, NULL);
    
    g_print("\nMain thread continues while tasks run in background...\n\n");
    
    /* Run the main loop */
    g_main_loop_run(loop);
    
    /* Give other tasks time to complete */
    g_usleep(500000);
    
    g_main_loop_unref(loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- GTask is the modern async API in GLib\n");
    g_print("- g_task_run_in_thread() runs task in thread pool\n");
    g_print("- Task function receives task object and data\n");
    g_print("- Use g_task_return_* to return results\n");
    g_print("- Callback receives GAsyncResult to get result\n");
    g_print("- Follow async/finish pattern for clean API\n");
    
    return 0;
}
