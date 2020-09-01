//
//  emulator.h
//  z80
//
//  Created by Alexander Litvinov on 1/21/17.
//  Copyright © 2017 Alexander Litvinov. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include "emulator.h"

// Read rom file into memory
int read_rom_file(state_t *state, const char *filename, int offset);

#endif
