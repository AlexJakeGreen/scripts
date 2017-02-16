//  emulator.c

#include "emulator.h"
#include "debug.h"

uint8_t parity16(uint16_t x) {
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return (0 == (x & 0x1));
}

void set_zsp16(state_t *state, uint16_t arg) {
  state->flag.z = (arg == 0);
  state->flag.s = ((arg & 0x8000) == 0x8000);
  state->flag.p = parity16(arg);
}
void set_zsp8(state_t *state, uint8_t arg) {
  state->flag.z = (arg == 0);
  state->flag.s = ((arg & 0x80) == 0x80);
  state->flag.p = parity16(arg);
}

// ADD word, word
void opcode_add16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 + *arg2;
  *arg1 = (uint16_t)res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xffff);
}
void opcode_add8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 + *arg2;
  *arg1 = (uint8_t)res;
  set_zsp8(state, *arg1);
  state->flag.c = (res > 0xff);
}

// SUB word, word
void opcode_sub16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 - *arg2;
  *arg1 = (uint16_t)res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xffff);
}
void opcode_sub8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 - *arg2;
  *arg1 = (uint8_t)res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xff);
}

void opcode_mov16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 = *arg2;
}
void opcode_mov8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 = *arg2;
}

void opcode_xor16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 ^= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void opcode_xor8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 ^= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void opcode_or16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 |= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void opcode_or8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 |= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void opcode_and16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 &= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void opcode_and8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 &= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void opcode_xchg16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint16_t tmp = *arg1;
  *arg1 = *arg2;
  *arg2 = tmp;
}
void opcode_xchg8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint8_t tmp = *arg1;
  *arg1 = *arg2;
  *arg2 = tmp;
}

void opcode_sbb16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint16_t res = *arg1 - *arg2 - state->flag.c;
  *arg1 = res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xffff);
}
void opcode_sbb8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint8_t res = *arg1 - *arg2 - state->flag.c;
  *arg1 = res;
  set_zsp8(state, *arg1);
  state->flag.c = (res > 0xff);

}

void opcode_cmp16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 - *arg2;
  state->flag.z = (res == 0);
  state->flag.s = ((res & 0x8000) == 0x8000);
  state->flag.p = parity16(res);
  state->flag.c = (res > 0xffff);
}
void opcode_cmp8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 - *arg2;
  state->flag.z = (res == 0);
  state->flag.s = ((res & 0x8000) == 0x8000);
  state->flag.p = parity16(res);
  state->flag.c = (res > 0xffff);
}

void opcode_adc16(state_t * state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 + *arg2 + state->flag.c;
  *arg1 = (uint16_t)res;
  state->flag.z = (*arg1 == 0);
  state->flag.s = ((*arg1 & 0x8000) == 0x8000);
  state->flag.p = parity16(*arg1);
  state->flag.c = (res > 0xffff);
}

uint8_t *get_reg8(state_t *state, uint8_t reg_offset) {
  uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh};
  return reg_table8[reg_offset];
}

uint16_t *get_reg16(state_t *state, uint8_t reg_offset) {
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  return reg_table16[reg_offset];
}

// opcode2(state, &opcode_mov)
void opcode2v(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
  uint8_t *code = &state->memory[state->ip];
  uint8_t direction = (code[0] & 0b10) >> 1;
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg_offset = (code[1] & 0b00111000) >> 3;
  uint8_t rm_offset = code[1] & 0b00000111;

  uint16_t rm_table1[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    (code[3] << 8) | code[2], // Direct address
    state->bx };
  uint16_t rm_table2[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    state->bp, // BP
    state->bx //BX
  };

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  
  uint16_t *rm_ptr;
  switch(mod) {
  case 0:
    rm_ptr = (uint16_t *)&state->memory[ rm_table1[rm_offset] ];
    if (rm_offset == 6)
      state->ip += 2;
    break;
  case 1:
  case 2:
    rm_ptr = (uint16_t *)&state->memory[ rm_table2[rm_offset] + ((code[3] << 8) | code[2]) ];
    state->ip += 2;
    break;
  case 3:
    rm_ptr = reg_table16[rm_offset];
    break;

  default:
    rm_ptr = NULL;
    unimplemented_instruction(state);
    break;
  }

  uint16_t *reg_ptr = reg_table16[reg_offset];
  if(0 == direction) {
    func(state, rm_ptr, reg_ptr);  
  } else {
    func(state, reg_ptr, rm_ptr);
  }

  state->ip += 1;
}


