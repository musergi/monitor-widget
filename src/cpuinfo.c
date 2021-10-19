#include "cpuinfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 256

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
  sranddev();
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
  FILE *fp = fopen(CPU_INFORMATION_PATH, "r");
  if (!fp) {
    printf("Failed to read %s.\n", CPU_INFORMATION_PATH);
    mtkProcessorLoadRandom(processorInformation);
    return;
  }

  /* Read summary line */
  readLine(fp, lineBuffer);
  char *head = lineBuffer;
  for (size_t offset = 0; offset < sizeof(MtkCpuInformation); offset += sizeof(unsigned int)) {
    while (*head && !isdigit(*head)) head++;
    strtol(head, &head, 10);
    head++;
  }

  /* Read cpu lines */
  // TODO: Read cpu lines

  fclose(fp);
}