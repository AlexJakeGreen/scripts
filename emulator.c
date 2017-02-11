//  emulator.c

#include "emulator.h"
#include "debug.h"

// ADD word, word
void opcode_add16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *(uint16_t *)arg1 += *arg2;
}

void opcode_mov16(state_t *state, uint16_t *arg1, uint16_t *arg2) {
  *(uint16_t *)arg1 = *arg2;
}


void opcode2_ev_gv(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
  uint16_t *ev, *gv;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  // printf("!!mod:%d reg:%d rm:%d\n", mod, reg, rm);
      
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

  switch(mod) {
  case 0: // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      gv = reg_table16[reg];
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
      gv = reg_table16[reg];
    }
    func(state, ev, gv);
    // TODO: af, cf, of, pf, sf, zf
    break;
        
  default:
    unimplemented_instruction(state);
    break;
  }
  state->ip += 1;
}

void opcode2_gv_ev(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *)) {
  uint16_t *ev, *gv;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  // printf("!!mod:%d reg:%d rm:%d\n", mod, reg, rm);
      
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

  switch(mod) {
  case 0: // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      gv = reg_table16[reg];
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
      gv = reg_table16[reg];
    }
    func(state, gv, ev);
    // TODO: af, cf, of, pf, sf, zf
    break;
        
  default:
    unimplemented_instruction(state);
    break;
  }
  state->ip += 1;
}

void opcode2_ev_iv(state_t *state, void (*func)(state_t *, uint16_t *, uint16_t *) ) {
  uint16_t *ev, *gv;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  // uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  // printf("!!mod:%d reg:%d rm:%d\n", mod, reg, rm);
      
  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};

  switch(mod) {
  case 0: // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
      gv = (uint16_t *)&state->memory[(opcode[5] << 8) | opcode[4]];
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
      gv = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
    }
    func(state, ev, gv);
    // TODO: af, cf, of, pf, sf, zf
    break;

  default:
    unimplemented_instruction(state);
    break;
  }
  state->ip += 3;
}

void opcode2_grp1_ev_ib(state_t *state) {
  uint16_t *ev;
  uint8_t *opcode = &state->memory[state->ip];
  
  uint8_t mod = (opcode[1] & 0b11000000) >> 6;
  uint8_t reg = (opcode[1] & 0b00111000) >> 3;
  uint8_t rm = opcode[1] & 0b00000111;

  uint8_t ib = opcode[2];

  uint16_t *reg_table16[] = {&state->ax, &state->cx, &state->dx, &state->bx, &state->sp, &state->bp, &state->si, &state->di};
  
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      ev = (uint16_t *)&state->memory[(opcode[3] << 8) | opcode[2]];
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
    ev = reg_table16[rm];
  } else { // should never be reached
    unimplemented_instruction(state);
  }

  switch(reg) { // GRP1: add or adc sbb and sub xor cmp
  case 0: *ev += ib; break;
  case 1: unimplemented_instruction(state); break;
  case 2: unimplemented_instruction(state); break;
  case 3: unimplemented_instruction(state); break;
  case 4: unimplemented_instruction(state); break;
  case 5: unimplemented_instruction(state); break;
  case 6: unimplemented_instruction(state); break;
  case 7: unimplemented_instruction(state); break;

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
  case 7: unimplemented_instruction(state); break;

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
  case 7: unimplemented_instruction(state); break;

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


int emulate_op(state_t *state) {
  unsigned char *opcode = &state->memory[state->ip];

  disassemble_opcode_8086(state, state->ip);

  switch(*opcode) {

  case 0x01: opcode2_ev_gv(state, &opcode_add16); break; // ADD Ev, Gv
  case 0x03: opcode2_gv_ev(state, &opcode_add16); break; // ADD Gv, Ev
  case 0x04: state->al = opcode[1]; state->ip += 1; break; // ADD AL, Ib
  case 0x05: // ADD AX, Iv
    {
      state->ax += ((opcode[2] << 8) | opcode[1]); // FIXME: use opcode_add16 instead
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

  case 0x80: opcode2_grp1_eb_ib(state); break; // GRP1 Eb, Ib
  case 0x81: opcode2_grp1_ev_iv(state); break; // GRP1 Ev, Iv
  case 0x83: opcode2_grp1_ev_ib(state); break; // GRP1 Ev, Ib
    
  case 0x89: opcode2_ev_gv(state, &opcode_mov16); break; // MOV Ev, Gv

  case 0x9c: state->sp = state->sp - 2; *((uint16_t *)(&state->memory[state->sp])) = 0xffff; break; // PUSHF
    
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

  case 0xc6: opcode2_mov_eb_ib(state); break; // MOV Eb, Ib
  case 0xc7: opcode2_ev_iv(state, &opcode_mov16); break; // MOV Ev, Iv

  default:
    unimplemented_instruction(state);
    exit(EXIT_FAILURE);
    break;
  }
  state->ip += 1;
  return 0;
}
