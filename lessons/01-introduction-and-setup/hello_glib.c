/*
 * hello_glib.c - Your first GLib program
 * 
 * This program demonstrates the basics of using GLib:
 * - Including the GLib header
 * - Using GLib's print function
 * - Using GLib's string type
 * - Proper memory cleanup
 */

#include <glib.h>

int main(int argc, char *argv[])
{
    /* GLib strings are UTF-8 encoded and use gchar* type */
    gchar *message = g_strdup("Hello, GLib!");
    
    /* g_print is GLib's printing function - similar to printf */
    g_print("%s\n", message);
    g_print("This is your first GLib program!\n");
    
    /* Always free memory allocated with GLib functions */
    g_free(message);
    
    /* Create a formatted string */
    gchar *formatted = g_strdup_printf("GLib version: %d.%d.%d",
                                       glib_major_version,
                                       glib_minor_version,
                                       glib_micro_version);
    g_print("%s\n", formatted);
    g_free(formatted);
    
    return 0;
}
