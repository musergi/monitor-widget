//
// Created by musergi on 19/10/21.
//

#include "meminfo.h"
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

void mtkMemoryInformationRead(MtkMemoryInformation *memoryInformation) {
  char lineBuffer[BUFFER_SIZE];
  FILE* fp = fopen(MEMORY_INFORMATION_PATH, "r");
  if (!fp) {
    printf("Failed to read %s.\n", MEMORY_INFORMATION_PATH);
    exit(EXIT_FAILURE);
  }
  size_t offset = 0;
  void *rawPointer = (void*) memoryInformation;
  while(offset < sizeof(MtkMemoryInformation) && readLine(fp, lineBuffer) != EOF) {
    char *head = lineBuffer;
    while (*head && !isdigit(*head)) head++;
    *(unsigned int *)(rawPointer + offset) = strtol(head, NULL, 10);
    offset += sizeof(unsigned int);
  }
  fclose(fp);
}