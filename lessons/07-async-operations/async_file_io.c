/*
 * async_file_io.c - Asynchronous file I/O with GIO
 * 
 * Demonstrates how to perform file operations asynchronously
 * using GIO's async file API.
 */

#include <glib.h>
#include <gio/gio.h>

static GMainLoop *main_loop = NULL;

/* Callback when file read completes */
static void on_file_read_complete(GObject *source,
                                   GAsyncResult *result,
                                   gpointer user_data)
{
    GFile *file = G_FILE(source);
    GError *error = NULL;
    gchar *contents = NULL;
    gsize length = 0;
    
    gboolean success = g_file_load_contents_finish(file, result,
                                                    &contents, &length,
                                                    NULL, &error);
    
    if (!success) {
        g_print("[Read] Error: %s\n", error->message);
        g_error_free(error);
    } else {
        g_print("[Read] Successfully read %zu bytes\n", length);
        g_print("[Read] Content preview: %.50s%s\n", 
                contents, length > 50 ? "..." : "");
        g_free(contents);
    }
    
    g_main_loop_quit(main_loop);
}

/* Callback when file write completes */
static void on_file_write_complete(GObject *source,
                                    GAsyncResult *result,
                                    gpointer user_data)
{
    GFile *file = G_FILE(source);
    GError *error = NULL;
    
    gboolean success = g_file_replace_contents_finish(file, result,
                                                       NULL, &error);
    
    if (!success) {
        g_print("[Write] Error: %s\n", error->message);
        g_error_free(error);
    } else {
        g_print("[Write] File written successfully!\n");
    }
    
    /* Now read the file back */
    g_print("\n[Main] Reading file back...\n");
    g_file_load_contents_async(file, NULL, on_file_read_complete, NULL);
}

int main(int argc, char *argv[])
{
    g_print("=== Async File I/O Example ===\n\n");
    
    main_loop = g_main_loop_new(NULL, FALSE);
    
    /* Create a test file */
    const gchar *test_content = "Hello from GIO async file operations!\n"
                                 "This content was written asynchronously.\n"
                                 "The main loop continues while I/O happens.\n";
    
    GFile *file = g_file_new_for_path("/tmp/glib_async_test.txt");
    
    g_print("[Main] Writing to file asynchronously...\n");
    
    /* Write file asynchronously */
    g_file_replace_contents_async(file,
                                   test_content,
                                   strlen(test_content),
                                   NULL,    /* etag */
                                   FALSE,   /* make_backup */
                                   G_FILE_CREATE_NONE,
                                   NULL,    /* cancellable */
                                   on_file_write_complete,
                                   NULL);   /* user_data */
    
    g_print("[Main] Async operation started, running main loop...\n\n");
    
    g_main_loop_run(main_loop);
    
    /* Cleanup */
    g_object_unref(file);
    g_main_loop_unref(main_loop);
    
    g_print("\n=== Key Points ===\n");
    g_print("- GIO provides async versions of file operations\n");
    g_print("- Use g_file_load_contents_async() for reading\n");
    g_print("- Use g_file_replace_contents_async() for writing\n");
    g_print("- Operations complete in callbacks\n");
    g_print("- Main loop continues while I/O happens\n");
    
    return 0;
}
