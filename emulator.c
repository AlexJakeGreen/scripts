//  emulator.c

#include "emulator.h"
#include "debug.h"

#define READ16(pos) *(uint16_t *)(code + pos)

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
void code_add16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 + *arg2;
  *arg1 = (uint16_t)res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xffff);
}
void code_add8(state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 + *arg2;
  *arg1 = (uint8_t)res;
  set_zsp8(state, *arg1);
  state->flag.c = (res > 0xff);
}

void code_inc16(state_t *state, uint16_t *arg) {
  uint32_t res = *arg + 1;
  *arg = (uint16_t)res;
  set_zsp16(state, *arg);
  state->flag.c = (res > 0xffff);  
}
void code_dec16(state_t *state, uint16_t *arg) {
  uint32_t res = *arg - 1;
  *arg = (uint16_t)res;
  set_zsp16(state, *arg);
  state->flag.c = (res > 0xffff);  
}

void code_push(state_t *state, uint16_t *arg) {
  state->sp -= 2;
  *((uint16_t *)(&state->memory[state->sp])) = *arg;
}

void code_pop(state_t *state, uint16_t *arg) {
  *arg = *((uint16_t *)(&state->memory[state->sp]));
  state->sp += 2; 
}

// SUB word, word
void code_sub16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 - *arg2;
  state->flag.c = (*arg1 < *arg2);
  *arg1 = (uint16_t)res;
  set_zsp16(state, *arg1);
}
void code_sub8(state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 - *arg2;
  state->flag.c = (*arg1 < *arg2);
  *arg1 = (uint8_t)res;
  set_zsp16(state, *arg1);
}

void code_mov16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 = *arg2;
}
void code_mov8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 = *arg2;
}

void code_xor16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 ^= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void code_xor8(state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 ^= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void code_or16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 |= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void code_or8(state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 |= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void code_and16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *arg1 &= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp16(state, *arg1);
}
void code_and8(state_t *state, uint8_t *arg1, uint8_t *arg2) {
  *arg1 &= *arg2;
  state->flag.o = 0;
  state->flag.c = 0;
  set_zsp8(state, *arg1);
}

void code_xchg16(__attribute__((__unused__)) state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint16_t tmp = *arg1;
  *arg1 = *arg2;
  *arg2 = tmp;
}
void code_xchg8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint8_t tmp = *arg1;
  *arg1 = *arg2;
  *arg2 = tmp;
}

void code_sbb16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint16_t res = *arg1 - *arg2 - state->flag.c;
  *arg1 = res;
  set_zsp16(state, *arg1);
  state->flag.c = (res > 0xffff);
}
void code_sbb8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint8_t res = *arg1 - *arg2 - state->flag.c;
  *arg1 = res;
  set_zsp8(state, *arg1);
  state->flag.c = (res > 0xff);

}

void code_cmp16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 - *arg2;
  state->flag.z = (res == 0);
  state->flag.s = ((res & 0x8000) == 0x8000);
  state->flag.p = parity16(res);
  state->flag.c = (res > 0xffff);
}
void code_cmp8(__attribute__((__unused__)) state_t *state, uint8_t *arg1, uint8_t *arg2) {
  uint16_t res = *arg1 - *arg2;
  state->flag.z = (res == 0);
  state->flag.s = ((res & 0x8000) == 0x8000);
  state->flag.p = parity16(res);
  state->flag.c = (res > 0xffff);
}

void code_adc16(state_t * state, uint16_t *arg1, uint16_t *arg2) {
  uint32_t res = *arg1 + *arg2 + state->flag.c;
  *arg1 = (uint16_t)res;
  state->flag.z = (*arg1 == 0);
  state->flag.s = ((*arg1 & 0x8000) == 0x8000);
  state->flag.p = parity16(*arg1);
  state->flag.c = (res > 0xffff);
}

/* uint8_t *get_reg8(state_t *state, uint8_t reg_offset) { */
/*   uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh}; */
/*   return reg_table8[reg_offset]; */
/* } */

/* uint16_t *get_reg16(state_t *state, uint8_t reg_offset) { */
/*   return reg_table16[reg_offset]; */
/* } */

