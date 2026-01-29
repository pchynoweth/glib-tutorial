/*
 * version_check.c - Check GLib version information
 * 
 * Demonstrates how to check GLib version at compile-time
 * and runtime, which is important for feature compatibility.
 */

#include <glib.h>

int main(void)
{
    g_print("=== GLib Version Information ===\n\n");
    
    /* Compile-time version (what you compiled against) */
    g_print("Compile-time version: %d.%d.%d\n",
            GLIB_MAJOR_VERSION,
            GLIB_MINOR_VERSION,
            GLIB_MICRO_VERSION);
    
    /* Runtime version (what's actually loaded) */
    g_print("Runtime version: %d.%d.%d\n",
            glib_major_version,
            glib_minor_version,
            glib_micro_version);
    
    /* Check if versions match */
    if (glib_major_version == GLIB_MAJOR_VERSION &&
        glib_minor_version == GLIB_MINOR_VERSION &&
        glib_micro_version == GLIB_MICRO_VERSION) {
        g_print("\n✓ Versions match!\n");
    } else {
        g_print("\n⚠ Warning: Compile and runtime versions differ!\n");
    }
    
    /* Check if runtime version is compatible */
    g_print("\nChecking compatibility...\n");
    if (glib_check_version(2, 40, 0)) {
        g_print("GLib 2.40.0 or later: NO\n");
        g_print("Required: %s\n", glib_check_version(2, 40, 0));
    } else {
        g_print("GLib 2.40.0 or later: YES\n");
    }
    
    return 0;
}
