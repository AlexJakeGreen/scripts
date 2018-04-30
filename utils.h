#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "state.h"

// Read rom file into memory
int read_rom_file(state_t *state, const char *filename, int offset);

#endif
