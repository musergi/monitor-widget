#ifndef MONITOR_WIDGET_MTK_SETTINGS_H
#define MONITOR_WIDGET_MTK_SETTINGS_H

typedef struct mtk_color_ {
  double r, g, b;
} MtkColor;

typedef struct mtk_settings_ {
  MtkColor baseColor;
  double transparentAlpha;
  double baseAlpha;
  double circleWidth;
  double margin;
} MtkSettings;

void mtkSettingsLoadDefault(MtkSettings *settings);

#endif //MONITOR_WIDGET_MTK_SETTINGS_H
