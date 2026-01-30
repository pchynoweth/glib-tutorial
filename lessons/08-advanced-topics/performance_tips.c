/*
 * performance_tips.c - Performance optimization with GLib
 * 
 * Demonstrates performance best practices when using GLib,
 * including efficient data structures and allocation patterns.
 */

#include <glib.h>

/* Benchmark helper */
static gdouble benchmark(const gchar *name, void (*func)(gint), gint iterations)
{
    GTimer *timer = g_timer_new();
    
    g_timer_start(timer);
    func(iterations);
    g_timer_stop(timer);
    
    gdouble elapsed = g_timer_elapsed(timer, NULL);
    g_print("  %s: %.4f seconds\n", name, elapsed);
    
    g_timer_destroy(timer);
    return elapsed;
}

/* ============================================================
 * String Comparison: g_string_append vs strcat
 * ============================================================ */

static void test_gstring_append(gint iterations)
{
    GString *str = g_string_new("");
    
    for (gint i = 0; i < iterations; i++) {
        g_string_append(str, "x");
    }
    
    g_string_free(str, TRUE);
}

static void test_manual_concat(gint iterations)
{
    gchar *result = g_strdup("");
    
    for (gint i = 0; i < iterations; i++) {
        gchar *new_result = g_strconcat(result, "x", NULL);
        g_free(result);
        result = new_result;
    }
    
    g_free(result);
}

/* ============================================================
 * List Append: GList vs GPtrArray
 * ============================================================ */

static void test_glist_append(gint iterations)
{
    GList *list = NULL;
    
    for (gint i = 0; i < iterations; i++) {
        list = g_list_append(list, GINT_TO_POINTER(i));
    }
    
    g_list_free(list);
}

static void test_glist_prepend(gint iterations)
{
    GList *list = NULL;
    
    for (gint i = 0; i < iterations; i++) {
        list = g_list_prepend(list, GINT_TO_POINTER(i));
    }
    
    /* Reverse if order matters */
    list = g_list_reverse(list);
    
    g_list_free(list);
}

static void test_gptrarray(gint iterations)
{
    GPtrArray *array = g_ptr_array_new();
    
    for (gint i = 0; i < iterations; i++) {
        g_ptr_array_add(array, GINT_TO_POINTER(i));
    }
    
    g_ptr_array_free(array, TRUE);
}

/* ============================================================
 * Hash Table: Different key types
 * ============================================================ */

static void test_hash_string_key(gint iterations)
{
    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               g_free, NULL);
    
    for (gint i = 0; i < iterations; i++) {
        gchar *key = g_strdup_printf("key_%d", i);
        g_hash_table_insert(table, key, GINT_TO_POINTER(i));
    }
    
    /* Lookup some values */
    for (gint i = 0; i < iterations; i += 100) {
        gchar *key = g_strdup_printf("key_%d", i);
        g_hash_table_lookup(table, key);
        g_free(key);
    }
    
    g_hash_table_destroy(table);
}

static void test_hash_int_key(gint iterations)
{
    GHashTable *table = g_hash_table_new(g_direct_hash, g_direct_equal);
    
    for (gint i = 0; i < iterations; i++) {
        g_hash_table_insert(table, GINT_TO_POINTER(i), GINT_TO_POINTER(i));
    }
    
    /* Lookup some values */
    for (gint i = 0; i < iterations; i += 100) {
        g_hash_table_lookup(table, GINT_TO_POINTER(i));
    }
    
    g_hash_table_destroy(table);
}

/* ============================================================
 * Memory Allocation Patterns
 * ============================================================ */

static void test_individual_allocs(gint iterations)
{
    gint **ptrs = g_new(gint*, iterations);
    
    for (gint i = 0; i < iterations; i++) {
        ptrs[i] = g_new(gint, 1);
        *ptrs[i] = i;
    }
    
    for (gint i = 0; i < iterations; i++) {
        g_free(ptrs[i]);
    }
    
    g_free(ptrs);
}

static void test_batch_alloc(gint iterations)
{
    gint *array = g_new(gint, iterations);
    
    for (gint i = 0; i < iterations; i++) {
        array[i] = i;
    }
    
    g_free(array);
}

int main(void)
{
    g_print("=== GLib Performance Tips ===\n\n");
    
    /* Test 1: String Building */
    g_print("1. String Building (10000 appends):\n\n");
    
    gdouble gstring_time = benchmark("GString append", test_gstring_append, 10000);
    gdouble concat_time = benchmark("g_strconcat loop", test_manual_concat, 10000);
    
    g_print("\n  Result: GString is %.1fx faster\n", concat_time / gstring_time);
    g_print("  Tip: Use GString for building strings incrementally\n");
    
    /* Test 2: List Operations */
    g_print("\n2. List Building (10000 elements):\n\n");
    
    gdouble append_time = benchmark("GList append", test_glist_append, 10000);
    gdouble prepend_time = benchmark("GList prepend+reverse", test_glist_prepend, 10000);
    gdouble array_time = benchmark("GPtrArray add", test_gptrarray, 10000);
    
    g_print("\n  Result: GPtrArray is %.1fx faster than append\n", 
            append_time / array_time);
    g_print("  Tip: Prefer GPtrArray for sequential access\n");
    g_print("  Tip: Use prepend+reverse instead of append\n");
    
    /* Test 3: Hash Table Keys */
    g_print("\n3. Hash Table Key Types (10000 entries):\n\n");
    
    gdouble str_key_time = benchmark("String keys", test_hash_string_key, 10000);
    gdouble int_key_time = benchmark("Integer keys (direct)", test_hash_int_key, 10000);
    
    g_print("\n  Result: Integer keys are %.1fx faster\n", 
            str_key_time / int_key_time);
    g_print("  Tip: Use g_direct_hash for integer keys\n");
    
    /* Test 4: Allocation Patterns */
    g_print("\n4. Allocation Patterns (10000 integers):\n\n");
    
    gdouble individual_time = benchmark("Individual allocs", test_individual_allocs, 10000);
    gdouble batch_time = benchmark("Batch alloc", test_batch_alloc, 10000);
    
    g_print("\n  Result: Batch alloc is %.1fx faster\n", 
            individual_time / batch_time);
    g_print("  Tip: Allocate in batches when possible\n");
    
    /* Summary */
    g_print("\n=== Performance Best Practices ===\n\n");
    
    g_print("String Operations:\n");
    g_print("  - Use GString for building strings\n");
    g_print("  - Pre-allocate with g_string_sized_new() if size known\n");
    g_print("  - Avoid repeated g_strconcat() in loops\n\n");
    
    g_print("Collections:\n");
    g_print("  - Prefer GPtrArray over GList for random access\n");
    g_print("  - Use g_list_prepend() + reverse instead of append\n");
    g_print("  - Pre-size arrays with g_ptr_array_sized_new()\n\n");
    
    g_print("Hash Tables:\n");
    g_print("  - Use g_direct_hash for integer keys\n");
    g_print("  - Consider GTree for sorted iteration\n\n");
    
    g_print("Memory:\n");
    g_print("  - Batch allocations when possible\n");
    g_print("  - Use g_new0() only when zero-init needed\n");
    g_print("  - Pool allocators for many same-size objects\n");
    
    return 0;
}
