/*
 * basic_types.c - GLib basic types and type system
 * 
 * GLib provides platform-independent type definitions to ensure
 * consistent behavior across different systems.
 */

#include <glib.h>

int main(void)
{
    g_print("=== GLib Basic Types ===\n\n");
    
    /* Integer types with guaranteed sizes */
    gint8 i8 = -128;
    guint8 u8 = 255;
    gint16 i16 = -32768;
    guint16 u16 = 65535;
    gint32 i32 = -2147483648;
    guint32 u32 = 4294967295U;
    gint64 i64 = G_MININT64;
    guint64 u64 = G_MAXUINT64;
    
    g_print("gint8: %d\n", i8);
    g_print("guint8: %u\n", u8);
    g_print("gint16: %d\n", i16);
    g_print("guint16: %u\n", u16);
    g_print("gint32: %d\n", i32);
    g_print("guint32: %u\n", u32);
    g_print("gint64: %" G_GINT64_FORMAT "\n", i64);
    g_print("guint64: %" G_GUINT64_FORMAT "\n", u64);
    
    /* Common types */
    gint regular_int = 42;
    guint unsigned_int = 42;
    gboolean bool_val = TRUE;  /* TRUE and FALSE are GLib constants */
    gchar character = 'A';
    gfloat float_val = 3.14f;
    gdouble double_val = 3.14159265359;
    gpointer generic_ptr = NULL;
    
    g_print("\nCommon types:\n");
    g_print("gint: %d\n", regular_int);
    g_print("guint: %u\n", unsigned_int);
    g_print("gboolean: %s\n", bool_val ? "TRUE" : "FALSE");
    g_print("gchar: %c\n", character);
    g_print("gfloat: %.2f\n", float_val);
    g_print("gdouble: %.10f\n", double_val);
    g_print("gpointer: %p\n", generic_ptr);
    
    /* Size types - platform dependent but guaranteed to hold sizes/indices */
    gsize size = 1024;  /* unsigned integer type for sizes */
    gssize ssize = -1;  /* signed integer type for sizes or errors */
    
    g_print("\nSize types:\n");
    g_print("gsize: %" G_GSIZE_FORMAT "\n", size);
    g_print("gssize: %" G_GSSIZE_FORMAT "\n", ssize);
    
    /* Demonstrating type safety */
    g_print("\n=== Type Limits ===\n");
    g_print("G_MININT8: %d\n", G_MININT8);
    g_print("G_MAXINT8: %d\n", G_MAXINT8);
    g_print("G_MAXUINT8: %u\n", G_MAXUINT8);
    g_print("G_MININT16: %d\n", G_MININT16);
    g_print("G_MAXINT16: %d\n", G_MAXINT16);
    g_print("G_MAXUINT16: %u\n", G_MAXUINT16);
    g_print("G_MININT32: %d\n", G_MININT32);
    g_print("G_MAXINT32: %d\n", G_MAXINT32);
    g_print("G_MAXUINT32: %u\n", G_MAXUINT32);
    
    return 0;
}
