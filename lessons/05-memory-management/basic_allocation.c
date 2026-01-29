/*
 * basic_allocation.c - GLib memory allocation functions
 * 
 * Demonstrates the various memory allocation functions provided by GLib
 * and when to use each one.
 */

#include <glib.h>

typedef struct {
    gint id;
    gchar *name;
    gdouble value;
} DataStruct;

/* Function to create and initialize a struct */
static DataStruct *create_data(gint id, const gchar *name, gdouble value)
{
    /* g_new allocates memory for one DataStruct */
    /* Type-safe: g_new(Type, count) expands to (Type*) g_malloc(sizeof(Type) * count) */
    DataStruct *data = g_new(DataStruct, 1);
    
    data->id = id;
    data->name = g_strdup(name);  /* Duplicate the string */
    data->value = value;
    
    return data;
}

/* Function to free a DataStruct */
static void free_data(DataStruct *data)
{
    if (data) {
        g_free(data->name);  /* Free the string first */
        g_free(data);        /* Then free the struct */
    }
}

int main(void)
{
    g_print("=== GLib Memory Allocation Examples ===\n\n");
    
    /* Example 1: g_malloc and g_free */
    g_print("1. Basic g_malloc/g_free:\n");
    gchar *str1 = g_malloc(20);  /* Allocate 20 bytes */
    g_snprintf(str1, 20, "Hello, GLib!");
    g_print("  Allocated string: %s\n", str1);
    g_free(str1);
    g_print("  Memory freed\n\n");
    
    /* Example 2: g_malloc0 - zero-initialized */
    g_print("2. g_malloc0 (zero-initialized):\n");
    gint *numbers = g_malloc0(5 * sizeof(gint));
    g_print("  Initialized values: ");
    for (gint i = 0; i < 5; i++) {
        g_print("%d ", numbers[i]);  /* All should be 0 */
    }
    g_print("\n");
    g_free(numbers);
    g_print("  Memory freed\n\n");
    
    /* Example 3: g_new and g_new0 */
    g_print("3. g_new (type-safe allocation):\n");
    DataStruct *data = g_new(DataStruct, 1);
    data->id = 42;
    data->name = g_strdup("Test");
    data->value = 3.14;
    g_print("  Created struct: id=%d, name=%s, value=%.2f\n",
            data->id, data->name, data->value);
    free_data(data);
    g_print("  Memory freed\n\n");
    
    /* Example 4: g_new0 - zero-initialized struct */
    g_print("4. g_new0 (zero-initialized struct):\n");
    DataStruct *zero_data = g_new0(DataStruct, 1);
    g_print("  Uninitialized struct: id=%d, name=%p, value=%.2f\n",
            zero_data->id, (void *)zero_data->name, zero_data->value);
    g_free(zero_data);
    g_print("  Memory freed\n\n");
    
    /* Example 5: g_try_malloc - doesn't abort on failure */
    g_print("5. g_try_malloc (safe allocation):\n");
    gchar *safe_alloc = g_try_malloc(100);
    if (safe_alloc) {
        g_print("  Allocation successful\n");
        g_free(safe_alloc);
    } else {
        g_print("  Allocation failed (returned NULL)\n");
    }
    g_print("\n");
    
    /* Example 6: g_strdup family */
    g_print("6. String duplication functions:\n");
    const gchar *original = "Original string";
    gchar *dup = g_strdup(original);
    gchar *dup_n = g_strndup(original, 8);  /* First 8 chars */
    g_print("  Original: %s\n", original);
    g_print("  Full duplicate: %s\n", dup);
    g_print("  Partial duplicate: %s\n", dup_n);
    g_free(dup);
    g_free(dup_n);
    g_print("  Memory freed\n\n");
    
    /* Example 7: g_strdup_printf */
    g_print("7. g_strdup_printf (formatted allocation):\n");
    gchar *formatted = g_strdup_printf("Number: %d, Float: %.2f", 42, 3.14);
    g_print("  Formatted string: %s\n", formatted);
    g_free(formatted);
    g_print("  Memory freed\n\n");
    
    /* Example 8: g_realloc */
    g_print("8. g_realloc (resize allocation):\n");
    gint *array = g_new(gint, 3);
    for (gint i = 0; i < 3; i++) {
        array[i] = i + 1;
    }
    g_print("  Original array: ");
    for (gint i = 0; i < 3; i++) {
        g_print("%d ", array[i]);
    }
    g_print("\n");
    
    /* Resize to hold 5 integers */
    array = g_renew(gint, array, 5);
    array[3] = 4;
    array[4] = 5;
    g_print("  Resized array: ");
    for (gint i = 0; i < 5; i++) {
        g_print("%d ", array[i]);
    }
    g_print("\n");
    g_free(array);
    g_print("  Memory freed\n\n");
    
    /* Example 9: g_memdup */
    g_print("9. g_memdup (duplicate memory block):\n");
    gint source[] = {10, 20, 30, 40, 50};
    gint *duplicate = g_memdup(source, sizeof(source));
    g_print("  Source: ");
    for (gsize i = 0; i < sizeof(source) / sizeof(gint); i++) {
        g_print("%d ", source[i]);
    }
    g_print("\n");
    g_print("  Duplicate: ");
    for (gsize i = 0; i < sizeof(source) / sizeof(gint); i++) {
        g_print("%d ", duplicate[i]);
    }
    g_print("\n");
    g_free(duplicate);
    g_print("  Memory freed\n\n");
    
    /* Example 10: Using the create/free pattern */
    g_print("10. Create/free pattern:\n");
    DataStruct *managed = create_data(100, "Managed Data", 2.718);
    g_print("  Created: id=%d, name=%s, value=%.3f\n",
            managed->id, managed->name, managed->value);
    free_data(managed);
    g_print("  Properly freed\n");
    
    g_print("\n=== Key Points ===\n");
    g_print("- g_malloc aborts on failure, g_try_malloc returns NULL\n");
    g_print("- g_new/g_new0 are type-safe allocation macros\n");
    g_print("- Always pair allocations with frees\n");
    g_print("- Free in reverse order of allocation for nested structures\n");
    g_print("- Use g_strdup for string duplication\n");
    g_print("- g_renew for resizing allocations\n");
    
    return 0;
}
