#include "mtk_settings.h"

void mtkSettingsLoadDefaults(MtkSettings *settings) {
  settings->baseColor.r = 0.1;
  settings->baseColor.g = 0.2;
  settings->baseColor.b = 0.8;
  settings->margin = 10.0;
}