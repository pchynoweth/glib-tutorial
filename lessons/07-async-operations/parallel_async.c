/*
 * parallel_async.c - Running multiple async operations in parallel
 * 
 * Demonstrates how to run multiple async operations concurrently
 * and wait for all of them to complete.
 */

#include <glib.h>

/* Track completion of multiple operations */
typedef struct {
    gint total_operations;
    gint completed_operations;
    GMainLoop *loop;
    GMutex mutex;
} ParallelContext;

/* Simulated async work function */
static void work_task_func(GTask *task,
                           gpointer source_object,
                           gpointer task_data,
                           GCancellable *cancellable)
{
    gint task_id = GPOINTER_TO_INT(task_data);
    
    /* Simulate variable work time */
    gint work_time = 500 + (task_id * 200);
    g_print("[Task %d] Starting work (%dms)...\n", task_id, work_time);
    
    g_usleep(work_time * 1000);
    
    if (g_cancellable_is_cancelled(cancellable)) {
        g_task_return_new_error(task, G_IO_ERROR, G_IO_ERROR_CANCELLED,
                                "Task %d cancelled", task_id);
        return;
    }
    
    gchar *result = g_strdup_printf("Result from task %d", task_id);
    g_print("[Task %d] Completed!\n", task_id);
    
    g_task_return_pointer(task, result, g_free);
}

/* Callback when one task completes */
static void on_task_complete(GObject *source,
                              GAsyncResult *result,
                              gpointer user_data)
{
    ParallelContext *ctx = (ParallelContext *)user_data;
    GError *error = NULL;
    
    gchar *task_result = g_task_propagate_pointer(G_TASK(result), &error);
    
    if (error) {
        g_print("[Callback] Error: %s\n", error->message);
        g_error_free(error);
    } else {
        g_print("[Callback] Received: %s\n", task_result);
        g_free(task_result);
    }
    
    /* Track completion */
    g_mutex_lock(&ctx->mutex);
    ctx->completed_operations++;
    
    g_print("[Progress] %d/%d operations complete\n",
            ctx->completed_operations, ctx->total_operations);
    
    /* Check if all operations are done */
    if (ctx->completed_operations >= ctx->total_operations) {
        g_print("\n[Main] All operations completed!\n");
        g_main_loop_quit(ctx->loop);
    }
    
    g_mutex_unlock(&ctx->mutex);
}

/* Start an async task */
static void start_async_task(gint task_id,
                              GCancellable *cancellable,
                              ParallelContext *ctx)
{
    GTask *task = g_task_new(NULL, cancellable, on_task_complete, ctx);
    g_task_set_task_data(task, GINT_TO_POINTER(task_id), NULL);
    g_task_run_in_thread(task, work_task_func);
    g_object_unref(task);
}

int main(void)
{
    g_print("=== Parallel Async Operations Example ===\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GCancellable *cancellable = g_cancellable_new();
    
    ParallelContext ctx = {
        .total_operations = 5,
        .completed_operations = 0,
        .loop = loop
    };
    g_mutex_init(&ctx.mutex);
    
    g_print("Starting %d parallel async operations...\n\n", ctx.total_operations);
    
    /* Start all operations at once - they run in parallel */
    for (gint i = 1; i <= ctx.total_operations; i++) {
        start_async_task(i, cancellable, &ctx);
    }
    
    g_print("[Main] All tasks started, waiting for completion...\n\n");
    
    g_main_loop_run(loop);
    
    /* Cleanup */
    g_mutex_clear(&ctx.mutex);
    g_object_unref(cancellable);
    g_main_loop_unref(loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Start multiple async operations without waiting\n");
    g_print("- Track completion count in callbacks\n");
    g_print("- Use mutex for thread-safe counting\n");
    g_print("- Quit main loop when all are complete\n");
    g_print("- All operations share same cancellable\n");
    
    return 0;
}
