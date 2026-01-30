/*
 * error_handling.c - Error propagation in async contexts
 * 
 * Demonstrates proper error handling patterns for
 * asynchronous operations in GLib.
 */

#include <glib.h>

/* Define a custom error domain for our application */
#define MY_APP_ERROR (my_app_error_quark())

typedef enum {
    MY_APP_ERROR_INVALID_INPUT,
    MY_APP_ERROR_PROCESSING_FAILED,
    MY_APP_ERROR_RESOURCE_NOT_FOUND
} MyAppError;

static GQuark my_app_error_quark(void)
{
    return g_quark_from_static_string("my-app-error");
}

static GMainLoop *main_loop = NULL;
static gint completed_tasks = 0;
static gint total_tasks = 0;

/* Task that might fail */
static void maybe_fail_task_func(GTask *task,
                                  gpointer source_object,
                                  gpointer task_data,
                                  GCancellable *cancellable)
{
    gint scenario = GPOINTER_TO_INT(task_data);
    
    g_print("[Task %d] Processing...\n", scenario);
    g_usleep(300000);  /* Simulate work */
    
    switch (scenario) {
        case 1:
            /* Success case */
            g_print("[Task 1] Completed successfully\n");
            g_task_return_int(task, 42);
            break;
            
        case 2:
            /* Invalid input error */
            g_print("[Task 2] Invalid input detected\n");
            g_task_return_new_error(task,
                                    MY_APP_ERROR,
                                    MY_APP_ERROR_INVALID_INPUT,
                                    "Input validation failed: negative value");
            break;
            
        case 3:
            /* Processing error */
            g_print("[Task 3] Processing failed\n");
            g_task_return_new_error(task,
                                    MY_APP_ERROR,
                                    MY_APP_ERROR_PROCESSING_FAILED,
                                    "Processing error: division by zero");
            break;
            
        case 4:
            /* Resource not found */
            g_print("[Task 4] Resource not found\n");
            g_task_return_new_error(task,
                                    MY_APP_ERROR,
                                    MY_APP_ERROR_RESOURCE_NOT_FOUND,
                                    "Resource 'config.json' not found");
            break;
            
        case 5:
            /* GIO error */
            g_print("[Task 5] I/O operation failed\n");
            g_task_return_new_error(task,
                                    G_IO_ERROR,
                                    G_IO_ERROR_PERMISSION_DENIED,
                                    "Permission denied accessing file");
            break;
            
        default:
            g_task_return_int(task, 0);
    }
}

/* Callback with error handling */
static void on_task_complete(GObject *source,
                              GAsyncResult *result,
                              gpointer user_data)
{
    gint scenario = GPOINTER_TO_INT(user_data);
    GError *error = NULL;
    
    gint value = g_task_propagate_int(G_TASK(result), &error);
    
    g_print("\n[Callback %d] Result:\n", scenario);
    
    if (error) {
        /* Handle different error types */
        if (g_error_matches(error, MY_APP_ERROR, MY_APP_ERROR_INVALID_INPUT)) {
            g_print("  -> Input Error: %s\n", error->message);
            g_print("  -> Action: Validate and retry with corrected input\n");
        } else if (g_error_matches(error, MY_APP_ERROR, MY_APP_ERROR_PROCESSING_FAILED)) {
            g_print("  -> Processing Error: %s\n", error->message);
            g_print("  -> Action: Log error and notify user\n");
        } else if (g_error_matches(error, MY_APP_ERROR, MY_APP_ERROR_RESOURCE_NOT_FOUND)) {
            g_print("  -> Resource Error: %s\n", error->message);
            g_print("  -> Action: Use default resource or create new\n");
        } else if (error->domain == G_IO_ERROR) {
            g_print("  -> I/O Error (%d): %s\n", error->code, error->message);
            g_print("  -> Action: Check permissions and retry\n");
        } else {
            g_print("  -> Unknown Error: %s\n", error->message);
        }
        
        g_error_free(error);
    } else {
        g_print("  -> Success! Value: %d\n", value);
    }
    
    completed_tasks++;
    if (completed_tasks >= total_tasks) {
        g_main_loop_quit(main_loop);
    }
}

/* Start an async task */
static void start_task(gint scenario)
{
    GTask *task = g_task_new(NULL, NULL, on_task_complete, 
                              GINT_TO_POINTER(scenario));
    g_task_set_task_data(task, GINT_TO_POINTER(scenario), NULL);
    g_task_run_in_thread(task, maybe_fail_task_func);
    g_object_unref(task);
}

int main(void)
{
    g_print("=== Async Error Handling Example ===\n\n");
    
    main_loop = g_main_loop_new(NULL, FALSE);
    
    total_tasks = 5;
    
    /* Start tasks with different outcomes */
    g_print("Starting %d tasks with different error scenarios...\n\n", total_tasks);
    
    for (gint i = 1; i <= total_tasks; i++) {
        start_task(i);
    }
    
    g_main_loop_run(main_loop);
    
    g_main_loop_unref(main_loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Define custom error domains with GQuark\n");
    g_print("- Use g_task_return_new_error() for failures\n");
    g_print("- Check g_error_matches() for specific errors\n");
    g_print("- Always free GError with g_error_free()\n");
    g_print("- Handle different error types appropriately\n");
    g_print("- Use standard G_IO_ERROR for I/O operations\n");
    
    return 0;
}
