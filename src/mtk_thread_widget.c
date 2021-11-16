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

  const double margin = MTK_SETTINGS(userData)->margin;
  const MtkColor color = {0.1, 0.2, 0.8};

  MtkProcessorInformation *processor = &MTK_USER_DATA(userData)->processorInformation;
  const unsigned int cpuCount = mtkProcessorInformationCpuCount(processor);

  /* Set up font */
  cairo_font_face_t *fontFace = cairo_toy_font_face_create("Ubuntu", CAIRO_FONT_SLANT_NORMAL,
                                                           CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_face(cairo, fontFace);
  cairo_set_font_size(cairo, 24.0);
  cairo_text_extents_t textExtents;

  /* Set up color */

  /* Set up size constants */
  const unsigned int rowElements = 4;
  const double strokeSize = 9.0;
  const double tileSize = ((double) width - (rowElements + 1) * margin) / rowElements ;
  const double radius = tileSize / 2 - strokeSize / 2;
  const double baseOffset = margin + tileSize / 2;
  const double advance = margin + tileSize;
  cairo_set_line_width(cairo, strokeSize);

  for (int i = 0; i < cpuCount; i++) {
    const double leftOffset = baseOffset + (i % rowElements) * advance;
    const double topOffset = baseOffset + (i / rowElements) * advance;
    const double usage = mtkProcessorInformationCpuUsage(processor, i);
    const double circleEnd = MTK_SETTINGS(userData)->circleStart + usage * (MTK_SETTINGS(userData)->circleEnd -
        MTK_SETTINGS(userData)->circleStart);

    cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->transparentAlpha);
    cairo_arc(cairo, leftOffset, topOffset, radius, MTK_SETTINGS(userData)->circleStart,
        MTK_SETTINGS(userData)->circleEnd);
    cairo_stroke(cairo);

    cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->baseAlpha);
    cairo_arc(cairo, leftOffset, topOffset, radius, MTK_SETTINGS(userData)->circleStart, circleEnd);
    cairo_stroke(cairo);
  }

  for (int i = 0; i < cpuCount; i++) {
    char string[3];
    const double leftOffset = baseOffset + (i % rowElements) * advance;
    const double topOffset = baseOffset + (i / rowElements) * advance;
    sprintf(string, "%u", i);
    cairo_set_source_rgba(cairo, MTK_SETTINGS(userData)->baseColor.r, MTK_SETTINGS(userData)->baseColor.g,
        MTK_SETTINGS(userData)->baseColor.b, MTK_SETTINGS(userData)->fontAlpha);
    cairo_text_extents(cairo, string, &textExtents);
    cairo_move_to(cairo, leftOffset - textExtents.width / 2, topOffset + textExtents.height / 2);
    cairo_show_text(cairo, string);
  }

  return FALSE;
}
