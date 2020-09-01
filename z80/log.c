#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define LOG_BUFFER_SIZE 256 * 1024
char log_buffer[LOG_BUFFER_SIZE];

void print_log(const char *message) {
  size_t buf_len = strlen(log_buffer);
  size_t message_len = strlen(message);
  if (buf_len + message_len > LOG_BUFFER_SIZE) {
    flush_log();
    log_buffer[0] = '\0';
  }
  strcat(log_buffer, message);
}

void flush_log() {
  FILE *f = fopen("emul.log", "a");
  if (f == NULL) {
    printf("Error opening log file!\n");
    exit(EXIT_SUCCESS);
  }
  fseek(f, 0L, SEEK_END);
  fprintf(f, "%s", log_buffer);
  fclose(f);
}
