/*
 * timeout_example.c - Timeout source examples
 * 
 * Demonstrates different ways to use timeout sources including
 * one-shot timeouts, repeating timeouts, and timeout removal.
 */

#include <glib.h>

typedef struct {
    gint count;
    gint max_count;
    const gchar *name;
} TimeoutData;

/* Repeating timeout */
static gboolean repeating_timeout(gpointer user_data)
{
    TimeoutData *data = (TimeoutData *)user_data;
    data->count++;
    
    g_print("[%s] Count: %d/%d\n", data->name, data->count, data->max_count);
    
    if (data->count >= data->max_count) {
        g_print("[%s] Finished!\n", data->name);
        return FALSE;  /* Stop repeating */
    }
    
    return TRUE;  /* Continue repeating */
}

/* One-shot timeout */
static gboolean oneshot_timeout(gpointer user_data)
{
    g_print("[One-shot] Executed once after delay!\n");
    return FALSE;  /* Don't repeat */
}

/* Fast timeout */
static gboolean fast_timeout(gpointer user_data)
{
    GMainLoop *loop = (GMainLoop *)user_data;
    static gint fast_count = 0;
    
    fast_count++;
    g_print("[Fast] Tick %d\n", fast_count);
    
    if (fast_count >= 10) {
        g_print("[Fast] Done - stopping main loop\n");
        g_main_loop_quit(loop);
        return FALSE;
    }
    
    return TRUE;
}

/* Timeout with seconds precision */
static gboolean seconds_timeout(gpointer user_data)
{
    static gint seconds = 0;
    seconds++;
    g_print("[Seconds] %d second(s) elapsed\n", seconds);
    return TRUE;
}

int main(void)
{
    g_print("=== GLib Timeout Examples ===\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    /* Example 1: One-shot timeout - executes once after 2 seconds */
    g_print("Setting up one-shot timeout (2 seconds)...\n");
    g_timeout_add(2000, oneshot_timeout, NULL);
    
    /* Example 2: Repeating timeout with data */
    TimeoutData slow_data = {
        .count = 0,
        .max_count = 3,
        .name = "Slow"
    };
    g_print("Setting up slow repeating timeout (1.5 seconds interval)...\n");
    g_timeout_add(1500, repeating_timeout, &slow_data);
    
    /* Example 3: Fast repeating timeout */
    g_print("Setting up fast timeout (500ms interval, will stop loop)...\n");
    g_timeout_add(500, fast_timeout, loop);
    
    /* Example 4: Timeout with seconds (g_timeout_add_seconds) */
    /* More efficient than g_timeout_add for second precision */
    g_print("Setting up seconds timeout (1 second interval)...\n");
    guint seconds_id = g_timeout_add_seconds(1, seconds_timeout, NULL);
    
    /* Example 5: Store timeout ID for later removal */
    g_print("\nAll timeouts configured!\n");
    g_print("Starting main loop...\n\n");
    
    /* Run the loop */
    g_main_loop_run(loop);
    
    /* Remove the seconds timeout if still active */
    if (g_source_remove(seconds_id)) {
        g_print("\n[Cleanup] Removed seconds timeout\n");
    }
    
    /* Clean up */
    g_main_loop_unref(loop);
    
    g_print("\n=== Finished ===\n");
    
    return 0;
}
