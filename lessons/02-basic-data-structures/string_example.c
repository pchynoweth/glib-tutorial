/*
 * string_example.c - GString operations
 * 
 * GString provides a mutable string buffer with automatic memory management.
 * Much more efficient than repeated string concatenation with standard C strings.
 */

#include <glib.h>

int main(void)
{
    g_print("=== GLib GString Example ===\n");
    
    /* 1. Creating a GString */
    g_print("\n1. Creating GString:\n");
    GString *str = g_string_new("Hello");
    g_print("Initial string: '%s'\n", str->str);
    g_print("Length: %" G_GSIZE_FORMAT "\n", str->len);
    
    /* 2. Appending to string */
    g_print("\n2. Appending:\n");
    g_string_append(str, ", World");
    g_string_append_c(str, '!');
    g_print("After append: '%s'\n", str->str);
    
    /* 3. Appending with printf-style formatting */
    g_string_append_printf(str, " (GLib version %d.%d)", 
                          glib_major_version, 
                          glib_minor_version);
    g_print("After append_printf: '%s'\n", str->str);
    
    /* 4. Prepending */
    g_print("\n3. Prepending:\n");
    g_string_prepend(str, ">>> ");
    g_print("After prepend: '%s'\n", str->str);
    
    /* 5. Inserting at position */
    g_print("\n4. Inserting:\n");
    g_string_insert(str, 4, "INSERTED ");
    g_print("After insert at position 4: '%s'\n", str->str);
    
    /* 6. Erasing part of string */
    g_print("\n5. Erasing:\n");
    g_string_erase(str, 4, 9);  /* Erase 9 chars starting at position 4 */
    g_print("After erase: '%s'\n", str->str);
    
    /* 7. Truncating */
    GString *str2 = g_string_new("This is a long string");
    g_string_truncate(str2, 10);
    g_print("\n6. Truncating:\n");
    g_print("Truncated to 10 chars: '%s'\n", str2->str);
    
    /* 8. Setting string value (replaces content) */
    g_print("\n7. Setting new value:\n");
    g_string_assign(str2, "New value");
    g_print("After assign: '%s'\n", str2->str);
    
    /* 9. String comparison */
    g_print("\n8. Comparison:\n");
    GString *str3 = g_string_new("New value");
    g_print("str2 equals str3: %s\n", 
            g_string_equal(str2, str3) ? "YES" : "NO");
    
    /* 10. Case conversion */
    g_print("\n9. Case conversion:\n");
    GString *str4 = g_string_new("Hello World");
    g_print("Original: '%s'\n", str4->str);
    
    GString *upper = g_string_ascii_up(g_string_new(str4->str));
    g_print("Uppercase: '%s'\n", upper->str);
    
    GString *lower = g_string_ascii_down(g_string_new(str4->str));
    g_print("Lowercase: '%s'\n", lower->str);
    
    /* 11. Building strings efficiently */
    g_print("\n10. Building complex strings:\n");
    GString *builder = g_string_new("");
    
    for (gint i = 1; i <= 5; i++) {
        g_string_append_printf(builder, "Item %d", i);
        if (i < 5) {
            g_string_append(builder, ", ");
        }
    }
    g_print("Built string: '%s'\n", builder->str);
    
    /* 12. Extracting C string and freeing GString */
    g_print("\n11. Extracting C string:\n");
    /* This frees the GString but returns the underlying char* */
    gchar *c_str = g_string_free(builder, FALSE);
    g_print("Extracted C string: '%s'\n", c_str);
    g_free(c_str);  /* Now we own the string memory */
    
    /* 13. Size and capacity */
    g_print("\n12. Size and capacity:\n");
    GString *capacity_test = g_string_sized_new(100);  /* Pre-allocate space */
    g_print("Initial allocated size: %" G_GSIZE_FORMAT "\n", 
            capacity_test->allocated_len);
    g_string_append(capacity_test, "Small string");
    g_print("Length: %" G_GSIZE_FORMAT ", Allocated: %" G_GSIZE_FORMAT "\n",
            capacity_test->len, capacity_test->allocated_len);
    
    /* 14. Creating from static data */
    g_print("\n13. Creating from static data:\n");
    GString *static_str = g_string_new_len("Hello\0World", 11);  /* Include null */
    g_print("Length with embedded null: %" G_GSIZE_FORMAT "\n", static_str->len);
    
    /* Clean up */
    g_string_free(str, TRUE);       /* TRUE means free the string data too */
    g_string_free(str2, TRUE);
    g_string_free(str3, TRUE);
    g_string_free(str4, TRUE);
    g_string_free(upper, TRUE);
    g_string_free(lower, TRUE);
    g_string_free(capacity_test, TRUE);
    g_string_free(static_str, TRUE);
    
    g_print("\n=== Memory cleaned up ===\n");
    
    return 0;
}
