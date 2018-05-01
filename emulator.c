#include <stdio.h>
#include "emulator.h"
#include "disassembler.h"
#include "cpm.h"

// Check bit parity for 8bit int using XOR
int parity8(uint8_t x) {
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return (0 == (x & 0x1));
}

// Check bit parity for 16bit int using XOR
int parity16(uint16_t x) {
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return (0 == (x & 0x1));
}

void print_stack(state_t *state) {
    /* printf("Stack: "); */
    /* printf("f6:%02x %02x  ", state->memory[0xfff6], state->memory[0xfff7]); */
    /* printf("f8:%02x %02x  ", state->memory[0xfff8], state->memory[0xfff9]); */
    /* printf("fa:%02x %02x  ", state->memory[0xfffa], state->memory[0xfffb]); */
    /* printf("fc:%02x %02x  ", state->memory[0xfffc], state->memory[0xfffd]); */
    /* printf("fe:%02x %02x", state->memory[0xfffe], state->memory[0xffff]); */
    /* printf("\n"); */
}

void print_state(state_t *state) {
    printf("\t==PC:%04x SP:%04x A:%02x F:%02x B:%02x C:%02x D:%02x E:%02x H:%02x L:%02x IX: %04x\n",
           state->r_pc, state->r_sp, state->r_a, state->r_f, state->r_b, state->r_c,
           state->r_d, state->r_e, state->r_h, state->r_l, state->r_ix);
    // printf("\t==PCH:%02x PCL:%02x\n", state->r_pch, state->r_pcl);
    printf("\t == C: %02x\n", state->flags.c);
    /* printf("\t==Z:%01x S:%01x P:%01x Cy:%01x Ac:%01x\n", */
    /*        state->cc.z, state->cc.s, state->cc.p, state->cc.cy, state->cc.ac); */
    print_stack(state);
}

void add8(state_t *state, uint8_t data) {
            if (((state->r_a ^ data) & 0x80) == 0 
                && ((1 ^ (state->r_a + data)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0;
            if ((state->r_a + data) > 0xffff)
                state->flags.c = 1;
            else
                state->flags.c = 0;
            state->flags.h = (((state->r_a & 0xf) + (data & 0xf)) & 0x10) > 1;
            state->r_a += data;
            state->flags.n = 0;
            state->flags.z = (state->r_a == 0);
            state->flags.s = ((state->r_a & 0x80) == 0x80);
            state->r_pc += 1;
}

void add16(state_t *state, uint16_t *lft, uint16_t *rgt) {
            uint32_t res = *lft + *rgt;
            state->flags.h = (((*lft & 0xf) + (*rgt & 0xf)) & 0x10) > 1;
            *lft = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->r_pc += 1;
}

void and8(state_t *state, uint8_t *r) {
    state->r_a = state->r_a & *r;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.h = 1;
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) == 0x80);
    state->r_pc++;
}

void cp8(state_t *state, uint8_t data) {
    uint8_t res = state->r_a - data;
    state->flags.c = (state->r_a < data);
    state->flags.n = 1;
    state->flags.h = (((res & 0xf) + (data & 0xf)) & 0x10) > 1;
    state->flags.z = (res == 0);
    state->flags.s = ((res & 0x80) == 0x80);
    if (((state->r_a ^ data) & 0x80) != 0 /* Not same sign */ 
        && ((data ^ res) & 0x80) == 0) /* Same sign */ 
        state->flags.pv = 1;
    else
        state->flags.pv = 0; 
    state->r_pc += 1;
}

void or8(state_t *state, uint8_t *r) {
    state->r_a = state->r_a | *r;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.h = 0;
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) == 0x80);
    state->r_pc += 1;
}

void xor8(state_t *state, uint8_t data) {
    state->r_a = state->r_a ^ data;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.h = 0;
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) == 0x80);
    state->r_pc++;
}

