#ifndef MONITOR_WIDGET_MEMINFO_H
#define MONITOR_WIDGET_MEMINFO_H

#define MEMORY_INFORMATION_PATH "/proc/meminfo"
#define MEMORY_INFORMATION_UNIT "kB"

typedef struct mtk_memory_information_ {
  unsigned int memoryTotal;
  unsigned int memoryFree;
  unsigned int memoryAvailable;
  unsigned int buffers;
  unsigned int cached;
  unsigned int swapCached;
  unsigned int active;
  unsigned int inactive;
  unsigned int activeAnon;
  unsigned int inactiveAnon;
  unsigned int activeFile;
  unsigned int inactiveFile;
  unsigned int unevictable;
  unsigned int mLocked;
  unsigned int swapTotal;
  unsigned int swapFree;
  // TODO: Add addition fields
} MtkMemoryInformation;

void mtkMemoryInformationRead(MtkMemoryInformation *memoryInformation);

#endif //MONITOR_WIDGET_MEMINFO_H
