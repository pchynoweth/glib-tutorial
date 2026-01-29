/*
 * context_threading.c - Main context usage across threads
 * 
 * Demonstrates how to properly use GMainContext across threads,
 * including pushing context as thread-default and scheduling
 * callbacks from worker threads to main thread.
 */

#include <glib.h>

static GMainLoop *main_loop = NULL;
static gint work_counter = 0;

/* Idle callback executed in main thread */
static gboolean update_from_worker(gpointer user_data)
{
    gint worker_id = GPOINTER_TO_INT(user_data);
    
    g_print("[Main Thread] Update from worker %d (counter: %d)\n", 
            worker_id, ++work_counter);
    
    if (work_counter >= 10) {
        g_print("[Main Thread] All work done, stopping loop\n");
        g_main_loop_quit(main_loop);
    }
    
    return FALSE;  /* One-shot */
}

/* Worker thread that schedules callbacks in main thread */
static gpointer worker_thread(gpointer user_data)
{
    gint worker_id = GPOINTER_TO_INT(user_data);
    
    g_print("[Worker %d] Started\n", worker_id);
    
    for (gint i = 0; i < 5; i++) {
        g_print("[Worker %d] Doing work iteration %d\n", worker_id, i + 1);
        
        /* Simulate work */
        g_usleep(500000);  /* 500ms */
        
        /* Schedule update in main thread using g_idle_add */
        /* This is thread-safe and adds to default context */
        g_idle_add(update_from_worker, GINT_TO_POINTER(worker_id));
    }
    
    g_print("[Worker %d] Finished\n", worker_id);
    return NULL;
}

/* Thread with its own context */
static gpointer context_owner_thread(gpointer user_data)
{
    g_print("[Context Thread] Starting with own context\n");
    
    /* Create a new context for this thread */
    GMainContext *context = g_main_context_new();
    GMainLoop *loop = g_main_loop_new(context, FALSE);
    
    /* Push this context as the thread-default */
    g_main_context_push_thread_default(context);
    
    g_print("[Context Thread] Context pushed as thread-default\n");
    
    /* Verify it's the thread-default */
    GMainContext *thread_default = g_main_context_get_thread_default();
    g_print("[Context Thread] Thread-default context: %p\n", 
            (void *)thread_default);
    g_print("[Context Thread] Our context: %p\n", (void *)context);
    g_print("[Context Thread] Match: %s\n",
            (thread_default == context) ? "YES" : "NO");
    
    /* Add a timeout to this thread's context */
    static gint timeout_count = 0;
    
    GSource *timeout = g_timeout_source_new(500);
    g_source_set_callback(timeout, 
        (GSourceFunc)(gboolean (*)(void))({
            gboolean callback(void) {
                timeout_count++;
                g_print("[Context Thread] Timeout fired (%d)\n", timeout_count);
                if (timeout_count >= 3) {
                    g_main_loop_quit(loop);
                    return FALSE;
                }
                return TRUE;
            }
            callback;
        }), 
        NULL, NULL);
    g_source_attach(timeout, context);
    g_source_unref(timeout);
    
    /* Run this thread's event loop */
    g_print("[Context Thread] Running event loop...\n");
    g_main_loop_run(loop);
    
    /* Clean up */
    g_main_context_pop_thread_default(context);
    g_main_loop_unref(loop);
    g_main_context_unref(context);
    
    g_print("[Context Thread] Finished\n");
    return NULL;
}

int main(void)
{
    g_print("=== Main Context Threading Example ===\n\n");
    
    /* Get default context info */
    GMainContext *default_context = g_main_context_default();
    g_print("Default context: %p\n", (void *)default_context);
    g_print("Thread-default context: %p\n", 
            (void *)g_main_context_get_thread_default());
    g_print("(NULL means use default)\n\n");
    
    /* Create main loop for main thread */
    main_loop = g_main_loop_new(NULL, FALSE);
    
    /* Example 1: Workers scheduling callbacks in main thread */
    g_print("1. Starting worker threads that update main thread:\n\n");
    
    GThread *worker1 = g_thread_new("worker1", worker_thread, 
                                    GINT_TO_POINTER(1));
    GThread *worker2 = g_thread_new("worker2", worker_thread,
                                    GINT_TO_POINTER(2));
    
    /* Run main loop - will process callbacks from workers */
    g_print("[Main Thread] Running main loop...\n\n");
    g_main_loop_run(main_loop);
    
    /* Wait for workers */
    g_thread_join(worker1);
    g_thread_join(worker2);
    
    g_print("\n2. Thread with own context:\n\n");
    
    /* Example 2: Thread with its own context */
    GThread *context_thread = g_thread_new("context-owner",
                                           context_owner_thread,
                                           NULL);
    g_thread_join(context_thread);
    
    /* Clean up */
    g_main_loop_unref(main_loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Use g_idle_add() to schedule callbacks from worker to main thread\n");
    g_print("- g_idle_add() is thread-safe for default context\n");
    g_print("- Each thread can have its own GMainContext\n");
    g_print("- Use g_main_context_push_thread_default() for thread-local context\n");
    g_print("- Context pushed is used by g_idle_add() called from that thread\n");
    g_print("- Don't share context between threads without synchronization\n");
    
    return 0;
}
