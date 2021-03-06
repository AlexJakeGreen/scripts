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
    printf("Usage: ./disasm rom_file [offset]\n");
    exit(EXIT_FAILURE);
  }

  uint16_t offset = 0x0;
  if (argc > 2)
    offset = (uint16_t)strtol(argv[2], NULL, 16);
  
  state_t *state = calloc(1, sizeof(size_t));
  state->memory = calloc(1, RAM_SIZE);
  state->pc = offset;
  
  int size = read_rom_file(state, argv[1], offset);
  size += offset;
  
  int steps = 1;
  while (state->pc < size) {
    steps = disassemble_opcode_8080(state->memory, state->pc);
    state->pc += steps;
  }
  
  return 0;
}
