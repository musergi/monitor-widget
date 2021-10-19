#include "mtk_thread_widget.h"

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData);

GtkWidget *mtkThreadWidgetNew(MtkUserData *userData) {
  GtkWidget *drawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawingArea, 500, 250);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(onDraw), userData);
  return drawingArea;
}

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData) {
  guint width, height;
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  const double margin = 10.0;
  const MtkColor color = {0.1, 0.2, 0.8};
  const double lineSpacing = 6.0;
  const unsigned int cpu_count = 8;
  const double usage[] = {0.2, 0.3, 0.8, 0.2, 0.4, 0.6, 0.1, 0.9};
  const double lineHeight = (height - 2 * margin - (cpu_count - 1) * lineSpacing) / cpu_count;
  const char *texts[] = {"CPU0", "CPU1", "CPU2", "CPU3", "CPU4", "CPU5", "CPU6", "CPU7"};

  /* Set up font */
  cairo_font_face_t *fontFace = cairo_toy_font_face_create("Ubuntu", CAIRO_FONT_SLANT_NORMAL,
                                                           CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_face(cairo, fontFace);
  cairo_set_font_size(cairo, lineHeight);
  cairo_text_extents_t textExtents;

  double maxTextWidth = 0.0;
  for (int i = 0; i < cpu_count; i++) {
    cairo_text_extents(cairo, texts[i], &textExtents);
    if (textExtents.width > maxTextWidth) maxTextWidth = textExtents.width;
  }

  const double lineOffset = 2 * margin + maxTextWidth;
  const double lineWidth = width - 3 * margin - maxTextWidth;
  const double lineAdvance = (lineHeight + lineSpacing);

  for (int i = 0; i < cpu_count; i++) {
    const double start = margin + i * lineAdvance;
    cairo_set_source_rgba(cairo, color.r, color.g, color.b, 0.3);
    cairo_rectangle(cairo, lineOffset, start, lineWidth, lineHeight);
    cairo_fill(cairo);

    cairo_set_source_rgba(cairo, color.r, color.g, color.b, 0.8);
    cairo_rectangle(cairo, lineOffset, start, lineWidth * usage[i], lineHeight);
    cairo_fill(cairo);

    cairo_move_to(cairo, margin, start + lineHeight / 2 + textExtents.height / 2);
    cairo_show_text(cairo, texts[i]);
  }

  return FALSE;
}