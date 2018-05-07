#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "utils.h"
#include "log.h"

#define RAM_SIZE 0xffff // 64k RAM

int main(int argc, const char * argv[]) {
  if (argc != 2) {
    printf("Usage: ./emul rom_file\n");
    exit(EXIT_FAILURE);
  }

  flush_log();

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

  /* Patch the memory of the program. Reset at 0x0000 is trapped by an
   * OUT which will stop emulation. CP/M bdos call 5 is trapped by an IN.
   * See Z80_INPUT_BYTE() and Z80_OUTPUT_BYTE() definitions in z80user.h.
   */
  state->memory[0] = 0xd3;       /* OUT N, A */
  state->memory[1] = 0x00;
  
  state->memory[5] = 0xdb;       /* IN A, N */
  state->memory[6] = 0x00;
  state->memory[7] = 0xc9;       /* RET */
        
  // printf("=====================\n");  
  int done = 0;
  
  while (done == 0) {
      done = emulate_op(state);
  }

  flush_log();
  return 0;
}
