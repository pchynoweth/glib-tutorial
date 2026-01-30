/*
 * debugging_example.c - GLib debugging techniques
 * 
 * Demonstrates debugging facilities provided by GLib including
 * logging, assertions, and environment variables for debugging.
 */

#include <glib.h>

/* Custom log handler */
static void my_log_handler(const gchar *log_domain,
                           GLogLevelFlags log_level,
                           const gchar *message,
                           gpointer user_data)
{
    const gchar *level_str;
    
    switch (log_level & G_LOG_LEVEL_MASK) {
        case G_LOG_LEVEL_ERROR:
            level_str = "ERROR";
            break;
        case G_LOG_LEVEL_CRITICAL:
            level_str = "CRITICAL";
            break;
        case G_LOG_LEVEL_WARNING:
            level_str = "WARNING";
            break;
        case G_LOG_LEVEL_MESSAGE:
            level_str = "MESSAGE";
            break;
        case G_LOG_LEVEL_INFO:
            level_str = "INFO";
            break;
        case G_LOG_LEVEL_DEBUG:
            level_str = "DEBUG";
            break;
        default:
            level_str = "UNKNOWN";
    }
    
    g_print("[%s][%s] %s\n", 
            log_domain ? log_domain : "app",
            level_str, 
            message);
}

/* Function with precondition checks */
static gint divide_numbers(gint a, gint b)
{
    /* Return check - returns on failure */
    g_return_val_if_fail(b != 0, -1);
    
    return a / b;
}

/* Function with assertion */
static void process_data(const gchar *data)
{
    /* Soft check - just returns on failure */
    g_return_if_fail(data != NULL);
    
    g_print("  Processing: %s\n", data);
}

int main(void)
{
    g_print("=== GLib Debugging Example ===\n\n");
    
    /* Example 1: Logging levels */
    g_print("1. Logging Levels:\n\n");
    
    /* Install custom log handler for our domain */
    g_log_set_handler("MyApp", 
                      G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL,
                      my_log_handler, 
                      NULL);
    
    g_log("MyApp", G_LOG_LEVEL_DEBUG, "Debug message");
    g_log("MyApp", G_LOG_LEVEL_INFO, "Info message");
    g_log("MyApp", G_LOG_LEVEL_MESSAGE, "General message");
    g_log("MyApp", G_LOG_LEVEL_WARNING, "Warning message");
    
    /* Convenience macros */
    g_print("\n  Using convenience macros:\n");
    g_message("This is a message");
    g_warning("This is a warning");
    
    /* Example 2: Return checks */
    g_print("\n2. Return Checks (g_return_if_fail):\n\n");
    
    g_print("  divide_numbers(10, 2) = %d\n", divide_numbers(10, 2));
    g_print("  divide_numbers(10, 0) = %d (caught by g_return_val_if_fail)\n", 
            divide_numbers(10, 0));
    
    g_print("\n  Processing valid data:\n");
    process_data("valid data");
    
    g_print("  Processing NULL (caught by g_return_if_fail):\n");
    process_data(NULL);
    g_print("  (function returned early)\n");
    
    /* Example 3: Timer for profiling */
    g_print("\n3. Timer for Profiling:\n\n");
    
    GTimer *timer = g_timer_new();
    
    /* Simulate some work */
    g_timer_start(timer);
    for (gint i = 0; i < 1000000; i++) {
        /* Busy work */
        volatile gint x = i * 2;
        (void)x;
    }
    g_timer_stop(timer);
    
    g_print("  Loop time: %.6f seconds\n", g_timer_elapsed(timer, NULL));
    
    g_timer_start(timer);
    g_usleep(100000);  /* 100ms */
    g_print("  Sleep time: %.6f seconds\n", g_timer_elapsed(timer, NULL));
    
    g_timer_destroy(timer);
    
    /* Example 4: Memory debugging info */
    g_print("\n4. Memory Debugging:\n\n");
    
    g_print("  Environment variables for debugging:\n");
    g_print("    G_DEBUG=gc-friendly  - Zero freed memory\n");
    g_print("    G_DEBUG=fatal-warnings  - Abort on warnings\n");
    g_print("    G_DEBUG=fatal-criticals  - Abort on criticals\n");
    g_print("    G_MESSAGES_DEBUG=all  - Enable debug messages\n");
    
    /* Example 5: Type checking */
    g_print("\n5. Type Information:\n\n");
    
    g_print("  sizeof(gint) = %zu\n", sizeof(gint));
    g_print("  sizeof(glong) = %zu\n", sizeof(glong));
    g_print("  sizeof(gpointer) = %zu\n", sizeof(gpointer));
    g_print("  sizeof(gsize) = %zu\n", sizeof(gsize));
    
    g_print("\n=== Key Points ===\n");
    g_print("- Use g_log() for structured logging\n");
    g_print("- g_return_if_fail() for precondition checks\n");
    g_print("- GTimer for performance measurement\n");
    g_print("- Set G_DEBUG env var for debugging options\n");
    g_print("- G_MESSAGES_DEBUG=all enables g_debug()\n");
    g_print("- Use Valgrind for memory debugging\n");
    
    return 0;
}