// opcode2(state, &opcode_mov)
void opcode2b(state_t *state, void (*func)(state_t *, uint8_t *, uint8_t *) ) {
  uint8_t *code = &state->memory[state->ip];
  uint8_t direction = (code[0] & 0b10) >> 1;
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg_offset = (code[1] & 0b00111000) >> 3;
  uint8_t rm_offset = code[1] & 0b00000111;

  uint16_t rm_table1[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    (code[3] << 8) | code[2], // Direct address
    state->bx };
  uint16_t rm_table2[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    state->bp, // BP
    state->bx //BX
  };

  uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh};
  
  uint8_t *rm_ptr;
  switch(mod) {
  case 0:
    rm_ptr = &state->memory[ rm_table1[rm_offset] ];
    if (rm_offset == 6)
      state->ip += 2;
    break;
  case 1:
  case 2:
    rm_ptr = &state->memory[ rm_table2[rm_offset] + ((code[3] << 8) | code[2]) ];
    state->ip += 2;
    break;
  case 3:
    rm_ptr = reg_table8[rm_offset];
    break;

  default:
    rm_ptr = NULL;
    unimplemented_instruction(state);
    break;
  }

  uint8_t *reg_ptr = reg_table8[reg_offset];
  if(0 == direction) {
    func(state, rm_ptr, reg_ptr);  
  } else {
    func(state, reg_ptr, rm_ptr);
  }

  state->ip += 1;
}




