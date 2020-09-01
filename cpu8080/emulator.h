//
//  emulator.h
//  z80
//
//  Created by Alexander Litvinov on 1/21/17.
//  Copyright © 2017 Alexander Litvinov. All rights reserved.
//

#ifndef emulator_h
#define emulator_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    uint8_t z:1; /* Zero - If the result of an instruction has the value 0,
                  this flag is set; otherwise it is reset */
    uint8_t s:1; /* Sign - If the most significant bit of the result of the operation
                  has the value 1, this flag is set; otherwise it is reset */
    uint8_t p:1; /* Parity - if the module 2 sum of the bits of the result
                  of the operation is 0, (i.e., if the result has even parity), this flag is set;
                  otherwise it is reset(i.e., if the result has odd parity) */
    uint8_t cy:1; /* Carry - If the instruction resulted in a carry (from addition),
                   or a borrow (from substraction or a comparison) out of the high-order bit,
                   this flag is set; otherwise it is reset */
    uint8_t ac:1; /* Auxiliary Carry - If the instruction caused a carry out of bit 3
                   and into bit 4 of the resulting value, the auxiliary carry is set;
                   otherwise it is reset. This flag is affected by single precision additions,
                   substractions, increments, decrements, but is principally used with
                   additions and increments preceding a DAA (Decimal Adjust Accumulator) instruction */
    uint8_t pad:3;
} condition_codes_t;

typedef struct  __attribute__((__packed__)) {
  union {
    uint16_t bc;
    struct {
      uint8_t c;
      uint8_t b;
    };
  };
  union {
    uint16_t de;
    struct {
      uint8_t e;
      uint8_t d;
    };
  };
  union {
    uint16_t hl;
    struct {
      uint8_t l;
      uint8_t h;
    };
  };
  uint8_t a;

  uint16_t sp;
  uint16_t pc;
  uint8_t *memory;
  union {
    uint8_t psw;
    condition_codes_t cc;
  };
  uint8_t int_enable;
} state_t;

// Emulate op code
int emulate_op(state_t *state);

// Debug current state
void print_state(state_t *state);

// Disassemble opcode
int disassemble_opcode_8080(unsigned char *codebuffer, int pc);

#endif /* emulator_h */
