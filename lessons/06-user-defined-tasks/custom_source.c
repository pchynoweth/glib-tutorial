/*
 * custom_source.c - Creating a custom GSource
 * 
 * Demonstrates how to create a custom event source that integrates
 * with the GLib main loop. This example creates a counter source that
 * fires after a specified number of iterations.
 */

#include <glib.h>

/* Custom source structure */
typedef struct {
    GSource parent;
    gint counter;
    gint trigger_value;
} CounterSource;

/* Prepare function - called before polling
 * Returns TRUE if source is ready to dispatch */
static gboolean counter_prepare(GSource *source, gint *timeout)
{
    CounterSource *counter = (CounterSource *)source;
    
    /* No timeout needed - we check immediately */
    *timeout = -1;
    
    /* Ready if counter reached trigger value */
    return counter->counter >= counter->trigger_value;
}

/* Check function - called after polling
 * Returns TRUE if source should be dispatched */
static gboolean counter_check(GSource *source)
{
    CounterSource *counter = (CounterSource *)source;
    
    /* Increment counter on each check */
    counter->counter++;
    
    g_print("[Counter Source] Check called, counter = %d/%d\n",
            counter->counter, counter->trigger_value);
    
    return counter->counter >= counter->trigger_value;
}

/* Dispatch function - called when source is ready
 * Invokes the callback */
static gboolean counter_dispatch(GSource *source,
                                 GSourceFunc callback,
                                 gpointer user_data)
{
    CounterSource *counter = (CounterSource *)source;
    
    g_print("[Counter Source] Dispatched at counter = %d\n", counter->counter);
    
    if (callback) {
        return callback(user_data);
    }
    
    return FALSE;
}

/* Finalize function - cleanup when source is destroyed */
static void counter_finalize(GSource *source)
{
    g_print("[Counter Source] Finalized\n");
}

/* Source function table */
static GSourceFuncs counter_funcs = {
    counter_prepare,
    counter_check,
    counter_dispatch,
    counter_finalize,
    NULL,  /* closure_callback */
    NULL   /* closure_marshal */
};

/* Create a new counter source */
static GSource *counter_source_new(gint trigger_value)
{
    GSource *source = g_source_new(&counter_funcs, sizeof(CounterSource));
    CounterSource *counter = (CounterSource *)source;
    
    counter->counter = 0;
    counter->trigger_value = trigger_value;
    
    return source;
}

/* Callback for counter source */
static gboolean counter_callback(gpointer user_data)
{
    gint id = GPOINTER_TO_INT(user_data);
    g_print("[Callback %d] Counter source triggered!\n\n", id);
    return FALSE;  /* Remove source */
}

/* Simple custom source that's always ready */
typedef struct {
    GSource parent;
    gint call_count;
    gint max_calls;
} SimpleSource;

static gboolean simple_prepare(GSource *source, gint *timeout)
{
    *timeout = 0;  /* Don't wait */
    return TRUE;   /* Always ready */
}

static gboolean simple_check(GSource *source)
{
    return TRUE;  /* Always ready */
}

static gboolean simple_dispatch(GSource *source,
                                GSourceFunc callback,
                                gpointer user_data)
{
    SimpleSource *simple = (SimpleSource *)source;
    simple->call_count++;
    
    g_print("[Simple Source] Dispatch #%d\n", simple->call_count);
    
    if (callback) {
        gboolean continue_source = callback(user_data);
        
        /* Stop after max calls */
        if (simple->call_count >= simple->max_calls) {
            return FALSE;
        }
        
        return continue_source;
    }
    
    return simple->call_count < simple->max_calls;
}

static GSourceFuncs simple_funcs = {
    simple_prepare,
    simple_check,
    simple_dispatch,
    NULL,  /* finalize */
    NULL,  /* closure_callback */
    NULL   /* closure_marshal */
};

static GSource *simple_source_new(gint max_calls)
{
    GSource *source = g_source_new(&simple_funcs, sizeof(SimpleSource));
    SimpleSource *simple = (SimpleSource *)source;
    
    simple->call_count = 0;
    simple->max_calls = max_calls;
    
    return source;
}

static gboolean simple_callback(gpointer user_data)
{
    static gint call_num = 0;
    call_num++;
    
    g_print("[Simple Callback] Called #%d\n", call_num);
    
    return (call_num < 5);  /* Continue for 5 calls */
}

int main(void)
{
    g_print("=== Custom GSource Example ===\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    /* Example 1: Counter source that triggers after N iterations */
    g_print("1. Creating counter source (triggers at 5):\n");
    GSource *counter1 = counter_source_new(5);
    g_source_set_callback(counter1, counter_callback, GINT_TO_POINTER(1), NULL);
    g_source_attach(counter1, NULL);
    g_source_unref(counter1);  /* Main context now owns it */
    
    /* Example 2: Another counter with different trigger */
    g_print("Creating counter source (triggers at 10):\n\n");
    GSource *counter2 = counter_source_new(10);
    g_source_set_callback(counter2, counter_callback, GINT_TO_POINTER(2), NULL);
    g_source_attach(counter2, NULL);
    g_source_unref(counter2);
    
    /* Example 3: Simple always-ready source */
    g_print("2. Creating simple always-ready source (5 calls):\n\n");
    GSource *simple = simple_source_new(5);
    g_source_set_callback(simple, simple_callback, NULL, NULL);
    g_source_set_priority(simple, G_PRIORITY_LOW);  /* Lower priority */
    g_source_attach(simple, NULL);
    g_source_unref(simple);
    
    /* Add a timeout to stop the loop */
    g_timeout_add(2000, (GSourceFunc)g_main_loop_quit, loop);
    
    g_print("3. Running main loop...\n\n");
    g_main_loop_run(loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Custom sources integrate with main loop\n");
    g_print("- Implement prepare, check, dispatch functions\n");
    g_print("- prepare() returns TRUE if ready to dispatch immediately\n");
    g_print("- check() returns TRUE if ready after polling\n");
    g_print("- dispatch() calls the callback function\n");
    g_print("- finalize() cleans up when source is destroyed\n");
    
    g_main_loop_unref(loop);
    
    return 0;
}
