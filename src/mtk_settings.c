#include "mtk_settings.h"

#define PI 3.14159264

void mtkSettingsLoadDefault(MtkSettings *settings) {
  settings->baseColor.r = 0.1;
  settings->baseColor.g = 0.2;
  settings->baseColor.b = 0.8;
  settings->transparentAlpha = 0.3;
  settings->baseAlpha = 0.8;
  settings->circleWidth = 15.0;
  settings->margin = 5.0;
  settings->circleStart = -PI / 2;
  settings->circleEnd = -PI / 2 + 2 * PI;
}