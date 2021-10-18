#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>

typedef struct mtk_color_ {
  double r, g, b;
} MtkColor;

typedef struct mtk_refresh_data {
  GtkWidget *drawingArea;
  double usage;
} MtkRefreshData;

typedef struct mtk_user_data {
  MtkRefreshData refreshData;
} MtkUserData;

static double parseUsage() {
  char buffer[1024];
  FILE *fp = popen("top -b -n 1 | grep \"MiB Mem\"", "r");
  fgets(buffer, 1024, fp);
  pclose(fp);
  double total_mem, free_mem;
  char *parsing_head = &buffer[0];
  while (*parsing_head && !isdigit(*parsing_head)) parsing_head++;
  total_mem = strtod(parsing_head, &parsing_head);
  parsing_head++;
  while (*parsing_head && !isdigit(*parsing_head)) parsing_head++;
  free_mem = strtod(parsing_head, &parsing_head);
  parsing_head++;
  while (*parsing_head && !isdigit(*parsing_head)) parsing_head++;
  free_mem = strtod(parsing_head, &parsing_head);
  return (total_mem - free_mem) / total_mem;
}

static gboolean onRefresh(gpointer userData) {
  MtkRefreshData *refreshData = (MtkRefreshData *) userData;
  refreshData->usage = parseUsage();
  gtk_widget_queue_draw(refreshData->drawingArea);
  return G_SOURCE_CONTINUE;
}

static gboolean onDraw(GtkWidget *widget, cairo_t *cairo, gpointer userData) {
  guint width, height;
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  const double margin = 10.0;
  const double cx = width / 2.0, cy = height / 2.0;
  const double radius = MIN(width, height) / 2.0 - 2.0 * margin;
  const MtkColor circleColor = {0.1, 0.2, 0.8};
  const double usage = *(double *)userData;
  const double lineSpacing = 5.0;

  /* Base circle */
  cairo_set_source_rgba(cairo, circleColor.r, circleColor.b, circleColor.g, 0.3);
  cairo_set_line_width(cairo, 14);
  cairo_arc(cairo, cx, cy, radius, 0, 2 * G_PI);
  cairo_stroke(cairo);

  /* Filled circle */
  cairo_set_source_rgba(cairo, circleColor.r, circleColor.b, circleColor.g, 0.8);
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

static void onActivate(GtkApplication *application, gpointer userData) {
  MtkUserData *castedUserData = (MtkUserData *) userData;
  castedUserData->refreshData.usage = parseUsage();

  GtkWidget *window = gtk_application_window_new(application);
  gtk_window_set_title(GTK_WINDOW(window), "Monitor");
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);

  GtkWidget *drawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawingArea, 100, 100);
  g_signal_connect(drawingArea, "draw", G_CALLBACK(onDraw), &castedUserData->refreshData.usage);
  gtk_container_add(GTK_CONTAINER(window), drawingArea);

  castedUserData->refreshData.drawingArea = drawingArea;
  gdk_threads_add_timeout_seconds(2, G_SOURCE_FUNC(onRefresh), &castedUserData->refreshData);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  MtkUserData userData;
  GtkApplication *application = gtk_application_new("dev.musergi.monitor-widget", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(application, "activate", G_CALLBACK(onActivate), &userData);
  int status = g_application_run(G_APPLICATION(application), argc, argv);
  g_object_unref(application);
  return status;
}