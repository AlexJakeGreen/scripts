
#ifndef debug_h
#define debug_h

#include "emulator.h"

void print_state(state_t *);

void print_memory(state_t *);

void unimplemented_instruction(state_t *);
void unimplemented_instruction_detailed(state_t *, int, const char *);

#endif
