/*
 * mutex_example.c - Mutex for thread synchronization
 * 
 * Demonstrates using GMutex to protect shared data from race conditions.
 * Shows both correct (with mutex) and incorrect (without mutex) approaches.
 */

#include <glib.h>

/* Shared counter - needs protection */
static gint shared_counter = 0;
static GMutex counter_mutex;

/* Unprotected counter for comparison */
static gint unprotected_counter = 0;

/* Thread function that increments with mutex protection */
static gpointer protected_increment_thread(gpointer user_data)
{
    gint iterations = GPOINTER_TO_INT(user_data);
    
    for (gint i = 0; i < iterations; i++) {
        /* Lock the mutex before accessing shared data */
        g_mutex_lock(&counter_mutex);
        
        /* Critical section - only one thread at a time */
        gint temp = shared_counter;
        g_usleep(1);  /* Simulate some work, increases chance of race conditions */
        shared_counter = temp + 1;
        
        /* Unlock when done */
        g_mutex_unlock(&counter_mutex);
    }
    
    return NULL;
}

/* Thread function without mutex (UNSAFE - for demonstration) */
static gpointer unprotected_increment_thread(gpointer user_data)
{
    gint iterations = GPOINTER_TO_INT(user_data);
    
    for (gint i = 0; i < iterations; i++) {
        /* NO MUTEX - race condition! */
        gint temp = unprotected_counter;
        g_usleep(1);
        unprotected_counter = temp + 1;
    }
    
    return NULL;
}

/* Demonstrates g_mutex_trylock */
static gpointer trylock_thread(gpointer user_data)
{
    gint thread_id = GPOINTER_TO_INT(user_data);
    
    for (gint i = 0; i < 5; i++) {
        if (g_mutex_trylock(&counter_mutex)) {
            g_print("[Thread %d] Lock acquired\n", thread_id);
            g_usleep(100000);  /* Hold lock for 100ms */
            g_mutex_unlock(&counter_mutex);
            g_print("[Thread %d] Lock released\n", thread_id);
        } else {
            g_print("[Thread %d] Lock busy, skipping\n", thread_id);
        }
        g_usleep(50000);
    }
    
    return NULL;
}

int main(void)
{
    g_print("=== GLib GMutex Example ===\n\n");
    
    /* Initialize mutex */
    g_mutex_init(&counter_mutex);
    
    /* Example 1: Protected increment with mutex */
    g_print("1. Testing PROTECTED increment:\n");
    
    #define NUM_THREADS 5
    #define ITERATIONS_PER_THREAD 1000
    
    GThread *threads[NUM_THREADS];
    
    shared_counter = 0;
    
    /* Create threads that increment with mutex protection */
    for (gint i = 0; i < NUM_THREADS; i++) {
        threads[i] = g_thread_new("protected", 
                                  protected_increment_thread,
                                  GINT_TO_POINTER(ITERATIONS_PER_THREAD));
    }
    
    /* Wait for all threads */
    for (gint i = 0; i < NUM_THREADS; i++) {
        g_thread_join(threads[i]);
    }
    
    gint expected = NUM_THREADS * ITERATIONS_PER_THREAD;
    g_print("Expected value: %d\n", expected);
    g_print("Actual value:   %d\n", shared_counter);
    g_print("Result: %s\n\n", 
            (shared_counter == expected) ? "✓ CORRECT" : "✗ INCORRECT");
    
    /* Example 2: Unprotected increment (demonstrates race condition) */
    g_print("2. Testing UNPROTECTED increment (race condition):\n");
    
    unprotected_counter = 0;
    
    /* Create threads that increment WITHOUT mutex */
    for (gint i = 0; i < NUM_THREADS; i++) {
        threads[i] = g_thread_new("unprotected",
                                  unprotected_increment_thread,
                                  GINT_TO_POINTER(ITERATIONS_PER_THREAD));
    }
    
    /* Wait for all threads */
    for (gint i = 0; i < NUM_THREADS; i++) {
        g_thread_join(threads[i]);
    }
    
    g_print("Expected value: %d\n", expected);
    g_print("Actual value:   %d\n", unprotected_counter);
    g_print("Result: %s\n", 
            (unprotected_counter == expected) ? "✓ CORRECT (lucky!)" : "✗ INCORRECT (race condition)");
    g_print("Difference: %d lost increments\n\n", 
            expected - unprotected_counter);
    
    /* Example 3: g_mutex_trylock */
    g_print("3. Testing g_mutex_trylock:\n");
    
    GThread *trylock_threads[3];
    for (gint i = 0; i < 3; i++) {
        trylock_threads[i] = g_thread_new("trylock",
                                          trylock_thread,
                                          GINT_TO_POINTER(i + 1));
    }
    
    for (gint i = 0; i < 3; i++) {
        g_thread_join(trylock_threads[i]);
    }
    
    /* Clean up */
    g_mutex_clear(&counter_mutex);
    
    g_print("\n=== Key Points ===\n");
    g_print("- Always use mutexes to protect shared mutable data\n");
    g_print("- g_mutex_lock() blocks until lock is acquired\n");
    g_print("- g_mutex_trylock() returns FALSE if lock is busy\n");
    g_print("- Always unlock after locking to prevent deadlocks\n");
    g_print("- Race conditions lead to unpredictable, incorrect results\n");
    g_print("- Keep critical sections as short as possible\n");
    
    return 0;
}
