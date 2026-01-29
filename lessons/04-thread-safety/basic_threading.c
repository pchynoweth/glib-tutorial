/*
 * basic_threading.c - Basic thread creation and management
 * 
 * Demonstrates creating threads, passing data to threads,
 * and joining threads.
 */

#include <glib.h>

typedef struct {
    gint thread_id;
    gint iterations;
} ThreadData;

/* Thread function - this runs in a separate thread */
static gpointer thread_function(gpointer user_data)
{
    ThreadData *data = (ThreadData *)user_data;
    
    g_print("[Thread %d] Started\n", data->thread_id);
    
    for (gint i = 0; i < data->iterations; i++) {
        g_print("[Thread %d] Iteration %d/%d\n", 
                data->thread_id, i + 1, data->iterations);
        
        /* Sleep for a bit */
        g_usleep(500000);  /* 500ms */
    }
    
    g_print("[Thread %d] Finished\n", data->thread_id);
    
    /* Return a value */
    return GINT_TO_POINTER(data->thread_id * 100);
}

/* Simple thread function with no data */
static gpointer simple_thread(gpointer user_data)
{
    const gchar *message = (const gchar *)user_data;
    
    for (gint i = 0; i < 3; i++) {
        g_print("[Simple] %s (iteration %d)\n", message, i + 1);
        g_usleep(300000);  /* 300ms */
    }
    
    return NULL;
}

int main(void)
{
    g_print("=== GLib Basic Threading Example ===\n\n");
    
    /* Check if threading is supported (always true in modern GLib) */
    g_print("Threading supported: YES\n\n");
    
    /* Example 1: Create and join a simple thread */
    g_print("1. Creating simple thread:\n");
    GThread *simple = g_thread_new("simple-thread", 
                                   simple_thread, 
                                   "Hello from thread");
    
    /* Do some work in main thread while simple thread runs */
    for (gint i = 0; i < 2; i++) {
        g_print("[Main] Working... (%d)\n", i + 1);
        g_usleep(400000);  /* 400ms */
    }
    
    /* Wait for simple thread to finish */
    g_thread_join(simple);
    g_print("Simple thread joined\n\n");
    
    /* Example 2: Create multiple threads with data */
    g_print("2. Creating multiple threads with data:\n");
    
    #define NUM_THREADS 3
    GThread *threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    /* Create threads */
    for (gint i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].iterations = 3;
        
        gchar *thread_name = g_strdup_printf("worker-%d", i + 1);
        threads[i] = g_thread_new(thread_name, 
                                  thread_function, 
                                  &thread_data[i]);
        g_free(thread_name);
        
        g_print("[Main] Created thread %d\n", i + 1);
    }
    
    /* Wait for all threads to complete */
    g_print("\n[Main] Waiting for threads to complete...\n\n");
    
    for (gint i = 0; i < NUM_THREADS; i++) {
        gpointer result = g_thread_join(threads[i]);
        g_print("[Main] Thread %d joined, returned: %d\n", 
                i + 1, GPOINTER_TO_INT(result));
    }
    
    /* Example 3: Get information about current thread */
    g_print("\n3. Thread information:\n");
    GThread *self = g_thread_self();
    g_print("Main thread pointer: %p\n", (void *)self);
    
    /* Example 4: Yield to other threads */
    g_print("\n4. Thread yielding:\n");
    GThread *yield_thread = g_thread_new("yield-test", simple_thread, "Yield test");
    
    for (gint i = 0; i < 5; i++) {
        g_print("[Main] Before yield %d\n", i + 1);
        g_thread_yield();  /* Hint to scheduler to run other threads */
        g_usleep(200000);
    }
    
    g_thread_join(yield_thread);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Use g_thread_new() to create threads\n");
    g_print("- Use g_thread_join() to wait for thread completion\n");
    g_print("- Thread function receives user_data and returns gpointer\n");
    g_print("- Always join threads unless created as detached\n");
    g_print("- Use g_thread_self() to get current thread\n");
    
    return 0;
}
