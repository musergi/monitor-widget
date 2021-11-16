#ifndef MONITOR_WIDGET_CPUINFO_H
#define MONITOR_WIDGET_CPUINFO_H

#define CPU_INFORMATION_PATH "/proc/stat"
#define CPU_INFORMATION_PATH_UNIT "Jiffies" // 1/100 sec
#define CPU_INFORMATION_MAX_CPU_COUNT 32

#include <stdlib.h>

typedef struct mtk_cpu_information_ {
   unsigned int user;
   unsigned int nice;
   unsigned int system;
   unsigned int idle;
   unsigned int iowait;
   unsigned int irq;
   unsigned int softirq;
} MtkCpuInformation;

void mtkCpuInformationString(MtkCpuInformation *cpuInformation, char *string, size_t stringSize);
unsigned int mtkCpuInformationTotal(MtkCpuInformation *cpuInformation);
double mtkCpuInformationUsage(MtkCpuInformation *oldInformation, MtkCpuInformation *newInformation);

typedef struct mtk_processor_information_ {
  unsigned int cpuCount;
  MtkCpuInformation aggregation;
  MtkCpuInformation cpus[CPU_INFORMATION_MAX_CPU_COUNT];
  MtkCpuInformation oldAggregation;
  MtkCpuInformation oldCpus[CPU_INFORMATION_MAX_CPU_COUNT];
} MtkProcessorInformation;

void mtkProcessorInformationRead(MtkProcessorInformation *processorInformation);
double mtkProcessorInformationAggregatedUsage(MtkProcessorInformation *processorInformation);
unsigned int mtkProcessorInformationCpuCount(MtkProcessorInformation *processorInformation);
double mtkProcessorInformationCpuUsage(MtkProcessorInformation *processorInformation, unsigned int index);

#endif //MONITOR_WIDGET_CPUINFO_H
