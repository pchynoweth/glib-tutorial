/*
 * async_queue.c - Producer-consumer pattern with GAsyncQueue
 * 
 * GAsyncQueue is a thread-safe queue for passing data between threads.
 * Perfect for producer-consumer patterns without manual locking.
 */

#include <glib.h>

typedef struct {
    gint task_id;
    gchar *description;
} Task;

/* Create a new task */
static Task *task_new(gint id, const gchar *desc)
{
    Task *task = g_new(Task, 1);
    task->task_id = id;
    task->description = g_strdup(desc);
    return task;
}

/* Free a task */
static void task_free(Task *task)
{
    if (task) {
        g_free(task->description);
        g_free(task);
    }
}

/* Producer thread - creates tasks and adds them to queue */
static gpointer producer_thread(gpointer user_data)
{
    GAsyncQueue *queue = (GAsyncQueue *)user_data;
    
    g_print("[Producer] Starting...\n");
    
    for (gint i = 1; i <= 10; i++) {
        /* Create a task */
        gchar *desc = g_strdup_printf("Task %d", i);
        Task *task = task_new(i, desc);
        g_free(desc);
        
        g_print("[Producer] Producing task %d\n", i);
        
        /* Push task to queue (thread-safe) */
        g_async_queue_push(queue, task);
        
        /* Simulate work */
        g_usleep(200000);  /* 200ms */
    }
    
    /* Send termination signals (NULL) for each consumer */
    g_print("[Producer] Sending termination signals...\n");
    g_async_queue_push(queue, NULL);
    g_async_queue_push(queue, NULL);
    
    g_print("[Producer] Finished\n");
    return NULL;
}

/* Consumer thread - processes tasks from queue */
static gpointer consumer_thread(gpointer user_data)
{
    GAsyncQueue *queue = (GAsyncQueue *)user_data;
    GThread *self = g_thread_self();
    
    g_print("[Consumer %p] Starting...\n", (void *)self);
    
    while (TRUE) {
        /* Pop task from queue (blocks if empty) */
        Task *task = g_async_queue_pop(queue);
        
        /* NULL means terminate */
        if (task == NULL) {
            g_print("[Consumer %p] Received termination signal\n", 
                    (void *)self);
            break;
        }
        
        g_print("[Consumer %p] Processing task %d: %s\n",
                (void *)self, task->task_id, task->description);
        
        /* Simulate processing */
        g_usleep(300000);  /* 300ms */
        
        g_print("[Consumer %p] Completed task %d\n",
                (void *)self, task->task_id);
        
        /* Free the task */
        task_free(task);
    }
    
    g_print("[Consumer %p] Finished\n", (void *)self);
    return NULL;
}

/* Demonstrates try_pop and timed_pop */
static gpointer trylock_consumer(gpointer user_data)
{
    GAsyncQueue *queue = (GAsyncQueue *)user_data;
    
    g_print("[TryConsumer] Starting...\n");
    
    for (gint i = 0; i < 5; i++) {
        /* Try to pop without blocking */
        Task *task = g_async_queue_try_pop(queue);
        
        if (task) {
            g_print("[TryConsumer] Got task %d\n", task->task_id);
            task_free(task);
        } else {
            g_print("[TryConsumer] Queue empty, doing other work\n");
        }
        
        g_usleep(400000);  /* 400ms */
    }
    
    g_print("[TryConsumer] Finished\n");
    return NULL;
}

int main(void)
{
    g_print("=== GLib GAsyncQueue Example ===\n\n");
    
    /* Create a thread-safe queue */
    GAsyncQueue *queue = g_async_queue_new();
    
    g_print("Queue created (initial length: %d)\n\n", 
            g_async_queue_length(queue));
    
    /* Example 1: Basic producer-consumer */
    g_print("1. Starting producer-consumer demo:\n\n");
    
    /* Create producer thread */
    GThread *producer = g_thread_new("producer", producer_thread, queue);
    
    /* Create consumer threads */
    GThread *consumer1 = g_thread_new("consumer1", consumer_thread, queue);
    GThread *consumer2 = g_thread_new("consumer2", consumer_thread, queue);
    
    /* Wait for all threads to complete */
    g_thread_join(producer);
    g_thread_join(consumer1);
    g_thread_join(consumer2);
    
    g_print("\n2. Queue statistics:\n");
    g_print("Final queue length: %d\n\n", g_async_queue_length(queue));
    
    /* Example 2: Try pop and timeout pop */
    g_print("3. Testing try_pop:\n\n");
    
    /* Add some tasks */
    for (gint i = 1; i <= 3; i++) {
        Task *task = task_new(i + 100, "Try-pop task");
        g_async_queue_push(queue, task);
    }
    
    /* Create try-consumer */
    GThread *try_consumer = g_thread_new("try-consumer", 
                                         trylock_consumer, 
                                         queue);
    g_thread_join(try_consumer);
    
    /* Example 3: Timed pop */
    g_print("\n4. Testing timed_pop:\n");
    
    g_print("[Main] Waiting up to 1 second for task...\n");
    gint64 end_time = g_get_monotonic_time() + 1 * G_TIME_SPAN_SECOND;
    Task *task = g_async_queue_timeout_pop(queue, end_time);
    
    if (task) {
        g_print("[Main] Got task: %d\n", task->task_id);
        task_free(task);
    } else {
        g_print("[Main] Timeout - no task available\n");
    }
    
    /* Clean up remaining tasks */
    Task *remaining;
    while ((remaining = g_async_queue_try_pop(queue)) != NULL) {
        task_free(remaining);
    }
    
    /* Clean up queue */
    g_async_queue_unref(queue);
    
    g_print("\n=== Key Points ===\n");
    g_print("- GAsyncQueue is thread-safe, no manual locking needed\n");
    g_print("- g_async_queue_pop() blocks until data available\n");
    g_print("- g_async_queue_try_pop() returns immediately\n");
    g_print("- g_async_queue_timeout_pop() waits with timeout\n");
    g_print("- Perfect for producer-consumer patterns\n");
    g_print("- Can have multiple producers and consumers\n");
    
    return 0;
}
