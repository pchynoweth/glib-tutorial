/*
 * array_example.c - Dynamic array operations
 * 
 * Demonstrates GArray (for fixed-size elements) and GPtrArray (for pointers).
 */

#include <glib.h>

/* Structure for demo purposes */
typedef struct {
    gint id;
    gchar name[32];
} Person;

/* Function to free Person data */
void free_person(gpointer data)
{
    Person *person = (Person *)data;
    g_print("  Freeing person: %s\n", person->name);
    g_free(person);
}

int main(void)
{
    g_print("=== GLib Array Examples ===\n");
    
    /* ===== GArray Example ===== */
    g_print("\n1. GArray (array of integers):\n");
    
    /* Create array of gint with initial size 0, auto-grow enabled */
    GArray *int_array = g_array_new(FALSE,  /* zero-terminated */
                                    FALSE,  /* clear elements */
                                    sizeof(gint));
    
    /* Add elements */
    for (gint i = 0; i < 5; i++) {
        gint value = i * 10;
        g_array_append_val(int_array, value);
    }
    
    g_print("Array length: %u\n", int_array->len);
    
    /* Access elements */
    g_print("Elements: ");
    for (guint i = 0; i < int_array->len; i++) {
        gint value = g_array_index(int_array, gint, i);
        g_print("%d ", value);
    }
    g_print("\n");
    
    /* Insert element at position */
    gint new_value = 15;
    g_array_insert_val(int_array, 2, new_value);
    g_print("After inserting 15 at index 2: ");
    for (guint i = 0; i < int_array->len; i++) {
        g_print("%d ", g_array_index(int_array, gint, i));
    }
    g_print("\n");
    
    /* Remove element */
    g_array_remove_index(int_array, 2);
    g_print("After removing index 2: ");
    for (guint i = 0; i < int_array->len; i++) {
        g_print("%d ", g_array_index(int_array, gint, i));
    }
    g_print("\n");
    
    /* Sort array */
    g_array_sort(int_array, (GCompareFunc)g_strcmp0);
    
    /* Prepend elements */
    gint prepend_val = -10;
    g_array_prepend_val(int_array, prepend_val);
    g_print("After prepending -10: ");
    for (guint i = 0; i < int_array->len; i++) {
        g_print("%d ", g_array_index(int_array, gint, i));
    }
    g_print("\n");
    
    /* Clean up - free array and data */
    g_array_free(int_array, TRUE);
    
    /* ===== GPtrArray Example ===== */
    g_print("\n2. GPtrArray (array of pointers):\n");
    
    /* Create pointer array with destroy function */
    GPtrArray *ptr_array = g_ptr_array_new_with_free_func(free_person);
    
    /* Add Person objects */
    for (gint i = 0; i < 3; i++) {
        Person *person = g_new(Person, 1);
        person->id = i + 1;
        g_snprintf(person->name, sizeof(person->name), "Person_%d", i + 1);
        g_ptr_array_add(ptr_array, person);
    }
    
    g_print("Pointer array length: %u\n", ptr_array->len);
    
    /* Access elements */
    g_print("People in array:\n");
    for (guint i = 0; i < ptr_array->len; i++) {
        Person *person = g_ptr_array_index(ptr_array, i);
        g_print("  ID: %d, Name: %s\n", person->id, person->name);
    }
    
    /* Insert at specific position */
    Person *new_person = g_new(Person, 1);
    new_person->id = 99;
    g_snprintf(new_person->name, sizeof(new_person->name), "Inserted_Person");
    g_ptr_array_insert(ptr_array, 1, new_person);
    
    g_print("After inserting at index 1:\n");
    for (guint i = 0; i < ptr_array->len; i++) {
        Person *person = g_ptr_array_index(ptr_array, i);
        g_print("  ID: %d, Name: %s\n", person->id, person->name);
    }
    
    /* Remove element by index (calls free_person) */
    g_print("\nRemoving element at index 1:\n");
    g_ptr_array_remove_index(ptr_array, 1);
    
    g_print("After removal:\n");
    for (guint i = 0; i < ptr_array->len; i++) {
        Person *person = g_ptr_array_index(ptr_array, i);
        g_print("  ID: %d, Name: %s\n", person->id, person->name);
    }
    
    /* ===== GArray with structs ===== */
    g_print("\n3. GArray with structs:\n");
    
    GArray *struct_array = g_array_new(FALSE, FALSE, sizeof(Person));
    
    for (gint i = 0; i < 3; i++) {
        Person p;
        p.id = i + 100;
        g_snprintf(p.name, sizeof(p.name), "Struct_Person_%d", i + 1);
        g_array_append_val(struct_array, p);
    }
    
    g_print("Struct array contents:\n");
    for (guint i = 0; i < struct_array->len; i++) {
        Person p = g_array_index(struct_array, Person, i);
        g_print("  ID: %d, Name: %s\n", p.id, p.name);
    }
    
    /* Clean up */
    g_array_free(struct_array, TRUE);
    
    g_print("\nFreeing pointer array:\n");
    g_ptr_array_free(ptr_array, TRUE);  /* Calls free_person on each element */
    
    g_print("\n=== Memory cleaned up ===\n");
    
    return 0;
}
