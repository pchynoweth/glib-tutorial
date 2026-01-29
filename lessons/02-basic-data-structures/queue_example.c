/*
 * queue_example.c - Queue operations
 * 
 * GQueue is a double-ended queue built on top of GList.
 * Supports efficient operations at both ends.
 */

#include <glib.h>

void print_queue(GQueue *queue, const gchar *title)
{
    g_print("\n%s (length %u):\n", title, g_queue_get_length(queue));
    g_print("  [");
    for (guint i = 0; i < g_queue_get_length(queue); i++) {
        gint value = GPOINTER_TO_INT(g_queue_peek_nth(queue, i));
        g_print("%d", value);
        if (i < g_queue_get_length(queue) - 1) {
            g_print(", ");
        }
    }
    g_print("]\n");
}

int main(void)
{
    g_print("=== GLib GQueue Example ===\n");
    
    /* 1. Create a queue */
    GQueue *queue = g_queue_new();
    
    /* 2. Push elements to the tail (enqueue) */
    g_print("\n1. Pushing to tail:\n");
    g_queue_push_tail(queue, GINT_TO_POINTER(10));
    g_queue_push_tail(queue, GINT_TO_POINTER(20));
    g_queue_push_tail(queue, GINT_TO_POINTER(30));
    print_queue(queue, "After pushing 10, 20, 30 to tail");
    
    /* 3. Push elements to the head */
    g_print("\n2. Pushing to head:\n");
    g_queue_push_head(queue, GINT_TO_POINTER(5));
    print_queue(queue, "After pushing 5 to head");
    
    /* 4. Peek at elements without removing */
    g_print("\n3. Peeking:\n");
    gint head = GPOINTER_TO_INT(g_queue_peek_head(queue));
    gint tail = GPOINTER_TO_INT(g_queue_peek_tail(queue));
    g_print("Head element: %d\n", head);
    g_print("Tail element: %d\n", tail);
    
    /* 5. Pop from head (dequeue) */
    g_print("\n4. Popping from head:\n");
    gint popped = GPOINTER_TO_INT(g_queue_pop_head(queue));
    g_print("Popped: %d\n", popped);
    print_queue(queue, "After pop");
    
    /* 6. Pop from tail */
    g_print("\n5. Popping from tail:\n");
    gint popped_tail = GPOINTER_TO_INT(g_queue_pop_tail(queue));
    g_print("Popped from tail: %d\n", popped_tail);
    print_queue(queue, "After tail pop");
    
    /* 7. Find element */
    g_print("\n6. Finding element:\n");
    gint search_value = 20;
    GList *found = g_queue_find(queue, GINT_TO_POINTER(search_value));
    if (found) {
        g_print("Found %d in queue\n", search_value);
    }
    
    /* 8. Check if queue contains element */
    g_print("Queue contains 20: %s\n", 
            g_queue_find(queue, GINT_TO_POINTER(20)) ? "YES" : "NO");
    g_print("Queue contains 99: %s\n", 
            g_queue_find(queue, GINT_TO_POINTER(99)) ? "YES" : "NO");
    
    /* 9. Insert at specific position */
    g_print("\n7. Inserting at position:\n");
    g_queue_push_tail(queue, GINT_TO_POINTER(40));
    g_queue_push_tail(queue, GINT_TO_POINTER(50));
    g_queue_insert_before(queue, g_queue_peek_nth_link(queue, 1), 
                         GINT_TO_POINTER(15));
    print_queue(queue, "After inserting 15 before index 1");
    
    /* 10. Remove specific value */
    g_print("\n8. Removing value:\n");
    gboolean removed = g_queue_remove(queue, GINT_TO_POINTER(15));
    g_print("Removed 15: %s\n", removed ? "YES" : "NO");
    print_queue(queue, "After removing 15");
    
    /* 11. Get queue as array */
    g_print("\n9. Getting array representation:\n");
    gpointer *array = g_queue_peek_tail_link(queue) ? 
                      g_new(gpointer, g_queue_get_length(queue)) : NULL;
    if (array) {
        for (guint i = 0; i < g_queue_get_length(queue); i++) {
            array[i] = g_queue_peek_nth(queue, i);
        }
        g_print("Array: [");
        for (guint i = 0; i < g_queue_get_length(queue); i++) {
            g_print("%d", GPOINTER_TO_INT(array[i]));
            if (i < g_queue_get_length(queue) - 1) {
                g_print(", ");
            }
        }
        g_print("]\n");
        g_free(array);
    }
    
    /* 12. Reverse queue */
    g_print("\n10. Reversing queue:\n");
    g_queue_reverse(queue);
    print_queue(queue, "After reverse");
    
    /* 13. Check if empty */
    g_print("\n11. Queue status:\n");
    g_print("Is empty: %s\n", g_queue_is_empty(queue) ? "YES" : "NO");
    g_print("Length: %u\n", g_queue_get_length(queue));
    
    /* 14. Clear queue */
    g_print("\n12. Clearing queue:\n");
    g_queue_clear(queue);
    print_queue(queue, "After clear");
    g_print("Is empty: %s\n", g_queue_is_empty(queue) ? "YES" : "NO");
    
    /* 15. Use as a stack (LIFO) */
    g_print("\n13. Using as stack (LIFO):\n");
    g_queue_push_head(queue, GINT_TO_POINTER(100));
    g_queue_push_head(queue, GINT_TO_POINTER(200));
    g_queue_push_head(queue, GINT_TO_POINTER(300));
    print_queue(queue, "Stack (pushed to head)");
    
    g_print("Pop stack: %d\n", GPOINTER_TO_INT(g_queue_pop_head(queue)));
    g_print("Pop stack: %d\n", GPOINTER_TO_INT(g_queue_pop_head(queue)));
    print_queue(queue, "After popping twice");
    
    /* Clean up */
    g_queue_free(queue);
    
    g_print("\n=== Memory cleaned up ===\n");
    
    return 0;
}
