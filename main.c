#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "utils.h"

#define RAM_SIZE 0xffff // 64k RAM

int main(int argc, const char * argv[]) {
  if (argc != 2) {
    printf("Usage: ./emul rom_file\n");
    exit(EXIT_FAILURE);
  }

  uint16_t rom_offset = 0x0100;

  state_t *state = calloc(1, sizeof(state_t));
  state->memory = calloc(1, RAM_SIZE);
  state->r_af = 0;
  state->r_bc = 0;
  state->r_de = 0;
  state->r_hl = 0;
  state->r_sp = 0x0000;
  state->r_pc = rom_offset;
  
  read_rom_file(state, argv[1], rom_offset);
  printf("=====================\n");  
  int done = 0;
  
  while (done == 0) {
      done = emulate_op(state);
  }
    
  return 0;
}
