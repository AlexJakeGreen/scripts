
#include "utils.h"

// Read ROM file into memory
int read_rom_file(uint8_t *data, const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: Couldn't open '%s'\n", filename);
    exit(EXIT_FAILURE);
  }

  printf("Loading %s\n", filename);
  // Get the file size and read it into a memory buffer
  fseek(f, 0L, SEEK_END);
  int fsize = (int)ftell(f);
  fseek(f, 0L, SEEK_SET);
  
  fread(data, fsize, 1, f);
  fclose(f);
  return fsize;
}
