#include "mtk_ram_widget.h"

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData);

GtkWidget *mtkRamWidgetNew(MtkUserData *userData) {
  GtkWidget *drawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawingArea, 250, 250);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(onDraw), userData);
  return drawingArea;
}

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData) {
  guint width, height;
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  const double margin = 10.0;
  const double cx = width / 2.0, cy = height / 2.0;
  const double radius = MIN(width, height) / 2.0 - 2.0 * margin;
  const MtkColor circleColor = {0.1, 0.2, 0.8};
  MtkMemoryInformation *memory = &MTK_USER_DATA(userData)->memoryInformation;
  const double usage = (double)(memory->memoryTotal - memory->memoryAvailable) / memory->memoryTotal;
  const double lineSpacing = 5.0;

  /* Base circle */
  cairo_set_source_rgba(cairo, circleColor.r, circleColor.g, circleColor.b, 0.3);
  cairo_set_line_width(cairo, 14);
  cairo_arc(cairo, cx, cy, radius, 0, 2 * G_PI);
  cairo_stroke(cairo);

  /* Filled circle */
  cairo_set_source_rgba(cairo, circleColor.r, circleColor.g, circleColor.b, 0.8);
  cairo_set_line_width(cairo, 14);
  cairo_arc(cairo, cx, cy, radius, -G_PI / 2, 2 * G_PI * usage - G_PI / 2);
  cairo_stroke(cairo);

  cairo_font_face_t *fontFace = cairo_toy_font_face_create("Ubuntu", CAIRO_FONT_SLANT_NORMAL,
                                                           CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_face(cairo, fontFace);
  cairo_text_extents_t textExtents;

  /* RAM text */
  cairo_set_font_size(cairo, 40.0);
  cairo_text_extents(cairo, "RAM", &textExtents);
  cairo_move_to(cairo, cx - textExtents.width / 2, cy - lineSpacing / 2);
  cairo_show_text(cairo, "RAM");

  /* Percentage text */
  char textBuffer[10];
  const unsigned int percent = (unsigned int) (usage * 100.0);
  sprintf(textBuffer, "%u%%", percent);
  cairo_set_font_size(cairo, 30.0);
  cairo_text_extents(cairo, textBuffer, &textExtents);
  cairo_move_to(cairo, cx - textExtents.width / 2, cy + textExtents.height + lineSpacing / 2);
  cairo_show_text(cairo, textBuffer);

  return FALSE;
}