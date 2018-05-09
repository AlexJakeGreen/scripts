#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "utils.h"

int main(int argc, const char * argv[]) {
  if (argc != 2) {
    printf("Usage: ./emul rom_file\n");
    exit(EXIT_FAILURE);
  }

  uint8_t data[0xffff];
  int fsize = read_rom_file(data, argv[1]);
  
  uint16_t rom_offset = 0x0100;
  em_init(data, fsize, rom_offset);
  
  /* Patch the memory of the program. Reset at 0x0000 is trapped by an
   * OUT which will stop emulation. CP/M bdos call 5 is trapped by an IN.
   * See Z80_INPUT_BYTE() and Z80_OUTPUT_BYTE() definitions in z80user.h.
   */
  em_setmem(0, 0xd3);
  em_setmem(1, 0x00);
  em_setmem(5, 0xdb);
  em_setmem(6, 0x00);
  em_setmem(7, 0xc9);
        
  int done = 0;
  
  while (done == 0) {
      done = emulate_op();
  }

  return 0;
}
