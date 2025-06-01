#include <math.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gio/gio.h>

#define PREVIEW_MAX_WIDTH 800
#define PREVIEW_MAX_HEIGHT 600

static GtkWidget *window = NULL;

void preview_file(const char *path) {
    if (window) gtk_widget_destroy(window);

    // 创建窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(window), PREVIEW_MAX_WIDTH, PREVIEW_MAX_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), container);

    GFile *file = g_file_new_for_path(path);
    GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
    const char *mime = g_file_info_get_content_type(info);

    if (g_str_has_prefix(mime, "image/")) {
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
        if (pixbuf) {
            int width = gdk_pixbuf_get_width(pixbuf);
            int height = gdk_pixbuf_get_height(pixbuf);
            double scale = fmin((double)PREVIEW_MAX_WIDTH / width, (double)PREVIEW_MAX_HEIGHT / height);
            if (scale < 1.0) {
                GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, width * scale, height * scale, GDK_INTERP_BILINEAR);
                GtkWidget *image = gtk_image_new_from_pixbuf(scaled);
                gtk_box_pack_start(GTK_BOX(container), image, TRUE, TRUE, 0);
                g_object_unref(scaled);
            } else {
                GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
                gtk_box_pack_start(GTK_BOX(container), image, TRUE, TRUE, 0);
            }
            g_object_unref(pixbuf);
        }
    } else if (g_str_has_prefix(mime, "text/")) {
        gchar *content = NULL;
        g_file_load_contents(file, NULL, &content, NULL, NULL, NULL);
        if (content) {
            GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
            GtkWidget *textview = gtk_text_view_new();
            gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);
            gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
            gtk_container_add(GTK_CONTAINER(scroll), textview);
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
            gtk_text_buffer_set_text(buffer, content, -1);
            gtk_box_pack_start(GTK_BOX(container), scroll, TRUE, TRUE, 0);
            g_free(content);
        }
    } else {
        GtkWidget *label = gtk_label_new("Unsupported file type");
        gtk_box_pack_start(GTK_BOX(container), label, TRUE, TRUE, 0);
    }

    g_object_unref(file);
    g_object_unref(info);

    gtk_widget_show_all(window);
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_space || event->keyval == GDK_KEY_Escape) {
        gtk_widget_destroy(window);
        gtk_main_quit();
    }
    return FALSE;
}

gboolean on_focus_out(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    gtk_widget_destroy(window);
    gtk_main_quit();
    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    if (argc < 2) {
        g_printerr("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    preview_file(argv[1]);

    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "focus-out-event", G_CALLBACK(on_focus_out), NULL);

    gtk_main();
    return 0;
}