void opcode2_ev_iv(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
  uint16_t *ev = NULL;
  uint16_t iv = 0;
  
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  // uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  // printf("!!mod:%d reg:%d rm:%d\n", mod, reg, rm);
      
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

  switch(mod) {
  case 0: // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)(&state->memory[(opcode[3] << 8) | opcode[2]]);
      iv = (opcode[5] << 8) | opcode[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
    break;
  case 1: // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
    break;
  case 2: // 10 Use R/M Table 2 with 16-bit displacement
    // OP rm_table2[rm] reg_w1[reg]
    unimplemented_instruction(state);
    break;
  case 3: // 11 Two register instruction; use REG table   
    {
      ev = reg_table16[rm];
      iv = (opcode[3] << 8) | opcode[2];
    }
    break;

  default:
    unimplemented_instruction(state);
    break;
  }
  func(state, ev, &iv);
  // TODO: af, cf, of, pf, sf, zf

  state->ip += 3;
}

void opcode2_grp1_ev_ib(state_t *state) {
  uint16_t *ev;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  uint16_t ib = (uint16_t)(int8_t)opcode[2];

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      ib = (uint16_t)(int8_t)opcode[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    ev = reg_table16[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: opcode_add16(state, ev, &ib); break;
  case 1: unimplemented_instruction(state); break;
  case 2: opcode_adc16(state, ev, &ib); break;
  case 3: opcode_sbb16(state, ev, &ib); break;
  case 4: opcode_and16(state, ev, &ib); break;
  case 5: opcode_sub16(state, ev, &ib); break;
  case 6: unimplemented_instruction(state); break;
  case 7: opcode_cmp16(state, ev, &ib); break;

  default:
    break;
  }
  state->ip += 2;
}

void opcode2_grp1_ev_iv(state_t *state) {
  uint16_t *ev;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  uint16_t iv = (opcode[3] << 8) | opcode[2];

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      iv = (opcode[5] << 8) | opcode[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    ev = reg_table16[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: *ev += iv; break;
  case 1: unimplemented_instruction(state); break;
  case 2: unimplemented_instruction(state); break;
  case 3: unimplemented_instruction(state); break;
  case 4: unimplemented_instruction(state); break;
  case 5: unimplemented_instruction(state); break;
  case 6: unimplemented_instruction(state); break;
  case 7: // TODO: set flags
    state->flag.z = (0 == (*ev - iv));
    break;

  default:
    break;
  }
  state->ip += 3;
}

void opcode2_grp1_eb_ib(state_t *state) {
  uint8_t *eb;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  uint8_t ib = opcode[2];

  uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh};

  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      eb = (uint8_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      ib = opcode[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    eb = reg_table8[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: *eb += ib; break;
  case 1: unimplemented_instruction(state); break;
  case 2: unimplemented_instruction(state); break;
  case 3: unimplemented_instruction(state); break;
  case 4: unimplemented_instruction(state); break;
  case 5: unimplemented_instruction(state); break;
  case 6: unimplemented_instruction(state); break;
  case 7: state->flag.z = (0 == (*eb - ib)); break;

  default:
    break;
  }
  state->ip += 2;
}

void opcode2_mov_eb_ib(state_t *state) { // MOV Eb Ib
  uint8_t *opcode = &state->memory[state->ip];

  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t rm = opcode[1] & 0b00000111;

  uint8_t *eb;
  uint8_t ib = opcode[2];
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      eb = (uint8_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      ib = opcode[4];

      *eb += ib; // actual move
      
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    unimplemented_instruction(state); // second arg is Ib
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  state->ip += 2;
}
void opcode2_mov_eb_gb(state_t *state) { // MOV Eb Ib
  uint8_t *opcode = &state->memory[state->ip];

  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t rm = opcode[1] & 0b00000111;
  // uint8_t reg = (opcode[1] & 0b00111000) >> 3;

  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      unimplemented_instruction(state);
      state->ip += 2;
    } else {
      if (rm == 1) {
        state->memory[state->bx + state->di] = state->al;
      } else {
        unimplemented_instruction(state);
      }
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    unimplemented_instruction(state); // second arg is Ib
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  state->ip += 1;
}

void opcode2_mov_sw_ew(state_t *state) { // MOV Sw Ew
  uint8_t *opcode = &state->memory[state->ip];

  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  uint16_t *sreg[] = {&state->es, &state->cs, &state->ss, &state->ds};
  uint16_t *sw = sreg[reg];
  
  // uint16_t ew;
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      //sw = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      //state->ip += 2;
      unimplemented_instruction(state);
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    uint16_t *ew = reg_table16[rm];
    *sw = *ew;
    // unimplemented_instruction(state);
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  state->ip += 1;
}



void grp5_inc(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_dec(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_call(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_call_mp(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_jmp(state_t *state, uint16_t *arg) { state->ip = *arg; }
void grp5_jmp_mp(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_push(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }


int emulate_op(state_t *state) {
  unsigned char *opcode = &state->memory[state->ip];

  //print_state(state);
  disassemble_opcode_8086(state, state->ip);

  switch(*opcode) {

  case 0x00:
  case 0x02: opcode2b(state, &opcode_add8); break; // ADD Gb, Ev

  case 0x01: // ADD Ev, Gv
  case 0x03: opcode2v(state, &opcode_add16); break; // ADD Gv, Ev
    
  case 0x04: state->al = opcode[1]; state->ip += 1; break; // ADD AL, Ib
  case 0x05: // ADD AX, Iv
    {
      state->ax += ((opcode[2] << 8) | opcode[1]); // FIXME: use opcode_add16 instead
      state->ip += 2;
    }
    break;

  case 0x08:
  case 0x0a:
    opcode2b(state, &opcode_or8);
    break;

  case 0x09:
  case 0x0b:
    opcode2v(state, &opcode_or16);
    break;

  case 0x18:
  case 0x1a:
    opcode2b(state, &opcode_sbb8);
    break;

  case 0x19:
  case 0x1b:
    opcode2v(state, &opcode_sbb16);
    break;

  case 0x20:
  case 0x22:
    opcode2b(state, &opcode_and8);
    break;
    
  case 0x28:
  case 0x2a:
    opcode2b(state, &opcode_sub8);
    break;
  case 0x29:
  case 0x2b:
    opcode2v(state, &opcode_sub16);
    break;
    
  case 0x30: // XOR Ev Gv
  case 0x32:
    opcode2b(state, &opcode_xor8);
    break;

  case 0x31: // XOR Ev Gv
  case 0x33:
    opcode2v(state, &opcode_xor16);
    break;

  case 0x38:
  case 0x3a: opcode2b(state, &opcode_cmp8); break;
  case 0x39:
  case 0x3b: opcode2v(state, &opcode_cmp16); break;

  case 0x3c: // CMP AL, imm8
    opcode_cmp8(state, &state->al, &opcode[1]);
    state->ip += 1;
    break;
    
  case 0x40: state->ax++; set_zsp16(state, state->ax); break; //INC AX
  case 0x41: state->cx++; set_zsp16(state, state->cx); break;
  case 0x42: state->dx++; set_zsp16(state, state->dx); break;
  case 0x43: state->bx++; set_zsp16(state, state->bx); break;
  case 0x44: state->sp++; set_zsp16(state, state->sp); break;
  case 0x45: state->bp++; set_zsp16(state, state->bp); break;
  case 0x46: state->si++; set_zsp16(state, state->si); break;
  case 0x47: state->di++; set_zsp16(state, state->di); break;
    
  case 0x48: state->ax--; set_zsp16(state, state->ax); break; // DEC AX
  case 0x49: state->cx--; set_zsp16(state, state->cx); break;
  case 0x4a: state->dx--; set_zsp16(state, state->dx); break;
  case 0x4b: state->bx--; set_zsp16(state, state->bx); break;
  case 0x4c: state->sp--; set_zsp16(state, state->sp); break;
  case 0x4d: state->bp--; set_zsp16(state, state->bp); break;
  case 0x4e: state->si--; set_zsp16(state, state->si); break;
  case 0x4f: state->di--; set_zsp16(state, state->di); break;

    
  case 0x50: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->ax; break; // PUSH AX
  case 0x51: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->cx; break; // PUSH CX
  case 0x52: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->dx; break; // PUSH DX
  case 0x53: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->bx; break; // PUSH BX
  case 0x54: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->sp; break; // PUSH SP
  case 0x55: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->bp; break; // PUSH BP
  case 0x56: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->si; break; // PUSH SI
  case 0x57: state->sp -= 2; *((uint16_t *)(&state->memory[state->sp])) = state->di; break; // PUSH DI
    
  case 0x58: state->ax = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP AX
  case 0x59: state->cx = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP CX
  case 0x5a: state->dx = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP DX
  case 0x5b: state->bx = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP BX
  case 0x5c: state->sp = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP SP
  case 0x5d: state->bp = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP BP
  case 0x5e: state->si = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP SI
  case 0x5f: state->di = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POP DI

  case 0x72: if(state->flag.c == 1) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JB Jb
    
  case 0x74: if(state->flag.z == 1) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JZ Jb
  case 0x75: if(state->flag.z == 0) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JNZ Jb
  case 0x76: if(state->flag.c == 1 || state->flag.z == 1) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JBE Jb
  case 0x77: if(state->flag.c == 0 || state->flag.z == 0) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JA Jb
  case 0x79: if(state->flag.s == 0) { state->ip += (int8_t)opcode[1]; }; state->ip += 1; break; // JNO Jb
    
  case 0x80: opcode2_grp1_eb_ib(state); break; // GRP1 Eb, Ib
  case 0x81: opcode2_grp1_ev_iv(state); break; // GRP1 Ev, Iv
  case 0x83: opcode2_grp1_ev_ib(state); break; // GRP1 Ev, Ib

  case 0x86: opcode2b(state, &opcode_xchg8); break;
  case 0x87: opcode2v(state, &opcode_xchg16); break;
  case 0x88: // MOV Eb Gb
  case 0x8a: opcode2b(state, &opcode_mov8); break;
  case 0x89: // MOV Ev, Gv
  case 0x8b: opcode2v(state, &opcode_mov16); break; // MOV Gv, Ev
    
  case 0x8e: opcode2_mov_sw_ew(state); break; // MOV Sw, Ev

  case 0x90: break; // NOP

  /* case 0x92: // XCHG DX, AX */
  /*   { */
  /*     uint16_t tmp = state->dx; */
  /*     state->dx = state->ax; */
  /*     state->ax = tmp; */
  /*   } */
  /*   break; */
    
  case 0x9c: state->sp = state->sp - 2; *((uint16_t *)(&state->memory[state->sp])) = state->flags; break; // PUSHF
  case 0x9d: state->flags = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POPF

  case 0xa1: // MOV AX, [addr16]
    {
      uint16_t addr16 = (opcode[2] << 8) | opcode[1];
      state->ax = *((uint16_t *)(&state->memory[addr16]));
      state->ip += 2;
    }
    break;
  case 0xa3: // MOV [addr16], AX
    {
      uint16_t addr16 = (opcode[2] << 8) | opcode[1];
      *((uint16_t *)(&state->memory[addr16])) = state->ax;
      state->ip += 2;
    }
    break;

  case 0xb0: state->al = opcode[1]; state->ip += 1; break; // MOV AL, Ib
  case 0xb1: state->cl = opcode[1]; state->ip += 1; break; // MOV CL, Ib
  case 0xb2: state->dl = opcode[1]; state->ip += 1; break; // MOV DL, Ib
  case 0xb3: state->bl = opcode[1]; state->ip += 1; break; // MOV BL, Ib
  case 0xb4: state->ah = opcode[1]; state->ip += 1; break; // MOV AH, Ib
  case 0xb5: state->ch = opcode[1]; state->ip += 1; break; // MOV CH, Ib
  case 0xb6: state->dh = opcode[1]; state->ip += 1; break; // MOV DH, Ib
  case 0xb7: state->bh = opcode[1]; state->ip += 1; break; // MOV BH, Ib

  case 0xb8: state->ax = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV AX, Iv
  case 0xb9: state->cx = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV CX, Iv
  case 0xba: state->dx = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV DX, Iv
  case 0xbb: state->bx = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV BX, Iv
  case 0xbc: state->sp = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV SP, Iv
  case 0xbd: state->bp = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV BP, Iv
  case 0xbe: state->si = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV SI, Iv
  case 0xbf: state->di = (opcode[2] << 8) | opcode[1]; state->ip += 2; break; // MOV DI, Iv

  case 0xc3: // RET
    {
      uint16_t addr = *(uint16_t *)(&state->memory[state->sp]);
      state->sp += 2;
      state->ip = addr - 1;
    }
    // unimplemented_instruction(state);
    break;
    
  case 0xc6: opcode2_mov_eb_ib(state); break; // MOV Eb, Ib
  case 0xc7: opcode2_ev_iv(state, &opcode_mov16); break; // MOV Ev, Iv

  case 0xe8: // CALL Jb
    state->sp = state->sp - 2;
    *(uint16_t *)(&state->memory[state->sp]) = state->ip + 3;
    state->ip = state->ip + (int16_t)((opcode[2] << 8) | opcode[1]) + 2;
    break;
    
  case 0xeb: state->ip += (1 + (int8_t)opcode[1]); break; // JMP Jb - jump relative
    
  case 0xf4: print_state(state); /* print_memory(state); */ exit(EXIT_SUCCESS); break; // HLT
    
  case 0xf5: state->flag.c = !state->flag.c; break; // CMC
    
  case 0xf8: state->flag.c = 0; break; // CLC
  case 0xf9: state->flag.c = 1; break; // STC
  case 0xfa: state->flag.i = 0; break; // CLI
  case 0xfb: state->flag.i = 1; break; // STI
  case 0xfc: state->flag.d = 0; break; // CLD
  case 0xfd: state->flag.d = 1; break; // STD

  case 0xfe: // GRP4 Eb
    {
      uint8_t mod = (opcode[1] & 0b11000000) >> 6;
      uint8_t reg = (opcode[1] & 0b00111000) >> 3;
      uint8_t rm = opcode[1] & 0b00000111;

      // uint16_t *ev;
      uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh};

      if(0 == mod) {
        if (6 == rm) {} else {};
        unimplemented_instruction(state);
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        uint8_t *eb = reg_table8[rm];
        switch(reg) {
        case 0: // INC
          *eb += 1;
          set_zsp8(state, *eb);
          break;
        case 1: // DEC
          *eb -= 1;
          set_zsp8(state, *eb);
          break;

        default:
          unimplemented_instruction(state);
          break;
        }
      } else {
        // should never be reached
      }

      state->ip += 1;
    }
    break;

  case 0xff: // GRP5 Ev
    {
      uint8_t mod = (opcode[1] & 0b11000000) >> 6;
      uint8_t reg = (opcode[1] & 0b00111000) >> 3;
      uint8_t rm = opcode[1] & 0b00000111;

      // uint16_t *ev;
      void (*grp5[])(state_t*, uint16_t *) = {&grp5_inc, &grp5_dec, &grp5_call, &grp5_call_mp, &grp5_jmp, &grp5_jmp_mp, &grp5_push};
      uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

      if(0 == mod) {
        if (6 == rm) {
        } else {
        }
        unimplemented_instruction(state);
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        (*grp5[reg])(state, reg_table16[rm]);
        unimplemented_instruction(state);
      } else {
        // should never be reached
      }
      state->ip += 1;
      //unimplemented_instruction(state);
    }
    break;
    
  default:
    unimplemented_instruction(state);
    exit(EXIT_FAILURE);
    break;
  }
  state->ip += 1;
  return 0;
}
