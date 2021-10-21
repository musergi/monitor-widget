//
// Created by musergi on 19/10/21.
//

#ifndef MONITOR_WIDGET_MTK_USER_DATA_H
#define MONITOR_WIDGET_MTK_USER_DATA_H

#include <gtk/gtk.h>
#include "meminfo.h"
#include "cpuinfo.h"
#include "mtk_settings.h"

typedef struct mtk_user_data_ {
  GtkWidget *ramDrawingArea;
  GtkWidget *cpuDrawingArea;
  GtkWidget *threadDrawingArea;
  MtkMemoryInformation memoryInformation;
  MtkProcessorInformation processorInformation;
  MtkSettings settings;
} MtkUserData;

#define MTK_USER_DATA(x) ((MtkUserData *)(x))

#endif //MONITOR_WIDGET_MTK_USER_DATA_H
