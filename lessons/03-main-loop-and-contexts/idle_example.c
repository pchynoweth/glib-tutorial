/*
 * idle_example.c - Idle source examples
 * 
 * Idle functions run when no higher priority sources are ready.
 * They're useful for background tasks and deferred work.
 */

#include <glib.h>

static gint idle_count = 0;
static gint work_items = 0;

/* Idle function - runs when main loop has nothing else to do */
static gboolean idle_function(gpointer user_data)
{
    idle_count++;
    g_print("[Idle] Background work iteration %d\n", idle_count);
    
    /* Simulate some work */
    for (volatile gint i = 0; i < 1000000; i++);
    
    if (idle_count >= 5) {
        g_print("[Idle] Background work complete\n");
        return FALSE;  /* Remove idle source */
    }
    
    return TRUE;  /* Keep running */
}

/* High priority idle */
static gboolean high_priority_idle(gpointer user_data)
{
    static gint count = 0;
    count++;
    g_print("[High Priority Idle] Iteration %d\n", count);
    
    if (count >= 3) {
        return FALSE;
    }
    
    return TRUE;
}

/* Work processing function */
static gboolean process_work(gpointer user_data)
{
    work_items++;
    g_print("[Work] Processing work item %d\n", work_items);
    
    if (work_items >= 8) {
        GMainLoop *loop = (GMainLoop *)user_data;
        g_print("[Work] All work items processed, stopping loop\n");
        g_main_loop_quit(loop);
        return FALSE;
    }
    
    return TRUE;
}

/* One-shot idle - runs once when idle */
static gboolean oneshot_idle(gpointer user_data)
{
    g_print("[One-shot Idle] This runs once when the loop becomes idle\n");
    return FALSE;  /* Don't repeat */
}

/* Demonstrates g_idle_add vs g_idle_add_full with priority */
int main(void)
{
    g_print("=== GLib Idle Source Examples ===\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    /* Add an idle function with default priority */
    g_print("Adding default priority idle function...\n");
    g_idle_add(idle_function, NULL);
    
    /* Add a high priority idle function */
    g_print("Adding high priority idle function...\n");
    g_idle_add_full(G_PRIORITY_HIGH_IDLE, high_priority_idle, NULL, NULL);
    
    /* Add a one-shot idle */
    g_print("Adding one-shot idle...\n");
    g_idle_add(oneshot_idle, NULL);
    
    /* Add a timeout to generate some work */
    /* This will run periodically, with idle functions filling the gaps */
    g_print("Adding work generator timeout (200ms)...\n");
    g_timeout_add(200, process_work, loop);
    
    g_print("\nStarting main loop...\n");
    g_print("Note: Idle functions run between timeout callbacks\n\n");
    
    /* Run the loop */
    g_main_loop_run(loop);
    
    /* Clean up */
    g_main_loop_unref(loop);
    
    g_print("\n=== Key Takeaways ===\n");
    g_print("- Idle functions run when no higher priority work is pending\n");
    g_print("- Higher priority idle functions run before lower priority ones\n");
    g_print("- Useful for background processing without blocking events\n");
    g_print("- Return FALSE to remove the idle source\n");
    
    return 0;
}
