#include "cpuinfo.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BUFFER_SIZE 256

void mtkCpuInformationString(MtkCpuInformation *cpuInformation, char *string, size_t stringSize) {
  sprintf(string, "{user:%u,nice:%u,system:%u,idle:%u,iowait:%u,irq:%u,softirq:%u}", cpuInformation->user,
      cpuInformation->nice, cpuInformation->system, cpuInformation->idle, cpuInformation->iowait, cpuInformation->irq,
      cpuInformation->softirq);
}

unsigned int mtkCpuInformationTotal(MtkCpuInformation *cpuInformation) {
  return cpuInformation->user + cpuInformation->nice + cpuInformation->system + cpuInformation->idle +
      cpuInformation->iowait + cpuInformation->irq + cpuInformation->softirq;
}

static int readLine(FILE *fp, char *buffer) {
  int i = 0;
  while (i < BUFFER_SIZE - 1) {
    int status = fscanf(fp, "%c", &buffer[i]);
    if (status == EOF) {
      buffer[i] = '\0';
      return EOF;
    }
    if (buffer[i] == '\n') {
      buffer[i] = '\0';
      return i + 1;
    }
    i++;
  }
  buffer[i] = '\0';
  return BUFFER_SIZE - 1;
}

static void mtkProcessorLoadRandom(MtkProcessorInformation *processorInformation) {
  srand(time(NULL));
  processorInformation->cpuCount = 8 + rand() % 4;
  memset(&processorInformation->aggregation, 0, sizeof(MtkCpuInformation));
  for (int i = 0; i < processorInformation->cpuCount; i++) {
    processorInformation->cpus[i].user = rand() % 100000;
    processorInformation->cpus[i].nice = rand() % 100;
    processorInformation->cpus[i].system = rand() % 2000;
    processorInformation->cpus[i].idle = rand() % 100000;
    processorInformation->cpus[i].iowait = rand() % 4000;
    processorInformation->cpus[i].irq = rand() % 100;
    processorInformation->cpus[i].softirq = rand() % 500;

    processorInformation->aggregation.user += processorInformation->cpus[i].user;
    processorInformation->aggregation.nice += processorInformation->cpus[i].nice;
    processorInformation->aggregation.system += processorInformation->cpus[i].system;
    processorInformation->aggregation.idle += processorInformation->cpus[i].idle;
    processorInformation->aggregation.iowait += processorInformation->cpus[i].iowait;
    processorInformation->aggregation.irq += processorInformation->cpus[i].irq;
    processorInformation->aggregation.softirq += processorInformation->cpus[i].softirq;
  }
}

void mtkProcessorInformationRead(MtkProcessorInformation *processorInformation) {
  char lineBuffer[BUFFER_SIZE];

  memcpy(&processorInformation->oldAggregation, &processorInformation->aggregation, sizeof(MtkCpuInformation));
  memcpy(&processorInformation->oldCpus, &processorInformation->cpus, sizeof(MtkCpuInformation) *
      CPU_INFORMATION_MAX_CPU_COUNT);

  FILE *fp = fopen(CPU_INFORMATION_PATH, "r");
  if (!fp) {
    printf("Failed to read %s.\n", CPU_INFORMATION_PATH);
    mtkProcessorLoadRandom(processorInformation);
    return;
  }

  /* Read summary line */
  readLine(fp, lineBuffer);
  char *head = lineBuffer;
  void *rawPointer = &processorInformation->aggregation;
  for (size_t offset = 0; offset < sizeof(MtkCpuInformation); offset += sizeof(unsigned int)) {
    while (*head && !isdigit(*head)) head++;
    *(unsigned int *) (rawPointer + offset) = strtol(head, &head, 10);
  }

  /* Read cpu lines */
  processorInformation->cpuCount = 0;
  while (readLine(fp, lineBuffer) != EOF && strncmp("cpu", lineBuffer, 3) == 0 &&
         processorInformation->cpuCount < CPU_INFORMATION_MAX_CPU_COUNT) {
    head = lineBuffer;
    rawPointer = &processorInformation->cpus[processorInformation->cpuCount];
    head = strchr(head, ' '); // Skip to first space
    for (size_t offset = 0; offset < sizeof(MtkCpuInformation); offset += sizeof(unsigned int)) {
      while (*head && !isdigit(*head)) head++;
      *(unsigned int *) (rawPointer + offset) = strtol(head, &head, 10);
    }
    processorInformation->cpuCount++;
  }

  fclose(fp);
}

double mtkProcessorInformationAggregatedUsage(MtkProcessorInformation *processorInformation) {
  unsigned int idleDelta = processorInformation->aggregation.idle - processorInformation->oldAggregation.idle;
  unsigned int totalDelta = mtkCpuInformationTotal(&processorInformation->aggregation) -
      mtkCpuInformationTotal(&processorInformation->oldAggregation);
  return (double) (totalDelta - idleDelta) / totalDelta;
}
