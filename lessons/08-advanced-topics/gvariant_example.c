/*
 * gvariant_example.c - GVariant for serialization
 * 
 * Demonstrates using GVariant for type-safe data serialization,
 * useful for IPC, configuration files, and data exchange.
 */

#include <glib.h>

/* Print a GVariant with indentation */
static void print_variant(GVariant *variant, gint indent)
{
    gchar *type_str = g_variant_print(variant, TRUE);
    g_print("%*s%s\n", indent, "", type_str);
    g_free(type_str);
}

int main(void)
{
    g_print("=== GVariant Serialization Example ===\n\n");
    
    /* Example 1: Basic types */
    g_print("1. Basic Types:\n\n");
    
    GVariant *int_var = g_variant_new_int32(42);
    GVariant *str_var = g_variant_new_string("Hello, GVariant!");
    GVariant *bool_var = g_variant_new_boolean(TRUE);
    GVariant *double_var = g_variant_new_double(3.14159);
    
    g_print("  Integer: ");
    print_variant(int_var, 0);
    g_print("  String:  ");
    print_variant(str_var, 0);
    g_print("  Boolean: ");
    print_variant(bool_var, 0);
    g_print("  Double:  ");
    print_variant(double_var, 0);
    
    /* Read values back */
    g_print("\n  Values: int=%d, str='%s', bool=%s, double=%.2f\n",
            g_variant_get_int32(int_var),
            g_variant_get_string(str_var, NULL),
            g_variant_get_boolean(bool_var) ? "true" : "false",
            g_variant_get_double(double_var));
    
    g_variant_unref(int_var);
    g_variant_unref(str_var);
    g_variant_unref(bool_var);
    g_variant_unref(double_var);
    
    /* Example 2: Tuples */
    g_print("\n2. Tuples (multiple types together):\n\n");
    
    GVariant *tuple = g_variant_new("(sidb)", 
                                     "Name", 
                                     42, 
                                     3.14, 
                                     TRUE);
    
    g_print("  Tuple: ");
    print_variant(tuple, 0);
    
    const gchar *name;
    gint id;
    gdouble value;
    gboolean active;
    
    g_variant_get(tuple, "(sidb)", &name, &id, &value, &active);
    g_print("  Unpacked: name='%s', id=%d, value=%.2f, active=%s\n",
            name, id, value, active ? "yes" : "no");
    
    g_variant_unref(tuple);
    
    /* Example 3: Arrays */
    g_print("\n3. Arrays:\n\n");
    
    GVariantBuilder builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE("ai"));
    
    for (gint i = 1; i <= 5; i++) {
        g_variant_builder_add(&builder, "i", i * 10);
    }
    
    GVariant *array = g_variant_builder_end(&builder);
    
    g_print("  Array: ");
    print_variant(array, 0);
    
    /* Iterate array */
    g_print("  Elements: ");
    GVariantIter iter;
    gint elem;
    g_variant_iter_init(&iter, array);
    while (g_variant_iter_next(&iter, "i", &elem)) {
        g_print("%d ", elem);
    }
    g_print("\n");
    
    g_variant_unref(array);
    
    /* Example 4: Dictionaries */
    g_print("\n4. Dictionaries:\n\n");
    
    g_variant_builder_init(&builder, G_VARIANT_TYPE("a{sv}"));
    
    g_variant_builder_add(&builder, "{sv}", "username",
                          g_variant_new_string("john_doe"));
    g_variant_builder_add(&builder, "{sv}", "age",
                          g_variant_new_int32(30));
    g_variant_builder_add(&builder, "{sv}", "premium",
                          g_variant_new_boolean(TRUE));
    g_variant_builder_add(&builder, "{sv}", "balance",
                          g_variant_new_double(1234.56));
    
    GVariant *dict = g_variant_builder_end(&builder);
    
    g_print("  Dictionary: ");
    print_variant(dict, 0);
    
    /* Lookup values */
    GVariant *lookup = g_variant_lookup_value(dict, "username", 
                                               G_VARIANT_TYPE_STRING);
    if (lookup) {
        g_print("  username: %s\n", g_variant_get_string(lookup, NULL));
        g_variant_unref(lookup);
    }
    
    gint32 age;
    if (g_variant_lookup(dict, "age", "i", &age)) {
        g_print("  age: %d\n", age);
    }
    
    g_variant_unref(dict);
    
    /* Example 5: Nested structures */
    g_print("\n5. Nested Structures:\n\n");
    
    g_variant_builder_init(&builder, G_VARIANT_TYPE("a(si)"));
    g_variant_builder_add(&builder, "(si)", "Alice", 95);
    g_variant_builder_add(&builder, "(si)", "Bob", 87);
    g_variant_builder_add(&builder, "(si)", "Charlie", 92);
    
    GVariant *scores = g_variant_builder_end(&builder);
    
    g_print("  Student scores: ");
    print_variant(scores, 0);
    
    /* Iterate */
    const gchar *student;
    gint score;
    g_variant_iter_init(&iter, scores);
    g_print("  Entries:\n");
    while (g_variant_iter_next(&iter, "(si)", &student, &score)) {
        g_print("    %s: %d\n", student, score);
    }
    
    g_variant_unref(scores);
    
    /* Example 6: Serialization to bytes */
    g_print("\n6. Serialization:\n\n");
    
    GVariant *data = g_variant_new("(si)", "config", 3);
    
    /* Serialize to bytes */
    gsize size = g_variant_get_size(data);
    gpointer serialized = g_malloc(size);
    g_variant_store(data, serialized);
    
    g_print("  Serialized size: %zu bytes\n", size);
    
    /* Deserialize */
    GVariant *restored = g_variant_new_from_data(
        G_VARIANT_TYPE("(si)"),
        serialized,
        size,
        FALSE,
        g_free,
        serialized
    );
    
    g_print("  Restored: ");
    print_variant(restored, 0);
    
    g_variant_unref(data);
    g_variant_unref(restored);
    
    g_print("\n=== Key Points ===\n");
    g_print("- GVariant is type-safe serialization\n");
    g_print("- Format strings define type: 's'=string, 'i'=int, etc.\n");
    g_print("- Use builders for arrays and dicts\n");
    g_print("- g_variant_lookup() for dict access\n");
    g_print("- Serialize with g_variant_store()\n");
    g_print("- Great for IPC, configs, D-Bus\n");
    
    return 0;
}
