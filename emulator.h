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

  struct {
    // status flags
    uint8_t cf :1; // carry
    uint8_t pf :1; // parity
    uint8_t af :1; // aux carry
    uint8_t zf :1; // zero
    uint8_t sf :1; // sign
    uint8_t of :1; // overflow
    
    // control flags
    uint8_t _if :1; // interrupt-enable
    uint8_t df :1; // direction
    uint8_t tf :1; // trap

    uint8_t pad :7; // padding
  } flags;

  
  uint8_t *memory;

  uint8_t int_enable;
} state_t;

// Disassemble opcode
int disassemble_opcode_8086(unsigned char *codebuffer, int pc);

#endif /* emulator_h */
