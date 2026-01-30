/*
 * custom_data_structure.c - Building custom data structures with GLib
 * 
 * Demonstrates how to create custom data structures using
 * GLib's building blocks.
 */

#include <glib.h>

/* ============================================================
 * Custom Priority Queue using GLib primitives
 * ============================================================ */

typedef struct {
    gchar *data;
    gint priority;
} PriorityItem;

typedef struct {
    GQueue *items;  /* Using GQueue as base */
} PriorityQueue;

static gint compare_priority(gconstpointer a, gconstpointer b, gpointer user_data)
{
    const PriorityItem *item_a = a;
    const PriorityItem *item_b = b;
    /* Higher priority first (descending order) */
    return item_b->priority - item_a->priority;
}

static PriorityQueue *priority_queue_new(void)
{
    PriorityQueue *pq = g_new(PriorityQueue, 1);
    pq->items = g_queue_new();
    return pq;
}

static void priority_queue_push(PriorityQueue *pq, const gchar *data, gint priority)
{
    PriorityItem *item = g_new(PriorityItem, 1);
    item->data = g_strdup(data);
    item->priority = priority;
    
    /* Insert sorted */
    g_queue_insert_sorted(pq->items, item, compare_priority, NULL);
}

static gchar *priority_queue_pop(PriorityQueue *pq)
{
    if (g_queue_is_empty(pq->items)) {
        return NULL;
    }
    
    PriorityItem *item = g_queue_pop_head(pq->items);
    gchar *data = item->data;  /* Transfer ownership */
    g_free(item);
    return data;
}

static gboolean priority_queue_is_empty(PriorityQueue *pq)
{
    return g_queue_is_empty(pq->items);
}

static void priority_queue_free(PriorityQueue *pq)
{
    while (!g_queue_is_empty(pq->items)) {
        PriorityItem *item = g_queue_pop_head(pq->items);
        g_free(item->data);
        g_free(item);
    }
    g_queue_free(pq->items);
    g_free(pq);
}

/* ============================================================
 * Custom LRU Cache using GLib primitives
 * ============================================================ */

typedef struct {
    GHashTable *table;  /* Key -> CacheEntry */
    GQueue *order;      /* Access order (most recent at head) */
    guint capacity;
    guint size;
} LRUCache;

typedef struct {
    gchar *key;
    gchar *value;
    GList *node;  /* Pointer to position in order queue */
} CacheEntry;

static void cache_entry_free(CacheEntry *entry)
{
    g_free(entry->key);
    g_free(entry->value);
    g_free(entry);
}

static LRUCache *lru_cache_new(guint capacity)
{
    LRUCache *cache = g_new(LRUCache, 1);
    cache->table = g_hash_table_new_full(g_str_hash, g_str_equal,
                                          NULL, (GDestroyNotify)cache_entry_free);
    cache->order = g_queue_new();
    cache->capacity = capacity;
    cache->size = 0;
    return cache;
}

static void lru_cache_put(LRUCache *cache, const gchar *key, const gchar *value)
{
    CacheEntry *existing = g_hash_table_lookup(cache->table, key);
    
    if (existing) {
        /* Update existing - move to front */
        g_free(existing->value);
        existing->value = g_strdup(value);
        g_queue_unlink(cache->order, existing->node);
        g_queue_push_head_link(cache->order, existing->node);
    } else {
        /* Evict if at capacity */
        if (cache->size >= cache->capacity) {
            GList *last = g_queue_pop_tail_link(cache->order);
            if (last) {
                CacheEntry *evicted = last->data;
                g_print("  [Cache] Evicting: %s\n", evicted->key);
                g_hash_table_remove(cache->table, evicted->key);
                g_list_free(last);
                cache->size--;
            }
        }
        
        /* Add new entry */
        CacheEntry *entry = g_new(CacheEntry, 1);
        entry->key = g_strdup(key);
        entry->value = g_strdup(value);
        
        g_queue_push_head(cache->order, entry);
        entry->node = cache->order->head;
        
        g_hash_table_insert(cache->table, entry->key, entry);
        cache->size++;
    }
}

static const gchar *lru_cache_get(LRUCache *cache, const gchar *key)
{
    CacheEntry *entry = g_hash_table_lookup(cache->table, key);
    
    if (entry) {
        /* Move to front (most recently used) */
        g_queue_unlink(cache->order, entry->node);
        g_queue_push_head_link(cache->order, entry->node);
        return entry->value;
    }
    
    return NULL;
}

static void lru_cache_free(LRUCache *cache)
{
    g_hash_table_destroy(cache->table);
    g_queue_free(cache->order);
    g_free(cache);
}

int main(void)
{
    g_print("=== Custom Data Structures Example ===\n\n");
    
    /* Example 1: Priority Queue */
    g_print("1. Priority Queue:\n\n");
    
    PriorityQueue *pq = priority_queue_new();
    
    priority_queue_push(pq, "Low priority task", 1);
    priority_queue_push(pq, "High priority task", 10);
    priority_queue_push(pq, "Medium priority task", 5);
    priority_queue_push(pq, "Urgent task", 15);
    priority_queue_push(pq, "Normal task", 5);
    
    g_print("  Popping in priority order:\n");
    while (!priority_queue_is_empty(pq)) {
        gchar *task = priority_queue_pop(pq);
        g_print("    - %s\n", task);
        g_free(task);
    }
    
    priority_queue_free(pq);
    
    /* Example 2: LRU Cache */
    g_print("\n2. LRU Cache (capacity=3):\n\n");
    
    LRUCache *cache = lru_cache_new(3);
    
    g_print("  Adding entries:\n");
    lru_cache_put(cache, "key1", "value1");
    g_print("    Added key1\n");
    lru_cache_put(cache, "key2", "value2");
    g_print("    Added key2\n");
    lru_cache_put(cache, "key3", "value3");
    g_print("    Added key3\n");
    
    g_print("\n  Looking up key1 (moves to front): %s\n", 
            lru_cache_get(cache, "key1"));
    
    g_print("\n  Adding key4 (should evict key2):\n");
    lru_cache_put(cache, "key4", "value4");
    
    g_print("\n  Cache contents after eviction:\n");
    const gchar *val;
    val = lru_cache_get(cache, "key1");
    g_print("    key1: %s\n", val ? val : "(not found)");
    val = lru_cache_get(cache, "key2");
    g_print("    key2: %s\n", val ? val : "(not found - evicted)");
    val = lru_cache_get(cache, "key3");
    g_print("    key3: %s\n", val ? val : "(not found)");
    val = lru_cache_get(cache, "key4");
    g_print("    key4: %s\n", val ? val : "(not found)");
    
    lru_cache_free(cache);
    
    /* Example 3: Using GLib data structures directly */
    g_print("\n3. GLib Building Blocks:\n\n");
    
    g_print("  GQueue - Double-ended queue\n");
    g_print("  GHashTable - Hash table with custom keys/values\n");
    g_print("  GList/GSList - Linked lists\n");
    g_print("  GTree - Self-balancing binary tree\n");
    g_print("  GArray - Dynamic array\n");
    g_print("  GPtrArray - Dynamic pointer array\n");
    
    g_print("\n=== Key Points ===\n");
    g_print("- Compose GLib types for custom structures\n");
    g_print("- Use GQueue for queue-like behavior\n");
    g_print("- GHashTable for O(1) key lookup\n");
    g_print("- Define comparison functions for sorting\n");
    g_print("- Handle memory ownership carefully\n");
    g_print("- Free all nested structures\n");
    
    return 0;
}
