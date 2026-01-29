/*
 * priority_example.c - Source priority examples
 * 
 * Demonstrates how source priorities affect execution order.
 * Lower priority values execute first.
 */

#include <glib.h>

static gint execution_order = 0;

/* High priority timeout */
static gboolean high_priority_callback(gpointer user_data)
{
    execution_order++;
    g_print("[%d] HIGH priority timeout executed\n", execution_order);
    return FALSE;
}

/* Default priority timeout */
static gboolean default_priority_callback(gpointer user_data)
{
    execution_order++;
    g_print("[%d] DEFAULT priority timeout executed\n", execution_order);
    return FALSE;
}

/* Low priority timeout */
static gboolean low_priority_callback(gpointer user_data)
{
    execution_order++;
    g_print("[%d] LOW priority timeout executed\n", execution_order);
    return FALSE;
}

/* Idle with high priority */
static gboolean high_idle_callback(gpointer user_data)
{
    execution_order++;
    g_print("[%d] HIGH_IDLE priority idle executed\n", execution_order);
    return FALSE;
}

/* Idle with default priority */
static gboolean default_idle_callback(gpointer user_data)
{
    execution_order++;
    g_print("[%d] DEFAULT_IDLE priority idle executed\n", execution_order);
    return FALSE;
}

/* Timer to stop the loop */
static gboolean stop_loop(gpointer user_data)
{
    GMainLoop *loop = (GMainLoop *)user_data;
    g_print("\n[Stop] Stopping main loop\n");
    g_main_loop_quit(loop);
    return FALSE;
}

int main(void)
{
    g_print("=== GLib Source Priority Example ===\n\n");
    
    g_print("Priority constants:\n");
    g_print("  G_PRIORITY_HIGH:         %d\n", G_PRIORITY_HIGH);
    g_print("  G_PRIORITY_DEFAULT:      %d\n", G_PRIORITY_DEFAULT);
    g_print("  G_PRIORITY_HIGH_IDLE:    %d\n", G_PRIORITY_HIGH_IDLE);
    g_print("  G_PRIORITY_DEFAULT_IDLE: %d\n", G_PRIORITY_DEFAULT_IDLE);
    g_print("  G_PRIORITY_LOW:          %d\n\n", G_PRIORITY_LOW);
    
    g_print("Lower values = higher priority (execute first)\n\n");
    
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    /* Create timeout sources with different priorities */
    /* All set to trigger at the same time (100ms) */
    
    g_print("Adding timeout sources (all set for 100ms):\n");
    
    /* Low priority */
    GSource *low_source = g_timeout_source_new(100);
    g_source_set_callback(low_source, low_priority_callback, NULL, NULL);
    g_source_set_priority(low_source, G_PRIORITY_LOW);
    g_source_attach(low_source, NULL);  /* NULL = default context */
    g_source_unref(low_source);
    g_print("  - LOW priority timeout added\n");
    
    /* Default priority */
    GSource *default_source = g_timeout_source_new(100);
    g_source_set_callback(default_source, default_priority_callback, NULL, NULL);
    g_source_set_priority(default_source, G_PRIORITY_DEFAULT);
    g_source_attach(default_source, NULL);
    g_source_unref(default_source);
    g_print("  - DEFAULT priority timeout added\n");
    
    /* High priority */
    GSource *high_source = g_timeout_source_new(100);
    g_source_set_callback(high_source, high_priority_callback, NULL, NULL);
    g_source_set_priority(high_source, G_PRIORITY_HIGH);
    g_source_attach(high_source, NULL);
    g_source_unref(high_source);
    g_print("  - HIGH priority timeout added\n");
    
    /* Add idle functions with different priorities */
    g_print("\nAdding idle sources:\n");
    g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, default_idle_callback, NULL, NULL);
    g_print("  - DEFAULT_IDLE priority idle added\n");
    
    g_idle_add_full(G_PRIORITY_HIGH_IDLE, high_idle_callback, NULL, NULL);
    g_print("  - HIGH_IDLE priority idle added\n");
    
    /* Add a timer to stop the loop after everything executes */
    g_timeout_add(500, stop_loop, loop);
    
    g_print("\nStarting main loop...\n");
    g_print("Expected order:\n");
    g_print("  1. HIGH priority timeout\n");
    g_print("  2. DEFAULT priority timeout\n");
    g_print("  3. HIGH_IDLE priority idle\n");
    g_print("  4. DEFAULT_IDLE priority idle\n");
    g_print("  5. LOW priority timeout\n\n");
    g_print("Actual execution order:\n");
    
    /* Run the loop */
    g_main_loop_run(loop);
    
    /* Clean up */
    g_main_loop_unref(loop);
    
    g_print("\n=== Key Takeaways ===\n");
    g_print("- Priorities determine execution order when multiple sources are ready\n");
    g_print("- HIGH priority (%d) executes before DEFAULT (%d)\n", 
            G_PRIORITY_HIGH, G_PRIORITY_DEFAULT);
    g_print("- Timeouts typically execute before idle functions\n");
    g_print("- Use priorities to ensure critical operations execute first\n");
    g_print("- Idle priorities (100+) run when no regular work is pending\n");
    
    return 0;
}
