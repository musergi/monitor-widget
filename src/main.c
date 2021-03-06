#include <gtk/gtk.h>
#include "mtk_user_data.h"
#include "mtk_ram_widget.h"
#include "mtk_cpu_widget.h"
#include "mtk_thread_widget.h"

static gboolean onRefresh(gpointer userData) {
  mtkMemoryInformationRead(&MTK_USER_DATA(userData)->memoryInformation);
  mtkProcessorInformationRead(&MTK_USER_DATA(userData)->processorInformation);
  gtk_widget_queue_draw(MTK_USER_DATA(userData)->ramDrawingArea);
  gtk_widget_queue_draw(MTK_USER_DATA(userData)->cpuDrawingArea);
  gtk_widget_queue_draw(MTK_USER_DATA(userData)->threadDrawingArea);
  return G_SOURCE_CONTINUE;
}

static void onActivate(GtkApplication *application, gpointer userData) {
  mtkMemoryInformationRead(&MTK_USER_DATA(userData)->memoryInformation);
  mtkProcessorInformationRead(&MTK_USER_DATA(userData)->processorInformation);
  mtkSettingsLoadDefault(&MTK_USER_DATA(userData)->settings);

  GtkWidget *window = gtk_application_window_new(application);
  gtk_window_set_title(GTK_WINDOW(window), "Monitor");
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DESKTOP);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  int windowPosition[2];
  int windowSize[2];
  gtk_window_get_position(GTK_WINDOW(window), &windowPosition[0], &windowPosition[1]);
  gtk_window_move(GTK_WINDOW(window), windowPosition[0] * 2 - 300, 30);
  printf("Window position (%d, %d)\n", windowPosition[0], windowPosition[1]);

  gtk_widget_set_app_paintable(window, TRUE);

  GdkScreen *screen = gtk_widget_get_screen(window);
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  if (!visual) exit(EXIT_FAILURE);
  gtk_widget_set_visual(window, visual);

  GtkWidget *gridContainer = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), gridContainer);

  GtkWidget *ramDrawingArea = mtkRamWidgetNew(userData);
  gtk_grid_attach(GTK_GRID(gridContainer), ramDrawingArea, 0, 0, 1, 1);

  GtkWidget *cpuDrawingArea = mtkCpuWidgetNew(userData);
  gtk_grid_attach(GTK_GRID(gridContainer), cpuDrawingArea, 1, 0, 1, 1);

  GtkWidget *threadDrawingArea = mtkThreadWidgetNew(userData);
  gtk_grid_attach(GTK_GRID(gridContainer), threadDrawingArea, 0, 1, 2, 1);

  MTK_USER_DATA(userData)->ramDrawingArea = ramDrawingArea;
  MTK_USER_DATA(userData)->cpuDrawingArea = cpuDrawingArea;
  MTK_USER_DATA(userData)->threadDrawingArea = threadDrawingArea;
  gdk_threads_add_timeout_seconds(2, G_SOURCE_FUNC(onRefresh), userData);

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