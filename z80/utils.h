#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "state.h"

// Read rom file into memory
int read_rom_file(uint8_t *data, const char *filename);

#endif
