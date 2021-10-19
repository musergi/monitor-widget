#include <gtk/gtk.h>
#include "mtk_user_data.h"
#include "mtk_ram_widget.h"

static gboolean onRefresh(gpointer userData) {
  mtkMemoryInformationRead(&MTK_USER_DATA(userData)->memoryInformation);
  mtkProcessorInformationRead(&MTK_USER_DATA(userData)->processorInformation);
  gtk_widget_queue_draw(MTK_USER_DATA(userData)->drawingArea);
  return G_SOURCE_CONTINUE;
}

static void onActivate(GtkApplication *application, gpointer userData) {
  mtkMemoryInformationRead(&MTK_USER_DATA(userData)->memoryInformation);
  mtkProcessorInformationRead(&MTK_USER_DATA(userData)->processorInformation);

  GtkWidget *window = gtk_application_window_new(application);
  gtk_window_set_title(GTK_WINDOW(window), "Monitor");
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  GtkWidget *drawingArea = mtkRamWidgetNew(userData);
  gtk_container_add(GTK_CONTAINER(window), drawingArea);

  MTK_USER_DATA(userData)->drawingArea = drawingArea;
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