// code2(state, &code_mov)
void code2v(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
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
    READ16(2), // Direct address
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
    rm_ptr = (uint16_t *)&state->memory[ rm_table2[rm_offset] + (READ16(2)) ];
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

  state->ip++;
}


// code2(state, &code_mov)
void code2b(state_t *state, void (*func)(state_t *, uint8_t *, uint8_t *) ) {
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
    READ16(2), // Direct address
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
    rm_ptr = &state->memory[ rm_table2[rm_offset] + (READ16(2)) ];
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

  state->ip++;
}




void code2_ev_iv(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
  uint16_t *ev = NULL;
  uint16_t iv = 0;
  
  uint8_t *code = &state->memory[state->ip];
  
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  // uint8_t reg = (code[1] & 0b00111000) >> 3;
  uint8_t rm = code[1] & 0b00000111;

  // printf("!!mod:%d reg:%d rm:%d\n", mod, reg, rm);
      
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

  switch(mod) {
  case 0: // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)(&state->memory[READ16(2)]);
      iv = READ16(4);
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
      iv = READ16(2);
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

void code2_grp1_ev_ib(state_t *state) {
  uint16_t *ev;
  uint8_t *code = &state->memory[state->ip];
  
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg = (code[1] & 0b00111000) >> 3;
  uint8_t rm = code[1] & 0b00000111;

  uint16_t ib = (uint16_t)(int8_t)code[2];

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  /*
  uint16_t rm_table1[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    READ16(2), // Direct address
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
  */
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[READ16(2)];
      ib = (uint16_t)(int8_t)code[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    // eb = &state->memory[ rm_table1[rm] + (READ16(2))]; // FIXME
    unimplemented_instruction(state);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    ev = reg_table16[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: code_add16(state, ev, &ib); break;
  case 1: code_or16(state, ev, &ib); break;
  case 2: code_adc16(state, ev, &ib); break;
  case 3: code_sbb16(state, ev, &ib); break;
  case 4: code_and16(state, ev, &ib); break;
  case 5: code_sub16(state, ev, &ib); break;
  case 6: printf("case5\n");unimplemented_instruction(state); break;
  case 7: code_cmp16(state, ev, &ib); break;

  default:
    break;
  }
  state->ip += 2;
}

void code2_grp1_ev_iv(state_t *state) {
  uint16_t *ev;
  uint8_t *code = &state->memory[state->ip];
  
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg = (code[1] & 0b00111000) >> 3;
  uint8_t rm = code[1] & 0b00000111;

  uint16_t iv = READ16(2);

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[READ16(2)];
      iv = READ16(4);
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

void code2_grp1_eb_ib(state_t *state) {
  uint8_t *eb;
  uint8_t *code = &state->memory[state->ip];
  
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg = (code[1] & 0b00111000) >> 3;
  uint8_t rm = code[1] & 0b00000111;

  uint8_t ib = code[2];

  uint8_t *reg_table8[] = {&state->al, &state->cl, &state->dl, &state->bl, &state->ah, &state->ch, &state->dh, &state->bh};
  /*  uint16_t rm_table1[8] = {
    state->bx + state->si, // BX+SI
    state->bx + state->di, // BX+DI
    state->bp + state->si, // BP+SI
    state->bp + state->di, // BP+DI
    state->si, // SI
    state->di, // DI
    READ16(2), // Direct address
    state->bx }; */
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

  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      eb = (uint8_t *)&state->memory[READ16(2)];
      ib = code[4];
      state->ip += 2;
    } else {
      unimplemented_instruction(state);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    unimplemented_instruction(state);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    uint16_t addr = rm_table2[rm] + (READ16(2)); // signed?
    eb = &state->memory[addr];
    ib = code[4];
    state->ip += 2;
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table
    eb = reg_table8[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: *eb += ib; break;
  case 1: code_or8(state, eb, &ib); break;
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

void code2_mov_eb_ib(state_t *state) { // MOV Eb Ib
  uint8_t *code = &state->memory[state->ip];

  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t rm = code[1] & 0b00000111;

  uint8_t *eb;
  uint8_t ib = code[2];
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      eb = (uint8_t *)&state->memory[READ16(2)];
      ib = code[4];

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

void code2_mov_sw_ew(state_t *state) { // MOV Sw Ew
  uint8_t *code = &state->memory[state->ip];

  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t reg = (code[1] & 0b00111000) >> 3;
  uint8_t rm = code[1] & 0b00000111;

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  uint16_t *sreg[] = {&state->es, &state->cs, &state->ss, &state->ds};
  uint16_t *sw = sreg[reg];
  
  // uint16_t ew;
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      //sw = (uint16_t *)&state->memory[READ16(2)];
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

  state->ip++;
}

void grp5_inc(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_dec(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_call(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_call_mp(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_jmp(state_t *state, uint16_t *arg) { state->ip = *arg; }
void grp5_jmp_mp(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }
void grp5_push(state_t *state, uint16_t *arg) { unimplemented_instruction(state); }

int emulate_op(state_t *state) {
  unsigned char *code = &state->memory[state->ip];

  //print_state(state);
  disassemble_opcode_8086(state, state->ip);

  switch(*code) {

  case 0x00:
  case 0x02: code2b(state, &code_add8); break;
  case 0x01:
  case 0x03: code2v(state, &code_add16); break;
  case 0x04: code_add8(state, &state->al, &code[1]); state->ip++; break;
  case 0x05:
    state->ax += READ16(1); // FIXME: use code_add16 instead
    state->ip += 2;
    break;

  case 0x08:
  case 0x0a: code2b(state, &code_or8); break;
  case 0x09:
  case 0x0b: code2v(state, &code_or16); break;
  case 0x18:
  case 0x1a: code2b(state, &code_sbb8); break;
  case 0x19:
  case 0x1b: code2v(state, &code_sbb16); break;
  case 0x20:
  case 0x22: code2b(state, &code_and8); break;    
  case 0x28:
  case 0x2a: code2b(state, &code_sub8); break;
  case 0x29:
  case 0x2b: code2v(state, &code_sub16); break;
    
  case 0x30: // XOR Ev Gv
  case 0x32:
    code2b(state, &code_xor8);
    break;

  case 0x31: // XOR Ev Gv
  case 0x33:
    code2v(state, &code_xor16);
    break;

  case 0x38:
  case 0x3a: code2b(state, &code_cmp8); break;
  case 0x39:
  case 0x3b: code2v(state, &code_cmp16); break;

  case 0x3c: // CMP AL, imm8
    code_cmp8(state, &state->al, &code[1]);
    state->ip++;
    break;

  case 0x40 ... 0x47: code_inc16(state, &state->ax + (code[0] & 0b111)); break;
  case 0x48 ... 0x4f: code_dec16(state, &state->ax + (code[0] & 0b111)); break;
  case 0x50 ... 0x57: code_push(state, &state->ax + (code[0] & 0b111)); break;
  case 0x58 ... 0x5f: code_pop(state, &state->ax + (code[0] & 0b111)); break;

  case 0x72: if(state->flag.c == 1) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JB Jb
    
  case 0x74: if(state->flag.z == 1) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JZ Jb
  case 0x75: if(state->flag.z == 0) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JNZ Jb
  case 0x76: if(state->flag.c == 1 || state->flag.z == 1) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JBE Jb
  case 0x77: if(state->flag.c == 0 ) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JA Jb
  case 0x79: if(state->flag.s == 0) { state->ip += (int8_t)code[1]; }; state->ip++; break; // JNO Jb
    
  case 0x80: code2_grp1_eb_ib(state); break; // GRP1 Eb, Ib
  case 0x81: code2_grp1_ev_iv(state); break; // GRP1 Ev, Iv
  case 0x83: code2_grp1_ev_ib(state); break; // GRP1 Ev, Ib

  case 0x86: code2b(state, &code_xchg8); break;
  case 0x87: code2v(state, &code_xchg16); break;
  case 0x88: // MOV Eb Gb
  case 0x8a: code2b(state, &code_mov8); break;
  case 0x89: // MOV Ev, Gv
  case 0x8b: code2v(state, &code_mov16); break; // MOV Gv, Ev
    
  case 0x8e: code2_mov_sw_ew(state); break; // MOV Sw, Ev

  case 0x90: break; // NOP

  case 0x92: // XCHG DX, AX
    code_xchg16(state, &state->dx, &state->ax);
    break;
    
  case 0x9c: state->sp = state->sp - 2; *((uint16_t *)(&state->memory[state->sp])) = state->flags; break; // PUSHF
  case 0x9d: state->flags = *((uint16_t *)(&state->memory[state->sp])); state->sp += 2; break; // POPF

  case 0xa1: // MOV AX, [addr16]
    state->ax = *((uint16_t *)(&state->memory[READ16(1)]));
    state->ip += 2;
    break;
  case 0xa3: // MOV [addr16], AX
    *((uint16_t *)(&state->memory[READ16(1)])) = state->ax;
    state->ip += 2;
    break;

  case 0xb0: state->al = code[1]; state->ip++; break; // MOV AL, Ib
  case 0xb1: state->cl = code[1]; state->ip++; break; // MOV CL, Ib
  case 0xb2: state->dl = code[1]; state->ip++; break; // MOV DL, Ib
  case 0xb3: state->bl = code[1]; state->ip++; break; // MOV BL, Ib
  case 0xb4: state->ah = code[1]; state->ip++; break; // MOV AH, Ib
  case 0xb5: state->ch = code[1]; state->ip++; break; // MOV CH, Ib
  case 0xb6: state->dh = code[1]; state->ip++; break; // MOV DH, Ib
  case 0xb7: state->bh = code[1]; state->ip++; break; // MOV BH, Ib

  case 0xb8: state->ax = READ16(1); state->ip += 2; break; // MOV AX, Iv
  case 0xb9: state->cx = READ16(1); state->ip += 2; break; // MOV CX, Iv
  case 0xba: state->dx = READ16(1); state->ip += 2; break; // MOV DX, Iv
  case 0xbb: state->bx = READ16(1); state->ip += 2; break; // MOV BX, Iv
  case 0xbc: state->sp = READ16(1); state->ip += 2; break; // MOV SP, Iv
  case 0xbd: state->bp = READ16(1); state->ip += 2; break; // MOV BP, Iv
  case 0xbe: state->si = READ16(1); state->ip += 2; break; // MOV SI, Iv
  case 0xbf: state->di = READ16(1); state->ip += 2; break; // MOV DI, Iv

  case 0xc3: // RET
    {
      uint16_t addr = *(uint16_t *)(&state->memory[state->sp]);
      state->sp += 2;
      state->ip = addr - 1;
    }
    break;
    
  case 0xc6: code2_mov_eb_ib(state); break; // MOV Eb, Ib
  case 0xc7: code2_ev_iv(state, &code_mov16); break; // MOV Ev, Iv

  case 0xe8: // CALL Jb
    state->sp = state->sp - 2;
    *(uint16_t *)(&state->memory[state->sp]) = state->ip + 3;
    state->ip = state->ip + (int16_t)(READ16(1)) + 2;
    break;
    
  case 0xeb: state->ip += (1 + (int8_t)code[1]); break; // JMP Jb - jump relative
    
  case 0xf4: print_state(state); print_memory(state); exit(EXIT_SUCCESS); break; // HLT
    
  case 0xf5: state->flag.c = !state->flag.c; break; // CMC
    
  case 0xf8: state->flag.c = 0; break; // CLC
  case 0xf9: state->flag.c = 1; break; // STC
  case 0xfa: state->flag.i = 0; break; // CLI
  case 0xfb: state->flag.i = 1; break; // STI
  case 0xfc: state->flag.d = 0; break; // CLD
  case 0xfd: state->flag.d = 1; break; // STD

  case 0xfe: // GRP4 Eb
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;

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

      state->ip++;
    }
    break;

  case 0xff: // GRP5 Ev
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;

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
      state->ip++;
      //unimplemented_instruction(state);
    }
    break;
    
  default:
    unimplemented_instruction(state);
    exit(EXIT_FAILURE);
    break;
  }
  state->ip++;
  return 0;
}
