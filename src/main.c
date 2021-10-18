#include <string.h>
#include <gtk/gtk.h>

typedef struct mtk_color_ {
    double r, g, b;
} mtk_color_t;

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData) {
    guint width, height;
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    const double margin = 10.0;
    const double cx = width / 2.0, cy = height / 2.0;
    const double radius = MIN(width, height) / 2.0 - 2.0 * margin;
    const mtk_color_t circleColor = {0.1, 0.2, 0.8};
    const double usage = 0.42;
    const double lineSpacing = 5.0;

    /* Base circle */
    cairo_set_source_rgba(cairo, circleColor.r, circleColor.b, circleColor.g, 0.5);
    cairo_set_line_width(cairo, 14);
    cairo_arc(cairo, cx, cy, radius, 0, 2 * G_PI);
    cairo_stroke(cairo);

    /* Filled circle */
    cairo_set_source_rgba(cairo, circleColor.r, circleColor.b, circleColor.g, 0.8);
    cairo_set_line_width(cairo, 14);
    cairo_arc(cairo, cx, cy, radius, -G_PI / 2, 2 * G_PI * usage - G_PI / 2);
    cairo_stroke(cairo);

    cairo_font_face_t *fontFace = cairo_toy_font_face_create("Monospace", CAIRO_FONT_SLANT_NORMAL,
                                                             CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_face(cairo, fontFace);
    cairo_text_extents_t textExtents;

    /* RAM text */
    cairo_set_font_size(cairo, 28.0);
    cairo_text_extents(cairo, "RAM", &textExtents);
    cairo_move_to(cairo, cx - textExtents.width / 2, cy - lineSpacing / 2);
    cairo_show_text(cairo, "RAM");

    /* Percentage text */
    char textBuffer[10];
    const unsigned int percent = (unsigned int) (usage * 100.0);
    sprintf(textBuffer, "%u%%", percent);
    cairo_set_font_size(cairo, 24.0);
    cairo_text_extents(cairo, textBuffer, &textExtents);
    cairo_move_to(cairo, cx - textExtents.width / 2, cy + textExtents.height + lineSpacing / 2);
    cairo_show_text(cairo, textBuffer);

    return FALSE;
}

static void onActivate(GtkApplication *application, gpointer userData) {
    GtkWidget *window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window), "Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);

    GtkWidget *drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, 100, 100);
    g_signal_connect(drawingArea, "draw", G_CALLBACK(onDraw), NULL);
    gtk_container_add(GTK_CONTAINER(window), drawingArea);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *application = gtk_application_new("dev.musergi.monitor-widget", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(onActivate), NULL);
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return status;
}