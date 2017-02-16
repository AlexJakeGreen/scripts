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

// registers
typedef struct  __attribute__((__packed__)) {
  // data group
  union { // accumulator
    uint16_t ax;
    struct {
      uint8_t al;
      uint8_t ah;
    };
  };
  union { // base
    uint16_t bx;
    struct {
      uint8_t bl;
      uint8_t bh;
    };
  };
  union { // count
    uint16_t cx;
    struct {
      uint8_t cl;
      uint8_t ch;
    };
  };
  union { // data
    uint16_t dx;
    struct {
      uint8_t dl;
      uint8_t dh;
    };
  };
  
  // pointer and index group
  uint16_t sp; // stack pointer
  uint16_t bp; // base pointer
  uint16_t si; // source index
  uint16_t di; // destination index
  
  // segment registers
  uint16_t cs; // code segment - current code segment
  uint16_t ds; // data segment - current stack segment
  uint16_t ss; // stack segment - current data segment
  uint16_t es; // extra segment - current extra segment (storage)
  
  uint16_t ip; // instruction pointer, analogous to PC in 8080
  
  union {
    uint16_t flags;
    struct {
      uint8_t     c: 1; // carry
      uint8_t    u1: 1;
      uint8_t     p: 1; // parity
      uint8_t    u2: 1;
      uint8_t     a: 1; // aux carry
      uint8_t    u3: 1;
      uint8_t     z: 1; // zero
      uint8_t     s: 1; // sign
      uint8_t     t: 1; // trap flag
      uint8_t     i: 1; // interrupt-enable
      uint8_t     d: 1; // direction
      uint8_t     o: 1; // trap
      uint8_t    u4: 4; // padding
    } flag;
  };
  
  uint8_t *memory;

  uint8_t int_enable;
} state_t;

int emulate_op(state_t *state);

extern int disassemble_opcode_8086(state_t *, int);

#endif /* emulator_h */