void stack_put16(state_t *state, uint16_t data) {
    state->memory[(uint16_t)(--state->r_sp)] = (data >> 8) & 0xff;
    state->memory[(uint16_t)(--state->r_sp)] = data & 0xff;
}

uint16_t stack_get16(state_t *state) {
    uint16_t res = state->memory[state->r_sp++] | (state->memory[state->r_sp++] << 8);
    return res;
}

void emul_unimplemented_op(uint8_t code) {
    printf("\nemul: unimplemented instruction: %02x\n", code);
    exit(EXIT_FAILURE);
}

void emulate_op_ed(state_t * state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x42: // sbc hl, bc
        {
            uint32_t res = state->r_hl - state->r_bc - state->flags.c;
            state->r_hl = (uint16_t)(res & 0xffff);
            state->flags.n = 1;
            if (((state->r_hl ^ (state->r_bc + state->flags.c)) & 0x80) != 0 /* Not same sign */ 
                && ((1 ^ (state->r_hl - (state->r_bc + state->flags.c))) & 0x80) == 0) /* Same sign */ 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->flags.c = (1 == ((res >> 16) & 0x1));
            state->flags.h = (((state->r_hl & 0xf) - ((state->r_bc + 1) & 0xf)) & 0x10) > 1; // TODO
            state->flags.z = (state->r_hl == 0);
            state->flags.s = ((state->r_hl & 0x80) == 0x80);
            state->r_pc += 2;
        }
        break;
    case 0x73: // ld (**), sp
        { 
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->memory[addr] = state->r_spl;
            state->memory[addr+1] = state->r_sph;
            state->r_pc += 4;
        }
        break;
    case 0x7b: // ld sp, (**)
        {
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->r_spl = state->memory[addr];
            state->r_sph = state->memory[addr+1];
            state->r_pc += 4;
        }
        break;
    case 0xb0: // ldir
        {
            do
            {
              state->memory[state->r_de] = state->memory[state->r_hl];
              state->r_de++;
              state->r_hl++;
              state->r_bc--;
            } while (state->r_bc > 0);
            state->flags.n = 0;
            state->flags.pv = 0;
            state->flags.h = 0;
            state->r_pc += 2;
        }
        break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void emulate_op_fd(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x09: // add iy, bc
        {
            uint32_t res = state->r_iy + state->r_bc;
            state->flags.h = (((state->r_iy & 0xf) + (state->r_bc & 0xf)) & 0x10) > 1;
            state->r_iy = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->r_pc += 2;
        }
        break;
    case 0x21: state->r_iy = *((uint16_t *)(&code[2])); state->r_pc += 4; break; // ld iy, **
    case 0x23: state->r_iy++; state->r_pc += 2; break;
    case 0x7e: // ld a, (ix + *)
      {
        int8_t c = code[2];
        state->r_a = state->memory[state->r_iy + c];
        state->r_pc += 3;
      }
      break;
    case 0xe1: state->r_iy = stack_get16(state); state->r_pc += 2; break;
    case 0xe5: stack_put16(state, state->r_iy); state->r_pc += 2; break;
    case 0xe9: // jp (iy) ????
      {
        state->r_pc = state->r_iy;
      }
      break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void emulate_op_cb(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x40: // bit 0, b
        {
            state->flags.n = 0;
            state->flags.h = 1;
            state->flags.z = (1 == (state->r_b & 0x1));
            state->r_pc += 2;
        }
        break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void emulate_op_dd_cb(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[3]) {
    case 0x46: // bit 0, (ix + *)
        {
            uint8_t addr = state->r_ix + code[2];
            uint8_t res = state->memory[addr];
            state->flags.z = (1 == (res & 0x1));
            state->flags.n = 0;
            state->flags.h = 1;
            state->r_pc += 4;
        }
        break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x %02x %02x\n", code[0], code[1], code[2], code[3]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void emulate_op_dd(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x09: // add ix, bc
        {
            uint32_t res = state->r_ix + state->r_bc;
            state->flags.h = (((state->r_ix & 0xf) + (state->r_bc & 0xf)) & 0x10) > 1;
            state->r_ix = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->r_pc += 2;
        }
        break;
    case 0x21: state->r_ix = *((uint16_t *)(&code[2])); state->r_pc += 4; break; // ld ix, **
    case 0x23: state->r_ix++; state->r_pc += 2; break;
    case 0x7e: // ld a, (ix + *)
      {
        int8_t c = code[2];
        state->r_a = state->memory[state->r_ix + c];
        state->r_pc += 3;
      }
      break;
    case 0x84: add8(state, state->r_ixh); state->r_pc++; break; // add ixh
    case 0x86: add8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break; // add (ix + *)
    case 0xcb: emulate_op_dd_cb(state); break; // IX bit instructions
    case 0xe1: state->r_ix = stack_get16(state); state->r_pc += 2; break;
    case 0xe5: stack_put16(state, state->r_ix); state->r_pc += 2; break;
    case 0xe9: // jp (ix) ????
      {
        state->r_pc = state->r_ix;
      }
      break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void dec8(state_t *state, uint8_t *r) {
    // print_state(state);
    if (((*r ^ 1) & 0x80) != 0 /* Not same sign */ 
        && ((1 ^ (*r - 1)) & 0x80) == 0) /* Same sign */ 
        state->flags.pv = 1;
    else
        state->flags.pv = 0; 
    *r = *r - 1;
    state->flags.n = 0;
    state->flags.h = (((*r & 0xf) + (1 & 0xf)) & 0x10) > 1;
    state->flags.z = (*r == 0);
    state->flags.s = ((*r & 0x80) == 0x80);
    state->r_pc += 1;
}

void inc8(state_t *state, uint8_t *r) {
    if (((*r ^ 1) & 0x80) == 0 
        && ((1 ^ (*r + 1)) & 0x80) != 0) 
        state->flags.pv = 1;
    else
        state->flags.pv = 0; 
    *r += 1;
    state->flags.n = 0;
    state->flags.h = (((*r & 0xf) + (1 & 0xf)) & 0x10) > 1;
    state->flags.z = (*r == 0);
    state->flags.s = ((*r & 0x80) == 0x80);
    state->r_pc += 1;
}

int emulate_op(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];

    if (state->r_pc == 0) {
        printf("\nREACHED WBOOT - NORMAL EXIT");
        print_state(state);
        exit(EXIT_SUCCESS);
    }

    disassemble_op(state);

    switch(*code) {
    case 0x00: state->r_pc++; break; // nop
    case 0x01: // ld bc, **
        {
            state->r_bc = *((uint16_t *)(&code[1]));
            state->r_pc += 3;
        }
        break;
    case 0x04: inc8(state, &state->r_b); break; // inc b
    case 0x05: dec8(state, &state->r_b); break; // dec b
    case 0x06: // ld b, *
        {
            state->r_b = code[1];
            state->r_pc += 2;
        }
        break;
    case 0x07: // rlca
        {
            uint16_t res = (state->r_a << 1) | (state->r_a >> 7);
            state->r_a = res & 0xff;
            state->flags.n = 0;
            state->flags.c = res >> 8;
            state->flags.h = 0;
            state->r_pc++;
        }
        break;
    case 0x08: // ex af, af'
      {
        uint16_t tmp = state->r_af;
        state->r_af = state->_r_af;
        state->_r_af = tmp;
        state->r_pc++;
      }
      break;
    case 0x09: // add hl, bc
        {
            uint32_t res = state->r_hl + state->r_bc;
            state->flags.h = (((state->r_hl & 0xf) + (state->r_bc & 0xf)) & 0x10) > 1;
            state->r_hl = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->r_pc += 1;
        }
        break;
    case 0x0b: // dec bc
        {
            state->r_bc--;
            state->r_pc++;
        }
        break;
    case 0x0d: dec8(state, &state->r_c); break; // dec b
    case 0x0e: // ld c, *
        {
            state->r_c = state->memory[state->r_pc + 1];
            state->r_pc += 2;
        }
        break;
    case 0x0f: // rrca
        {
            uint8_t x = state->r_a;
            state->r_a = ((x & 1) << 7) | (x >> 1);
            state->flags.h = 0;
            state->flags.n = 0;
            state->flags.c = (1 == (x & 0x1));
            state->r_pc++;
        }
        break;
    case 0x10: // djnz *
      {
        int8_t c = code[1];
        state->r_b--;
        if (state->r_b != 0) {
          state->r_pc += c;
          state->r_pc += 2;
        } else {
          state->r_pc += 2;
        }
        // print_state(state);
      }
      break;
    case 0x11: state->r_de = *((uint16_t *)(&code[1])); state->r_pc += 3; break; // ld de, **
    case 0x12: // ld (de), a
        {
            state->memory[state->r_de] = state->r_a;
            state->r_pc++;
        }
        break;
    case 0x13: // inc de
        {
            state->r_de++;
            state->r_pc++;
        }
        break;
    case 0x14: inc8(state, &state->r_d); break; // inc d
    case 0x16: // ld d, *
        {
            state->r_d = code[1];
            state->r_pc += 2;
        }
        break;
    case 0x19: // add hl, de
        {
            uint32_t res = state->r_hl + state->r_de;
            state->flags.h = (((state->r_hl & 0xf) + (state->r_de & 0xf)) & 0x10) > 1;
            state->r_hl = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->r_pc += 1;
        }
        break;
    case 0x1a: state->r_a = state->memory[state->r_de]; state->r_pc++; break; // ld a, (de)
    case 0x20: // jr nz, *
      {
        int8_t c = code[1];
        if (state->flags.z != 1)
          state->r_pc += c + 2;
        else
          state->r_pc += 2;
      }
      break;
    case 0x21: // ld hl, **
        {
            state->r_hl = *((uint16_t *)(&code[1]));
            state->r_pc += 3;
        }
        break;
    case 0x22: state->memory[*((uint16_t *)(&code[1]))] = state->r_hl; state->r_pc += 3; break; // ld (**), hl
    case 0x23: state->r_hl++; state->r_pc++; break; // inc hl
    case 0x26: state->r_h = code[1]; state->r_pc += 2; break; // ld h, *
    case 0x28: // jr z, *
      {
        int8_t c = code[1];
        if (state->flags.z == 1)
          state->r_pc += c + 2;
        else
          state->r_pc += 2;
      }
      break;
    case 0x29: // add hl, hl
        {
            uint32_t res = state->r_hl + state->r_hl;
            state->flags.c = (res >> 16) & 0x1;;
            state->flags.n = 0;
            state->flags.h = (((state->r_hl & 0xf) + (state->r_hl & 0xf)) & 0x10) > 1;;
            state->r_hl = (uint16_t)(res & 0xffff);
            state->r_pc++;
        }
        break;
    case 0x2a: // ld hl, (**)
        {
            uint8_t addr = *((uint16_t *)(&code[1]));
            state->r_l = state->memory[addr];
            state->r_h = state->memory[addr + 1];
            state->r_pc += 3;
        }
        break;
    case 0x2b: // dec hl
        {
            state->r_hl--;
            state->r_pc++;
        }
        break;
    case 0x30: // jr nc, *
      {
        int8_t c = code[1];
        if (state->flags.c != 1)
          state->r_pc += c + 2;
        else
          state->r_pc += 2;
      }
      break;
    case 0x31: state->r_sp = *((uint16_t *)(&code[1])); state->r_pc += 3; break; // ld sp, **
    case 0x32: // ld (**), a
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
            state->memory[addr] = state->r_a;
            state->r_pc += 3;
        }
        break;
    case 0x34: // inc (hl)
        {
            if (((state->memory[state->r_hl] ^ 1) & 0x80) == 0 
                && ((1 ^ (state->memory[state->r_hl] + 1)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->memory[state->r_hl]++;
            state->flags.n = 0;
            state->flags.h = (((state->memory[state->r_hl] & 0xf) + (1 & 0xf)) & 0x10) > 1;
            state->flags.z = (state->memory[state->r_hl] == 0);
            state->flags.s = ((state->memory[state->r_hl] & 0x80) == 0x80);
            state->r_pc += 1;
        }
        break;
    case 0x36: // ld (hl), *
        {
            state->memory[state->r_hl] = code[1];
            state->r_pc += 2;
        }
        break;
    case 0x38: // jr c, *
      {
        int8_t c = code[1];
        if (state->flags.c == 1)
          state->r_pc += c + 2;
        else
          state->r_pc += 2;
      }
      break;
    case 0x3a: // ld a, (**)
        {
            state->r_a = state->memory[*((uint16_t *)(&code[1]))];
            state->r_pc += 3;
        }
        break;
    case 0x3c: inc8(state, &state->r_a); break; // inc a
    case 0x3e: state->r_a = code[1]; state->r_pc += 2; break; // ld a, *
        
    case 0x40: state->r_b = state->r_b; state->r_pc++; break; // ld b, b
    case 0x41: state->r_b = state->r_c; state->r_pc++; break; // ld b, c
    case 0x42: state->r_b = state->r_d; state->r_pc++; break; // ld b, d
    case 0x43: state->r_b = state->r_e; state->r_pc++; break; // ld b, e
    case 0x44: state->r_b = state->r_h; state->r_pc++; break; // ld b, h
    case 0x45: state->r_b = state->r_l; state->r_pc++; break; // ld b, l
    case 0x46: state->r_b = state->memory[state->r_hl]; state->r_pc++; break; // ld b, (hl)
    case 0x47: state->r_b = state->r_a; state->r_pc++; break; // ld b, a
    case 0x48: state->r_c = state->r_b; state->r_pc++; break; // ld c, b
    case 0x49: state->r_c = state->r_c; state->r_pc++; break; // ld c, c
    case 0x4a: state->r_c = state->r_d; state->r_pc++; break; // ld c, d
    case 0x4b: state->r_c = state->r_e; state->r_pc++; break; // ld c, e
    case 0x4c: state->r_c = state->r_h; state->r_pc++; break; // ld c, h
    case 0x4d: state->r_c = state->r_l; state->r_pc++; break; // ld c, l
    case 0x4e: state->r_c = state->memory[state->r_hl]; state->r_pc++; break; // ld c, (hl)
    case 0x4f: state->r_c = state->r_a; state->r_pc++; break; // ld c, a

    case 0x54: state->r_d = state->r_h; state->r_pc++; break; // ld d, h
    case 0x5d: state->r_e = state->r_l; state->r_pc++; break;  // ld e, l
    case 0x5e: state->r_e = state->memory[state->r_hl]; state->r_pc++; break; // ld e, (hl)
    case 0x5f: state->r_e = state->r_a; state->r_pc++; break; // ld e, a
    case 0x65: state->r_h = state->r_l; state->r_pc++; break; // ld h, l
    case 0x66: state->r_h = state->memory[state->r_hl]; state->r_pc++; break; // ld h, (hl)
    case 0x6f: state->r_l = state->r_a; state->r_pc++; break; // ld l, a

    case 0x77: state->memory[state->r_hl] = state->r_a; state->r_pc++; break; // ld (hl), a
    case 0x78: state->r_a = state->r_b; state->r_pc++; break; // ld a, b
    case 0x79: state->r_a = state->r_c; state->r_pc++; break; // ld ld a, c
    case 0x7a: state->r_a = state->r_d; state->r_pc++; break; // ld ld a, d
    case 0x7b: state->r_a = state->r_e; state->r_pc++; break; // ld a, e
    case 0x7c: state->r_a = state->r_h; state->r_pc++; break; // ld a, h
    case 0x7d: state->r_a = state->r_l; state->r_pc++; break; // ld a, l
    case 0x7e: state->r_a = state->memory[state->r_hl]; state->r_pc++; break; // ld a, (hl)

    case 0x80: add8(state, state->r_b); break;

    case 0xa0: and8(state, &state->r_b); break; // and b
    case 0xa1: and8(state, &state->r_c); break; // and c
    case 0xa8: xor8(state, state->r_b); break; // xor b
    case 0xa9: xor8(state, state->r_c); break; // xor c
    case 0xaa: xor8(state, state->r_d); break; // xor d
    case 0xab: xor8(state, state->r_e); break; // xor e
    case 0xac: xor8(state, state->r_h); break; // xor h
    case 0xad: xor8(state, state->r_l); break; // xor l
    case 0xae: xor8(state, state->memory[state->r_hl]); break; // xor (hl)
    case 0xaf: xor8(state, state->r_a); break; // xor a
    case 0xb6: or8(state, &state->memory[state->r_hl]); break;// or (hl)
    case 0xb7: or8(state, &state->r_a); break; // or a
    case 0xbe: cp8(state, state->memory[state->r_hl]); break; // cp (hl)

    case 0xc0: // ret nz
      {
        if (state->flags.z != 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xc1: state->r_bc = stack_get16(state); state->r_pc += 1; break; // pop bc
    case 0xc2: // jp nz, **
        {
            if (state->flags.z != 1)
                state->r_pc = *((uint16_t *)(&code[1]));
            else
                state->r_pc += 3;
        }
        break;
    case 0xc3: state->r_pc = *((uint16_t *)(&code[1])); break; // jp **
    case 0xc4: // call nz, **
        {
            state->r_pc += 3;
            if (state->flags.z != 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xc5: // push bc
        {
            stack_put16(state, state->r_bc);
            state->r_pc += 1;
        }
        break;
    case 0xc6: add8(state, code[1]); state->r_pc++; break; // add a, *
    case 0xc8: // ret z
      {
        if (state->flags.z == 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xc9: state->r_pc = stack_get16(state); break; // ret
    case 0xca: // jp z, **
        {
            if (state->flags.z == 1) {
                state->r_pc = *((uint16_t *)(&code[1]));
            } else {
                state->r_pc += 3;
            }
        }
        break;
    case 0xcb: emulate_op_cb(state); break;
    case 0xcc: // call z, **
        {
            state->r_pc += 3;
            if (state->flags.z == 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xcd: // call **
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
            state->r_pc += 3;
            if (5 == addr) {
                cpm_bdos(state);
            } else if (0 == addr) {
                exit(EXIT_SUCCESS);
            } else {
                stack_put16(state, state->r_pc);
                state->r_pc = addr;
            }
        }
        break;
    case 0xd0: // ret nc
      {
        if (state->flags.c != 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xd1: // pop de
        {
            state->r_de = stack_get16(state);
            state->r_pc += 1;
        }
        break;
    case 0xd2: // jp nc, **
        {
            if (state->flags.c != 1) {
                state->r_pc = *((uint16_t *)(&code[1]));
            } else {
                state->r_pc += 3;
            }
        }
        break;
    case 0xd4: // call nc, **
        {
            state->r_pc += 3;
            if (state->flags.c != 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xd5: // push de
        {
            stack_put16(state, state->r_de);
            state->r_pc += 1;
        }
        break;
    case 0xd8: // ret c
      {
        if (state->flags.c == 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xd9: // exx
      {
        uint16_t tmp;
        tmp = state->r_bc;
        state->r_bc = state->_r_bc;
        state->_r_bc = tmp;
        tmp = state->r_de;
        state->r_de = state->_r_de;
        state->_r_de = tmp;
        tmp = state->r_hl;
        state->r_hl = state->_r_hl;
        state->_r_hl = tmp;
        state->r_pc++;
      }
      break;
    case 0xda: // jp c, **
        {
            if (state->flags.c == 1) {
                state->r_pc = *((uint16_t *)(&code[1]));
            } else {
                state->r_pc += 3;
            }
        }
        break;
    case 0xdc: // call c, **
        {
            state->r_pc += 3;
            if (state->flags.c == 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xdd: emulate_op_dd(state); break;
        
    case 0xe0: // ret po
      {
        if (state->flags.pv != 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xe1: // pop hl
        {
            state->r_hl = stack_get16(state);
            state->r_pc += 1;
        }
        break;
    case 0xe2: // jp po, **
        {
            if (state->flags.pv != 1)
                state->r_pc = *((uint16_t *)(&code[1]));
            else
                state->r_pc += 3;
        }
        break;
    case 0xe4: // call po, **
        {
            state->r_pc += 3;
            if (state->flags.pv != 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xe5: // push hl
        {
            stack_put16(state, state->r_hl);
            state->r_pc += 1;
        }
        break;
    case 0xe6: // and *
        {
            state->r_a &= code[1];
            state->flags.c = 0;
            state->flags.n = 0;
            state->flags.h = 1;
            state->flags.pv = parity8(state->r_a);
            state->flags.z = (state->r_a == 0);
            state->flags.s = ((state->r_a & 0x80) == 0x80);
            state->r_pc += 2;
        }
        break;
    case 0xe8: // ret pe
      {
        if (state->flags.pv == 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xe9: // jp (hl) ????
      {
        state->r_pc = state->r_hl;
      }
      break;
    case 0xea: // jp pe, **
        {
            if (state->flags.pv == 1)
                state->r_pc = *((uint16_t *)(&code[1]));
            else
                state->r_pc += 3;
        }
        break;
    case 0xeb: //ex de, hl
        {
            uint16_t temp = state->r_de;
            state->r_de = state->r_hl;
            state->r_hl = temp;
            state->r_pc += 1;
        }
        break;
    case 0xec: // call pe, **
        {
            state->r_pc += 3;
            if (state->flags.pv == 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xed: // extended op
        {
            emulate_op_ed(state);
        }
        break;
    case 0xf0: // ret p
      {
        if (state->flags.s != 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xf1: // pop af
        {
            state->r_af = stack_get16(state);
            state->r_pc += 1;
        }
        break;
    case 0xf2: // jp p, **
        {
            if (state->flags.s != 1)
                state->r_pc = *((uint16_t *)(&code[1]));
            else
                state->r_pc += 3;
        }
        break;
    case 0xf3: /* printf("DI not implemented. Skipped\n"); */ state->r_pc++; break;
    case 0xf4: // call p, **
        {
            state->r_pc += 3;
            if (state->flags.s != 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xf5: // push af
        {
            stack_put16(state, state->r_af);
            state->r_pc += 1;
        }
        break;
    case 0xf8: // ret m
      {
        if (state->flags.s == 1)
          state->r_pc = stack_get16(state);
        else
          state->r_pc++;
      }
      break;
    case 0xf9: // ld sp, hl
        {
            state->r_sp = state->r_hl;
            state->r_pc += 1;
        }
        break;
    case 0xfa: // jp m, **
        {
            if (state->flags.s == 1)
                state->r_pc = *((uint16_t *)(&code[1]));
            else
                state->r_pc += 3;
        }
        break;
    case 0xfb: state->r_pc++; break; // ei // TODO
    case 0xfc: // call m, **
        {
            state->r_pc += 3;
            if (state->flags.s == 1) {
                stack_put16(state, state->r_pc);
                state->r_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xfd: emulate_op_fd(state); break;
    case 0xfe: cp8(state, code[1]); state->r_pc++; break; // cp *
    default: emul_unimplemented_op(*code); break;
    }

    // print_state(state);

    
    return 0;
};
