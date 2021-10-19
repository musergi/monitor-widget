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

void mtkProcessorInformationRead(MtkProcessorInformation *processorInformation) {
  char lineBuffer[BUFFER_SIZE];
  FILE *fp = fopen(CPU_INFORMATION_PATH, "r");
  if (!fp) {
    printf("Failed to read %s.\n", CPU_INFORMATION_PATH);
    exit(EXIT_FAILURE);
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