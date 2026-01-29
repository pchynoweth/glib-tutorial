/*
 * simple_loop.c - Basic GMainLoop example
 * 
 * Demonstrates creating and running a simple main loop with a timeout
 * that stops the loop after a few iterations.
 */

#include <glib.h>

/* Global counter */
static gint counter = 0;

/* Timeout callback function - called repeatedly */
static gboolean timeout_callback(gpointer user_data)
{
    GMainLoop *loop = (GMainLoop *)user_data;
    
    counter++;
    g_print("Timeout callback #%d\n", counter);
    
    /* Stop after 5 iterations */
    if (counter >= 5) {
        g_print("Stopping main loop...\n");
        g_main_loop_quit(loop);
        return FALSE;  /* Remove this source */
    }
    
    return TRUE;  /* Continue calling this function */
}

int main(void)
{
    g_print("=== Simple GMainLoop Example ===\n\n");
    
    /* Create a new main loop using the default context */
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    
    g_print("Main loop created\n");
    g_print("Is running: %s\n\n", 
            g_main_loop_is_running(loop) ? "YES" : "NO");
    
    /* Add a timeout source - calls callback every 1000ms (1 second) */
    g_timeout_add(1000, timeout_callback, loop);
    
    g_print("Starting main loop (will run for 5 seconds)...\n\n");
    
    /* Run the loop - this blocks until g_main_loop_quit() is called */
    g_main_loop_run(loop);
    
    g_print("\nMain loop stopped\n");
    g_print("Is running: %s\n", 
            g_main_loop_is_running(loop) ? "YES" : "NO");
    
    /* Clean up */
    g_main_loop_unref(loop);
    
    g_print("\n=== Finished ===\n");
    
    return 0;
}
