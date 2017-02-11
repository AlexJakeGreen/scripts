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

/*
  info
  http://mysite.du.edu/~etuttle/math/8086.htm#Opco
 */

int main(int argc, const char * argv[]) {

  state_t *state = calloc(1, sizeof(state_t));
  state->memory = calloc(1, RAM_SIZE);
  state->ip = 0x0000;
  state->cs = 0xffff;
  state->ds = 0x0000;
  state->ss = 0x0000;
  state->es = 0x0000;
  
  read_rom_file(state, argv[1], 0x0);

  int done = 0;
  
  while (0 == done) {
    done = emulate_op(state);
  }

  return 0;
}
