//
//  main.c
//  z80
//
//  Created by Alexander Litvinov on 1/21/17.
//  Copyright © 2017 Alexander Litvinov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "utils.h"

#define RAM_SIZE 0xffff // 64k RAM

int main(int argc, const char * argv[]) {
  if (argc < 2 || argc > 3) {
    printf("Usage: ./emul rom_file offset\n");
    exit(EXIT_FAILURE);
  }

  uint16_t rom_offset = 0x0; 

  if (argc > 2) {
    rom_offset = strtol(argv[2], NULL, 16);
  }
  
  state_t *state = calloc(1, sizeof(state_t));
  state->memory = calloc(1, RAM_SIZE);
  state->sp = 0x0000;
  state->pc = rom_offset;
  
  read_rom_file(state, argv[1], rom_offset);
    
  int done = 0;
    
  while (done == 0) {
    done = emulate_op(state);
  }
    
  return 0;
}
