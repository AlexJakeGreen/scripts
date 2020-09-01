
#ifndef disassembler_h
#define disassembler_h

#include <inttypes.h>
#include "emulator.h"
#include "debug.h"
#include "utils.h"

void disassemble_opcode_8086(state_t *state, int ip);

#endif
