//
//  emulator.c
//  z80
//
//  Created by Alexander Litvinov on 1/21/17.
//  Copyright © 2017 Alexander Litvinov. All rights reserved.
//

#include "utils.h"

// Read ROM file into memory
int read_rom_file(state_t *state, const char *filename, int offset) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: Couldn't open '%s'\n", filename);
    exit(EXIT_FAILURE);
  }

  printf("Loading %s with offset %04x\n", filename, offset);
  // Get the file size and read it into a memory buffer
  fseek(f, 0L, SEEK_END);
  int fsize = (int)ftell(f);
  fseek(f, 0L, SEEK_SET);
  
  fread(state->memory + offset, fsize, 1, f);
  fclose(f);
  return fsize;
}
