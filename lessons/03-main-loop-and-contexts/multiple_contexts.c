/*
 * multiple_contexts.c - Working with multiple contexts
 * 
 * Demonstrates creating and using multiple GMainContext objects.
 * This is useful for complex applications that need separate event loops.
 */

#include <glib.h>

typedef struct {
    GMainContext *context;
    const gchar *name;
    gint counter;
} ContextData;

/* Timeout for first context */
static gboolean context1_timeout(gpointer user_data)
{
    ContextData *data = (ContextData *)user_data;
    data->counter++;
    
    g_print("[%s] Timeout fired (count: %d)\n", data->name, data->counter);
    
    if (data->counter >= 3) {
        g_print("[%s] Reached limit\n", data->name);
        return FALSE;
    }
    
    return TRUE;
}

/* Timeout for second context */
static gboolean context2_timeout(gpointer user_data)
{
    ContextData *data = (ContextData *)user_data;
    data->counter++;
    
    g_print("  [%s] Timeout fired (count: %d)\n", data->name, data->counter);
    
    if (data->counter >= 5) {
        g_print("  [%s] Reached limit\n", data->name);
        return FALSE;
    }
    
    return TRUE;
}

int main(void)
{
    g_print("=== Multiple GMainContext Example ===\n\n");
    
    /* Create custom contexts */
    GMainContext *context1 = g_main_context_new();
    GMainContext *context2 = g_main_context_new();
    
    /* Get the default context for comparison */
    GMainContext *default_context = g_main_context_default();
    
    g_print("Created contexts:\n");
    g_print("  Context 1: %p\n", (void *)context1);
    g_print("  Context 2: %p\n", (void *)context2);
    g_print("  Default context: %p\n\n", (void *)default_context);
    
    /* Prepare context data */
    ContextData data1 = { context1, "Context-1", 0 };
    ContextData data2 = { context2, "Context-2", 0 };
    
    /* Add timeout sources to each context */
    GSource *source1 = g_timeout_source_new(1000);  /* 1 second */
    g_source_set_callback(source1, context1_timeout, &data1, NULL);
    g_source_attach(source1, context1);
    g_source_unref(source1);  /* Context holds a reference */
    
    GSource *source2 = g_timeout_source_new(500);   /* 0.5 seconds */
    g_source_set_callback(source2, context2_timeout, &data2, NULL);
    g_source_attach(source2, context2);
    g_source_unref(source2);
    
    g_print("Added timeout sources to contexts\n");
    g_print("  Context 1: 1000ms interval\n");
    g_print("  Context 2: 500ms interval\n\n");
    
    g_print("Manually iterating contexts...\n\n");
    
    /* Manually iterate contexts until both are done */
    gboolean context1_active = TRUE;
    gboolean context2_active = TRUE;
    
    while (context1_active || context2_active) {
        /* Check and iterate context1 */
        if (context1_active) {
            /* Acquire the context to ensure thread safety */
            g_main_context_acquire(context1);
            
            /* Check if any sources are ready */
            if (g_main_context_pending(context1)) {
                /* Dispatch one iteration */
                g_main_context_iteration(context1, FALSE);
            }
            
            /* Check if context still has sources */
            if (data1.counter >= 3) {
                context1_active = FALSE;
            }
            
            g_main_context_release(context1);
        }
        
        /* Check and iterate context2 */
        if (context2_active) {
            g_main_context_acquire(context2);
            
            if (g_main_context_pending(context2)) {
                g_main_context_iteration(context2, FALSE);
            }
            
            if (data2.counter >= 5) {
                context2_active = FALSE;
            }
            
            g_main_context_release(context2);
        }
        
        /* Small sleep to prevent busy-waiting */
        g_usleep(100000);  /* 100ms */
    }
    
    g_print("\n=== Context Information ===\n");
    
    /* Check context ownership */
    g_print("Context 1 owner thread: %p\n", 
            g_main_context_get_thread_default());
    
    /* Clean up contexts */
    g_main_context_unref(context1);
    g_main_context_unref(context2);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Multiple contexts allow independent event loops\n");
    g_print("- Each context has its own set of sources\n");
    g_print("- Contexts can be iterated manually or with GMainLoop\n");
    g_print("- Must acquire context before iteration for thread safety\n");
    g_print("- Useful for complex applications with separate concerns\n");
    
    return 0;
}
