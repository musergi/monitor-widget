#include "mtk_settings.h"

#define PI 3.14159264

void mtkSettingsLoadDefault(MtkSettings *settings) {
  settings->baseColor.r = 0.1;
  settings->baseColor.g = 0.2;
  settings->baseColor.b = 0.8;
  settings->transparentAlpha = 0.2;
  settings->baseAlpha = 0.5;
  settings->fontAlpha = 0.8;
  settings->circleWidth = 30.0;
  settings->margin = 10.0;
  settings->circleStart = -PI / 2;
  settings->circleEnd = -PI / 2 + 2 * PI;
  settings->centerFontSize = 40.0;
  settings->percentageFontSize = 30.0;
  settings->textSpacing = 8.0;
}