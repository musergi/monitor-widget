#include "mtk_cpu_widget.h"

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData);

GtkWidget *mtkCpuWidgetNew(MtkUserData *userData) {
  GtkWidget *drawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawingArea, 250, 250);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(onDraw), userData);
  return drawingArea;
}

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData) {
  guint width, height;
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  /* Base circle */
  const double cx = width / 2.0, cy = height / 2.0;
  const double radius =
          MIN(width, height) / 2.0 - MTK_SETTINGS(userData)->margin - MTK_SETTINGS(userData)->circleWidth / 2;
  cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
                        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->transparentAlpha);
  cairo_set_line_width(cairo, MTK_SETTINGS(userData)->circleWidth);
  cairo_arc(cairo, cx, cy, radius, MTK_SETTINGS(userData)->circleStart, MTK_SETTINGS(userData)->circleEnd);
  cairo_stroke(cairo);

  /* Filled circle */
  cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
                        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->baseAlpha);
  cairo_set_line_width(cairo, MTK_SETTINGS(userData)->circleWidth);
  MtkProcessorInformation *processor = &MTK_USER_DATA(userData)->processorInformation;
  unsigned int total = processor->aggregation.idle + processor->aggregation.user + processor->aggregation.iowait +
                       processor->aggregation.irq + processor->aggregation.nice + processor->aggregation.softirq +
                       processor->aggregation.system;
  const double usage = (double) (total - processor->aggregation.idle) / total;
  const double end = MTK_SETTINGS(userData)->circleStart +
                     usage * (MTK_SETTINGS(userData)->circleEnd - MTK_SETTINGS(userData)->circleStart);
  cairo_arc(cairo, cx, cy, radius, MTK_SETTINGS(userData)->circleStart, end);
  cairo_stroke(cairo);

  /* Set up font */
  cairo_font_face_t *fontFace = cairo_toy_font_face_create("Ubuntu", CAIRO_FONT_SLANT_NORMAL,
                                                           CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_face(cairo, fontFace);
  cairo_text_extents_t textExtents;

  /* CPU text */
  cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
                        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->fontAlpha);
  cairo_set_font_size(cairo, MTK_SETTINGS(userData)->centerFontSize);
  cairo_text_extents(cairo, "CPU", &textExtents);
  cairo_move_to(cairo, cx - textExtents.width / 2, cy - MTK_SETTINGS(userData)->textSpacing / 2);
  cairo_show_text(cairo, "CPU");

  /* Percentage text */
  char textBuffer[10];
  const unsigned int percent = (unsigned int) (usage * 100.0);
  sprintf(textBuffer, "%u%%", percent);
  cairo_set_font_size(cairo, MTK_SETTINGS(userData)->percentageFontSize);
  cairo_text_extents(cairo, textBuffer, &textExtents);
  cairo_move_to(cairo, cx - textExtents.width / 2, cy + textExtents.height + MTK_SETTINGS(userData)->textSpacing / 2);
  cairo_show_text(cairo, textBuffer);

  return FALSE;
}