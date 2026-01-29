/*
 * hash_table_example.c - Hash table operations
 * 
 * GHashTable provides a hash table implementation with automatic
 * memory management and collision handling.
 */

#include <glib.h>

/* Function to print key-value pairs */
void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
    g_print("  %s => %s\n", (gchar *)key, (gchar *)value);
}

/* Function to check a condition */
gboolean key_starts_with_a(gpointer key, gpointer value, gpointer user_data)
{
    return g_str_has_prefix((gchar *)key, "a");
}

int main(void)
{
    g_print("=== GLib GHashTable Example ===\n");
    
    /* 1. Create a hash table with string keys and values */
    /* g_str_hash and g_str_equal are built-in hash/compare functions for strings */
    /* g_free will be called on both keys and values when entries are removed */
    GHashTable *hash = g_hash_table_new_full(g_str_hash,
                                              g_str_equal,
                                              g_free,      /* key destroy func */
                                              g_free);     /* value destroy func */
    
    g_print("\n1. Creating and populating hash table:\n");
    
    /* 2. Inserting key-value pairs */
    /* g_strdup duplicates the string so we own the memory */
    g_hash_table_insert(hash, g_strdup("name"), g_strdup("Alice"));
    g_hash_table_insert(hash, g_strdup("city"), g_strdup("New York"));
    g_hash_table_insert(hash, g_strdup("country"), g_strdup("USA"));
    g_hash_table_insert(hash, g_strdup("occupation"), g_strdup("Engineer"));
    
    /* 3. Getting the size */
    g_print("Hash table size: %u\n", g_hash_table_size(hash));
    
    /* 4. Looking up values */
    g_print("\n2. Looking up values:\n");
    const gchar *name = g_hash_table_lookup(hash, "name");
    const gchar *city = g_hash_table_lookup(hash, "city");
    g_print("Name: %s\n", name ? name : "Not found");
    g_print("City: %s\n", city ? city : "Not found");
    
    /* 5. Checking if key exists */
    g_print("\n3. Checking key existence:\n");
    g_print("Has 'name' key: %s\n", 
            g_hash_table_contains(hash, "name") ? "YES" : "NO");
    g_print("Has 'age' key: %s\n", 
            g_hash_table_contains(hash, "age") ? "YES" : "NO");
    
    /* 6. Replacing a value */
    g_print("\n4. Replacing value:\n");
    g_hash_table_replace(hash, g_strdup("city"), g_strdup("San Francisco"));
    g_print("New city: %s\n", (gchar *)g_hash_table_lookup(hash, "city"));
    
    /* 7. Iterating over all entries */
    g_print("\n5. All entries:\n");
    g_hash_table_foreach(hash, print_key_value, NULL);
    
    /* 8. Getting keys and values as lists */
    g_print("\n6. Getting all keys:\n");
    GList *keys = g_hash_table_get_keys(hash);
    for (GList *l = keys; l != NULL; l = l->next) {
        g_print("  Key: %s\n", (gchar *)l->data);
    }
    g_list_free(keys);  /* Free the list, not the keys themselves */
    
    /* 9. Adding more entries for removal demo */
    g_hash_table_insert(hash, g_strdup("age"), g_strdup("30"));
    g_hash_table_insert(hash, g_strdup("active"), g_strdup("true"));
    
    /* 10. Removing an entry */
    g_print("\n7. Removing 'age' key:\n");
    gboolean removed = g_hash_table_remove(hash, "age");
    g_print("Removal %s\n", removed ? "successful" : "failed");
    g_print("Size after removal: %u\n", g_hash_table_size(hash));
    
    /* 11. Conditional removal */
    g_print("\n8. Removing keys starting with 'a':\n");
    guint removed_count = g_hash_table_foreach_remove(hash, 
                                                       key_starts_with_a, 
                                                       NULL);
    g_print("Removed %u entries\n", removed_count);
    g_print("Size after conditional removal: %u\n", g_hash_table_size(hash));
    
    /* 12. Looking up with extended info */
    g_print("\n9. Extended lookup:\n");
    gpointer orig_key, value;
    if (g_hash_table_lookup_extended(hash, "name", &orig_key, &value)) {
        g_print("Found - Key: %s, Value: %s\n", 
                (gchar *)orig_key, (gchar *)value);
    } else {
        g_print("Key 'name' not found\n");
    }
    
    /* 13. Creating a hash table with direct keys (integers) */
    g_print("\n10. Integer hash table:\n");
    GHashTable *int_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
    
    g_hash_table_insert(int_hash, GINT_TO_POINTER(1), GINT_TO_POINTER(100));
    g_hash_table_insert(int_hash, GINT_TO_POINTER(2), GINT_TO_POINTER(200));
    g_hash_table_insert(int_hash, GINT_TO_POINTER(3), GINT_TO_POINTER(300));
    
    gint value_for_key_2 = GPOINTER_TO_INT(
        g_hash_table_lookup(int_hash, GINT_TO_POINTER(2))
    );
    g_print("Value for key 2: %d\n", value_for_key_2);
    
    /* Clean up */
    g_hash_table_destroy(hash);      /* Calls g_free on all keys and values */
    g_hash_table_destroy(int_hash);
    
    g_print("\n=== Memory cleaned up ===\n");
    
    return 0;
}
