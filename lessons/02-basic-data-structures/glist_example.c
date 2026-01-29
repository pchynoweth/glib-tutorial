/*
 * glist_example.c - Doubly-linked list operations
 * 
 * GList is a doubly-linked list that allows traversal in both directions.
 * Each element has pointers to the next and previous elements.
 */

#include <glib.h>

/* Helper function to print a list of integers */
void print_list(GList *list, const gchar *title)
{
    g_print("\n%s:\n", title);
    for (GList *l = list; l != NULL; l = l->next) {
        g_print("%d ", GPOINTER_TO_INT(l->data));
    }
    g_print("\n");
}

/* Custom comparison function for sorting */
gint compare_ints(gconstpointer a, gconstpointer b)
{
    return GPOINTER_TO_INT(a) - GPOINTER_TO_INT(b);
}

/* Function to be called for each element */
void print_element(gpointer data, gpointer user_data)
{
    const gchar *prefix = (const gchar *)user_data;
    g_print("%s%d\n", prefix, GPOINTER_TO_INT(data));
}

int main(void)
{
    GList *list = NULL;
    
    g_print("=== GLib GList (Doubly-Linked List) Example ===\n");
    
    /* 1. Adding elements - prepend is O(1), append is O(n) */
    g_print("\n1. Building list with append and prepend:\n");
    list = g_list_append(list, GINT_TO_POINTER(10));
    list = g_list_append(list, GINT_TO_POINTER(20));
    list = g_list_append(list, GINT_TO_POINTER(30));
    list = g_list_prepend(list, GINT_TO_POINTER(5));  /* Faster than append */
    print_list(list, "After adding 5, 10, 20, 30");
    
    /* 2. Inserting at specific positions */
    list = g_list_insert(list, GINT_TO_POINTER(15), 2);  /* Insert at index 2 */
    print_list(list, "After inserting 15 at index 2");
    
    /* 3. Getting list information */
    g_print("\nList length: %u\n", g_list_length(list));
    g_print("First element: %d\n", GPOINTER_TO_INT(g_list_first(list)->data));
    g_print("Last element: %d\n", GPOINTER_TO_INT(g_list_last(list)->data));
    
    /* 4. Finding elements */
    GList *found = g_list_find(list, GINT_TO_POINTER(20));
    if (found) {
        g_print("Found element 20 at position %d\n", 
                g_list_position(list, found));
    }
    
    /* 5. Accessing by index */
    GList *third = g_list_nth(list, 2);
    if (third) {
        g_print("Third element (index 2): %d\n", 
                GPOINTER_TO_INT(third->data));
    }
    
    /* 6. Sorting the list */
    list = g_list_sort(list, compare_ints);
    print_list(list, "After sorting");
    
    /* 7. Reversing the list */
    list = g_list_reverse(list);
    print_list(list, "After reversing");
    
    /* 8. Iterating with foreach */
    g_print("\nIterating with foreach:\n");
    g_list_foreach(list, print_element, "  Value: ");
    
    /* 9. Removing elements */
    list = g_list_remove(list, GINT_TO_POINTER(15));
    print_list(list, "After removing 15");
    
    /* 10. Removing by position */
    list = g_list_remove(list, g_list_nth_data(list, 0));
    print_list(list, "After removing first element");
    
    /* 11. Copying a list */
    GList *copy = g_list_copy(list);
    g_print("\nList copy created (length: %u)\n", g_list_length(copy));
    
    /* 12. Concatenating lists */
    GList *list2 = NULL;
    list2 = g_list_append(list2, GINT_TO_POINTER(100));
    list2 = g_list_append(list2, GINT_TO_POINTER(200));
    list = g_list_concat(list, list2);  /* list2 is now invalid */
    print_list(list, "After concatenating with [100, 200]");
    
    /* Clean up - free the list */
    g_list_free(list);
    g_list_free(copy);
    
    g_print("\n=== Memory cleaned up ===\n");
    
    return 0;
}
