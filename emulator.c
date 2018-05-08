#include <stdio.h>
#include "emulator.h"
#include "disassembler.h"
#include "cpm.h"


#define COND_PE state->flags.pv == 1
#define COND_P state->flags.s == 0
#define COND_PO state->flags.pv == 0
#define COND_C state->flags.c == 1
#define COND_NC state->flags.c == 0
#define COND_Z state->flags.z == 1
#define COND_NZ state->flags.z == 0
#define COND_M state->flags.s == 1

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

void print_state(state_t *state) {
    printf("\t==PC:%04x SP:%04x A:%02x F:%02x B:%02x C:%02x D:%02x E:%02x H:%02x L:%02x IX: %04x\n",
           state->r_pc, state->r_sp, state->r_a, state->r_f, state->r_b, state->r_c,
           state->r_d, state->r_e, state->r_h, state->r_l, state->r_ix);
    // printf("\t==PCH:%02x PCL:%02x\n", state->r_pch, state->r_pcl);
    printf("\t == C: %02x\n", state->flags.c);
    /* printf("\t==Z:%01x S:%01x P:%01x Cy:%01x Ac:%01x\n", */
    /*        state->cc.z, state->cc.s, state->cc.p, state->cc.cy, state->cc.ac); */
}

void add16(state_t *state, uint16_t *r1, uint16_t *r2) {
    uint32_t res = *r1 + *r2;
    state->flags.h = (((*r1 & 0xf) + (*r2 & 0xf)) & 0x10) > 1;
    *r1 = res & 0xffff;
    state->flags.c = (res >> 16) & 0x1;
    state->flags.n = 0;
    state->flags.f3 = ((*r1 >> 8) & 0b0001000) >> 3;
    state->flags.f5 = ((*r1 >> 8) & 0b0100000) >> 5;
}

void add8(state_t *state, uint8_t val) {
  uint16_t total = state->r_a;
  state->flags.h = ((((total & 0xf) + (val & 0xf)) & 0x10) != 0);

  total += val;
  state->flags.c = ((total & 0x100) != 0);
  state->flags.s = ((total & 0x80) != 0);
  state->flags.z = ((total & 0xff) == 0);
  state->flags.n = 0;

  uint16_t sign1 = state->r_a & 0x80;
  uint16_t sign2 = val & 0x80;
  uint16_t sign3 = total & 0x80;
  state->flags.pv = (sign1 == sign2 && sign3 != sign1);
  state->flags.f3 = (total & 0b0001000) >> 3;
  state->flags.f5 = (total & 0b0100000) >> 5;

  state->r_a = total & 0xff;
  state->r_pc++;
}

void adc8(state_t *state, uint8_t val) {
  uint16_t total = state->r_a;
  uint16_t value = val + state->flags.c;
  state->flags.h = ((((total & 0xf) + (value & 0xf)) & 0x10) != 0);
  total += value;
  state->flags.c = ((total & 0x100) != 0);
  state->flags.s = ((total & 0x80) != 0);
  state->flags.z = ((total & 0xff) == 0);
  state->flags.n = 0;

  uint16_t sign1 = state->r_a & 0x80;
  uint16_t sign2 = value & 0x80;
  uint16_t sign3 = total & 0x80;
  state->flags.pv = (sign1 == sign2 && sign3 != sign1);
  state->flags.f3 = (total & 0b0001000) >> 3;
  state->flags.f5 = (total & 0b0100000) >> 5;
  state->r_a = total & 0xff;
  state->r_pc++;
}

void sub8(state_t *state, uint8_t val) {
  uint16_t total = state->r_a;
  state->flags.h = ((((total & 0xf) - (val & 0xf)) & 0x10) != 0);
  total -= val;
  state->flags.c = ((total & 0x100) != 0);
  state->flags.s = ((total & 0x80) != 0);
  state->flags.z = (total == 0);
  state->flags.n = 1;

  uint16_t sign1 = state->r_a & 0x80;
  uint16_t sign2 = val & 0x80;
  uint16_t sign3 = total & 0x80;
  state->flags.pv = (sign1 != sign2 && sign3 != sign1);
  state->r_a = total & 0xff;
  state->flags.f3 = (state->r_a & 0b0001000) >> 3;
  state->flags.f5 = (state->r_a & 0b0100000) >> 5;
  state->r_pc++;
}

void sbc8(state_t *state, uint8_t val) {
  uint16_t total = state->r_a;
  uint16_t value = val + state->flags.c;
  state->flags.h = ((((total & 0x0f) - (value & 0x0f)) & 0x10) != 0);
  total -= value;
  state->flags.c = ((total & 0x100) != 0);

  uint16_t sign1 = state->r_a & 0x80;
  uint16_t sign2 = value & 0x80;
  uint16_t sign3 = total & 0x80;
  state->flags.pv = (sign1 != sign2 && sign3 != sign1);
  state->flags.s = ((total & 0x80) != 0);
  state->flags.z = (total == 0);
  state->flags.n = 1;
  state->r_a = total & 0xff;
  state->flags.f3 = (state->r_a & 0b0001000) >> 3;
  state->flags.f5 = (state->r_a & 0b0100000) >> 5;
  state->r_pc++;
}

void and8(state_t *state, uint8_t *r) {
    state->r_a = state->r_a & *r;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.h = 1;
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) == 0x80);
    state->flags.f3 = (state->r_a & 0b0001000) >> 3;
    state->flags.f5 = (state->r_a & 0b0100000) >> 5;
    state->r_pc++;
}

void cp8(state_t *state, uint8_t data) {
    uint16_t res = state->r_a - data;
    state->flags.h = ((((state->r_a & 0xf) - (data & 0xf)) & 0x10) != 0);
    state->flags.n = 1;
    state->flags.s = ((res & 0x80) != 0);
    state->flags.c = ((res & 0x100) != 0);
    state->flags.z = ((res & 0xff) == 0);
    uint8_t sign1 = state->r_a & 0x80;
    uint8_t sign2 = data & 0x80;
    uint8_t sign3 = res & 0x80;
    state->flags.pv = (sign1 != sign2 && sign3 != sign1); 
    state->flags.f3 = (data & 0b0001000) >> 3;
    state->flags.f5 = (data & 0b0100000) >> 5;
    state->r_pc += 1;
}

void or8(state_t *state, uint8_t *r) {
    state->r_a |= *r;
    state->flags.h = 0;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) != 0);
    state->flags.f3 = (state->r_a & 0b0001000) >> 3;
    state->flags.f5 = (state->r_a & 0b0100000) >> 5;
    state->r_pc += 1;
}

void xor8(state_t *state, uint8_t data) {
    state->r_a ^= data;
    state->flags.h = 0;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->r_a);
    state->flags.z = (state->r_a == 0);
    state->flags.s = ((state->r_a & 0x80) != 0);
    state->flags.f3 = (state->r_a & 0b0001000) >> 3;
    state->flags.f5 = (state->r_a & 0b0100000) >> 5;
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
}

void ret(state_t *state, int cond) {
    if (cond)
        state->r_pc = stack_get16(state);
    else
        state->r_pc++;
}

void call(state_t *state, int cond, uint16_t addr) {
    state->r_pc += 3;
    if (cond) {
        stack_put16(state, state->r_pc);
        state->r_pc = addr;
    }
}

void jp(state_t *state, int cond, uint16_t addr) {
    if (cond)
        state->r_pc = addr;
    else
        state->r_pc += 3;
}

void bit8(state_t *state, int n, uint8_t val) {
    // BIT n,r               *Z513*0-  PV as Z, S set only if n=7 and b7 of r set
    state->flags.n = 0;
    state->flags.h = 1;
    state->flags.z = (0 == (val & (0b1 << n)));
    state->flags.pv = state->flags.z;
    state->flags.f3 = (val & 0b0001000) >> 3;
    state->flags.f5 = (val & 0b0100000) >> 5;
    state->flags.s = (n == 7 && (val & 0b1000000) == 1);
}

void sbc16(state_t *state, uint16_t *r1, uint16_t val) {
    uint32_t total = *r1;
    uint32_t value = val + state->flags.c;
    state->flags.h = ((((total & 0x0fff) - (value & 0x0fff)) & 0x1000) != 0);
    total -= value;
    state->flags.c = ((total & 0x10000) != 0);

    uint32_t sign1 = *r1 & 0x8000;
    uint32_t sign2 = value & 0x8000;
    uint32_t sign3 = total & 0x8000;
    state->flags.pv = (sign1 != sign2 && sign3 != sign1);
    state->flags.s = ((total & 0x8000) != 0);
    state->flags.z = (total == 0);
    state->flags.n = 1;
    state->r_hl = total & 0xffff;
    state->flags.f3 = ((*r1 >> 8) & 0b0001000) >> 3;
    state->flags.f5 = ((*r1 >> 8) & 0b0100000) >> 5;
}

void adc16(state_t *state, uint16_t *r1, uint16_t *r2) {
    uint32_t total = *r1;
    uint32_t value = *r2 + state->flags.c;
    state->flags.h = ((((total & 0x0fff) + (value & 0x0fff)) & 0x1000) != 0);
    total += value;
    state->flags.c = ((total & 0x10000) != 0);
    
    uint32_t sign1 = *r1 & 0x8000;
    uint32_t sign2 = value & 0x8000;
    uint32_t sign3 = total & 0x8000;
    state->flags.pv = (sign1 == sign2 && sign3 != sign1);
    state->flags.s = ((total & 0x8000) != 0);
    state->flags.z = (total == 0);
    state->flags.n = 0;
    *r1 = total;
    state->flags.f3 = ((*r1 >> 8) & 0b0001000) >> 3;
    state->flags.f5 = ((*r1 >> 8) & 0b0100000) >> 5;
}

void emulate_op_ed(state_t * state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x42: sbc16(state,(uint16_t *)(void *)&state->r_hl, state->r_bc); state->r_pc += 2; break; // sbc hl, bc
    case 0x4a: adc16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_bc); state->r_pc += 2; break; // adc hl, bc
    case 0x52: sbc16(state,(uint16_t *)(void *)&state->r_hl, state->r_de); state->r_pc += 2; break; // sbc hl, de
    case 0x5a: adc16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_de); state->r_pc += 2; break; // adc hl, de
    case 0x62: sbc16(state,(uint16_t *)(void *)&state->r_hl, state->r_hl); state->r_pc += 2; break; // sbc hl, hl
    case 0x6a: adc16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_hl); state->r_pc += 2; break; // adc hl, hl
    case 0x72: sbc16(state,(uint16_t *)(void *)&state->r_hl, state->r_sp); state->r_pc += 2; break; // sbc hl, sp
    case 0x73: // ld (**), sp
        { 
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->memory[addr] = state->r_spl;
            state->memory[addr+1] = state->r_sph;
            state->r_pc += 4;
        }
        break;
    case 0x7a: adc16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_sp); state->r_pc += 2; break; // adc hl, sp
    case 0x7b: // ld sp, (**)
        {
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->r_spl = state->memory[addr];
            state->r_sph = state->memory[addr+1];
            state->r_pc += 4;
        }
        break;
    case 0xa1: // cpi
        {
            uint16_t res = state->memory[state->r_hl] - state->r_a;
            state->flags.n = 1;
            
            state->flags.f3 = (res & 0b0001000) >> 3;
            state->flags.f5 = (res & 0b0100000) >> 5;

            state->r_pc += 2;
            state->r_hl++;
            state->r_bc--;
        }
        break;
    case 0xa9: // cpd
        {
            uint16_t res = state->r_a - state->memory[state->r_hl];
            state->flags.h = ((((state->r_a & 0xf) - (state->memory[state->r_hl] & 0xf)) & 0x10) != 0);
            state->flags.n = 1;
            
            state->flags.f3 = ((res - state->flags.h) & 0b0001000) >> 3;
            state->flags.f5 = ((res - state->flags.h) & 0b0000010) >> 1;

            state->r_pc += 2;
            state->r_hl--;
            state->r_bc--;
        }
        break;
    case 0xb0: // ldir
        {
            do
            {
                uint8_t b = state->memory[state->r_hl];
                state->memory[state->r_de] = b;
                state->flags.f3 = ((b + state->r_a) & 0b00001000) >> 3;
                state->flags.f5 = ((b + state->r_a) & 0b00100000) >> 5;
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
    case 0xb9: // cpdr
        {
            uint16_t res;
            do
            {
                state->flags.pv = state->r_bc != 0;

                res = state->r_a - state->memory[state->r_hl];
                state->flags.h = ((((state->r_a & 0xf) - (state->memory[state->r_hl] & 0xf)) & 0x10) != 0);
                state->flags.n = 1;
                state->flags.s = ((res & 0x80) != 0);
                state->flags.z = ((res & 0xff) == 0);

                state->flags.f3 = ((res - state->flags.h) & 0b0001000) >> 3;
                state->flags.f5 = ((res - state->flags.h) & 0b0000010) >> 1;

                state->r_hl--;
            } while (--state->r_bc && res);
            state->flags.pv = state->r_bc != 0;
            state->r_pc += 2;
        }
        break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            print_state(state);
            exit(EXIT_FAILURE);
        }
        break;
    }
}


void emulate_fd_cb(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[3]) {
        // FDCB**NN
    case 0x01: // rlc (iy + *), c
        {
            uint16_t addr = state->r_iy + code[3];
            uint8_t val = state->memory[addr];
            state->flags.c = (val & 0x80) == 1;
            val = val << 1;
            val |= state->flags.c;
            state->r_c = val;
            state->memory[addr] = val;
            state->r_pc += 4;
        }
        break;
    case 0x46: // bit 0, (iy + *)
        {
            int8_t val = code[2];
            uint16_t addr = state->r_iy + val;
            uint8_t mem = state->memory[addr];
            bit8(state, 0, mem);
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

void emulate_op_fd(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x09: add16(state, (uint16_t *)(void *)&state->r_iy, (uint16_t *)(void *)&state->r_bc); state->r_pc += 2; break; // add iy, bc
    case 0x19: add16(state, (uint16_t *)(void *)&state->r_iy, (uint16_t *)(void *)&state->r_de); state->r_pc += 2; break; // add iy, de
    case 0x21: state->r_iy = *((uint16_t *)(&code[2])); state->r_pc += 4; break; // ld iy, **
    case 0x23: state->r_iy++; state->r_pc += 2; break;
    case 0x29: add16(state, (uint16_t *)(void *)&state->r_iy, (uint16_t *)(void *)&state->r_iy); state->r_pc += 2; break; // add iy, iy
    case 0x39: add16(state, (uint16_t *)(void *)&state->r_iy, (uint16_t *)(void *)&state->r_sp); state->r_pc += 2; break; // add iy, sp
    case 0x7e: // ld a, (iy + *)
      {
        int8_t c = code[2];
        state->r_a = state->memory[state->r_iy + c];
        state->r_pc += 3;
      }
      break;
    case 0x84: add8(state, state->r_iyh); state->r_pc++; break;
    case 0x85: add8(state, state->r_iyl); state->r_pc++; break;
    case 0x86:
        {
            int8_t val = code[2];
            add8(state, state->memory[(uint16_t)(state->r_iy + val)]);
            state->r_pc += 2;
        }
        break;
    case 0x8c: adc8(state, state->r_iyh); state->r_pc++; break;
    case 0x8d: adc8(state, state->r_iyl); state->r_pc++; break;
    case 0x8e: adc8(state, state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;

    case 0x94: sub8(state, state->r_iyh); state->r_pc++; break;
    case 0x95: sub8(state, state->r_iyl); state->r_pc++; break;
    case 0x96: sub8(state, state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;
    case 0x9c: sbc8(state, state->r_iyh); state->r_pc++; break;
    case 0x9d: sbc8(state, state->r_iyl); state->r_pc++; break;
    case 0x9e:
        {
            int8_t val = code[2];
            sbc8(state, state->memory[(uint16_t)(state->r_iy + val)]);
            state->r_pc += 2;
        }
        break;
    case 0xa4: and8(state, &state->r_iyh); state->r_pc++; break;
    case 0xa5: and8(state, &state->r_iyl); state->r_pc++; break;
    case 0xa6: and8(state, &state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;
    case 0xac: xor8(state, state->r_iyh); state->r_pc++; break;
    case 0xad: xor8(state, state->r_iyl); state->r_pc++; break;
    case 0xae: xor8(state, state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;

    case 0xb4: or8(state, &state->r_iyh); state->r_pc++; break;
    case 0xb5: or8(state, &state->r_iyl); state->r_pc++; break;
    case 0xb6: or8(state, &state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;
    case 0xbc: cp8(state, state->r_iyh); state->r_pc++; break;
    case 0xbd: cp8(state, state->r_iyl); state->r_pc++; break;
    case 0xbe: cp8(state, state->memory[(uint16_t)(state->r_iy + code[2])]); state->r_pc += 2; break;

    case 0xcb: emulate_fd_cb(state); break;
        
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
    case 0x40: bit8(state, 0, state->r_b); state->r_pc += 2; break;
    case 0x41: bit8(state, 0, state->r_c); state->r_pc += 2; break;
    case 0x42: bit8(state, 0, state->r_d); state->r_pc += 2; break;
    case 0x43: bit8(state, 0, state->r_e); state->r_pc += 2; break;
    case 0x44: bit8(state, 0, state->r_h); state->r_pc += 2; break;
    case 0x45: bit8(state, 0, state->r_l); state->r_pc += 2; break;
    case 0x46: bit8(state, 0, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x47: bit8(state, 0, state->r_a); state->r_pc += 2; break;
    case 0x48: bit8(state, 1, state->r_b); state->r_pc += 2; break;
    case 0x49: bit8(state, 1, state->r_c); state->r_pc += 2; break;
    case 0x4a: bit8(state, 1, state->r_d); state->r_pc += 2; break;
    case 0x4b: bit8(state, 1, state->r_e); state->r_pc += 2; break;
    case 0x4c: bit8(state, 1, state->r_h); state->r_pc += 2; break;
    case 0x4d: bit8(state, 1, state->r_l); state->r_pc += 2; break;
    case 0x4e: bit8(state, 1, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x4f: bit8(state, 1, state->r_a); state->r_pc += 2; break;

    case 0x50: bit8(state, 2, state->r_b); state->r_pc += 2; break;
    case 0x51: bit8(state, 2, state->r_c); state->r_pc += 2; break;
    case 0x52: bit8(state, 2, state->r_d); state->r_pc += 2; break;
    case 0x53: bit8(state, 2, state->r_e); state->r_pc += 2; break;
    case 0x54: bit8(state, 2, state->r_h); state->r_pc += 2; break;
    case 0x55: bit8(state, 2, state->r_l); state->r_pc += 2; break;
    case 0x56: bit8(state, 2, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x57: bit8(state, 2, state->r_a); state->r_pc += 2; break;
    case 0x58: bit8(state, 3, state->r_b); state->r_pc += 2; break;
    case 0x59: bit8(state, 3, state->r_c); state->r_pc += 2; break;
    case 0x5a: bit8(state, 3, state->r_d); state->r_pc += 2; break;
    case 0x5b: bit8(state, 3, state->r_e); state->r_pc += 2; break;
    case 0x5c: bit8(state, 3, state->r_h); state->r_pc += 2; break;
    case 0x5d: bit8(state, 3, state->r_l); state->r_pc += 2; break;
    case 0x5e: bit8(state, 3, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x5f: bit8(state, 3, state->r_a); state->r_pc += 2; break;

    case 0x60: bit8(state, 4, state->r_b); state->r_pc += 2; break;
    case 0x61: bit8(state, 4, state->r_c); state->r_pc += 2; break;
    case 0x62: bit8(state, 4, state->r_d); state->r_pc += 2; break;
    case 0x63: bit8(state, 4, state->r_e); state->r_pc += 2; break;
    case 0x64: bit8(state, 4, state->r_h); state->r_pc += 2; break;
    case 0x65: bit8(state, 4, state->r_l); state->r_pc += 2; break;
    case 0x66: bit8(state, 4, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x67: bit8(state, 4, state->r_a); state->r_pc += 2; break;
    case 0x68: bit8(state, 5, state->r_b); state->r_pc += 2; break;
    case 0x69: bit8(state, 5, state->r_c); state->r_pc += 2; break;
    case 0x6a: bit8(state, 5, state->r_d); state->r_pc += 2; break;
    case 0x6b: bit8(state, 5, state->r_e); state->r_pc += 2; break;
    case 0x6c: bit8(state, 5, state->r_h); state->r_pc += 2; break;
    case 0x6d: bit8(state, 5, state->r_l); state->r_pc += 2; break;
    case 0x6e: bit8(state, 5, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x6f: bit8(state, 5, state->r_a); state->r_pc += 2; break;

    case 0x70: bit8(state, 6, state->r_b); state->r_pc += 2; break;
    case 0x71: bit8(state, 6, state->r_c); state->r_pc += 2; break;
    case 0x72: bit8(state, 6, state->r_d); state->r_pc += 2; break;
    case 0x73: bit8(state, 6, state->r_e); state->r_pc += 2; break;
    case 0x74: bit8(state, 6, state->r_h); state->r_pc += 2; break;
    case 0x75: bit8(state, 6, state->r_l); state->r_pc += 2; break;
    case 0x76: bit8(state, 6, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x77: bit8(state, 6, state->r_a); state->r_pc += 2; break;
    case 0x78: bit8(state, 7, state->r_b); state->r_pc += 2; break;
    case 0x79: bit8(state, 7, state->r_c); state->r_pc += 2; break;
    case 0x7a: bit8(state, 7, state->r_d); state->r_pc += 2; break;
    case 0x7b: bit8(state, 7, state->r_e); state->r_pc += 2; break;
    case 0x7c: bit8(state, 7, state->r_h); state->r_pc += 2; break;
    case 0x7d: bit8(state, 7, state->r_l); state->r_pc += 2; break;
    case 0x7e: bit8(state, 7, state->memory[state->r_hl]); state->r_pc += 2; break;
    case 0x7f: bit8(state, 7, state->r_a); state->r_pc += 2; break;

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
    switch(code[2]) {

    case 0x01: state->r_pc += 4; break;
        
    case 0x40 ... 0x47: bit8(state, 0, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x48 ... 0x4f: bit8(state, 1, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x50 ... 0x57: bit8(state, 2, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x58 ... 0x5f: bit8(state, 3, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x60 ... 0x67: bit8(state, 4, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x68 ... 0x6f: bit8(state, 5, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x70 ... 0x77: bit8(state, 6, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;
    case 0x78 ... 0x7f: bit8(state, 7, state->memory[state->r_ix + code[2]]); state->r_pc += 4; break;

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
    case 0x09: add16(state, (uint16_t *)(void *)&state->r_ix, (uint16_t *)(void *)&state->r_bc); state->r_pc += 2; break; // add ix, bc
    case 0x19: add16(state, (uint16_t *)(void *)&state->r_ix, (uint16_t *)(void *)&state->r_de); state->r_pc += 2; break; // add ix, de
    case 0x21: state->r_ix = *((uint16_t *)(&code[2])); state->r_pc += 4; break; // ld ix, **
    case 0x23: state->r_ix++; state->r_pc += 2; break;
    case 0x29: add16(state, (uint16_t *)(void *)&state->r_ix, (uint16_t *)(void *)&state->r_ix); state->r_pc += 2; break; // add ix, ix
    case 0x39: add16(state, (uint16_t *)(void *)&state->r_ix, (uint16_t *)(void *)&state->r_sp); state->r_pc += 2; break; // add ix, sp
    case 0x7e: // ld a, (ix + *)
      {
        int8_t c = code[2];
        state->r_a = state->memory[state->r_ix + c];
        state->r_pc += 3;
      }
      break;
    case 0x84: add8(state, state->r_ixh); state->r_pc++; break; // add ixh
    case 0x85: add8(state, state->r_ixl); state->r_pc++; break;
    case 0x86:
        {
            int8_t val = code[2];
            add8(state, state->memory[(uint16_t)(state->r_ix + val)]);
            state->r_pc += 2;
        }
        break; // add (ix + *)
    case 0x8c: adc8(state, state->r_ixh); state->r_pc++; break;
    case 0x8d: adc8(state, state->r_ixl); state->r_pc++; break;
    case 0x8e: adc8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;
      
    case 0x94: sub8(state, state->r_ixh); state->r_pc++; break;
    case 0x95: sub8(state, state->r_ixl); state->r_pc++; break;
    case 0x96: sub8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;
    case 0x9c: sbc8(state, state->r_ixh); state->r_pc++; break;
    case 0x9d: sbc8(state, state->r_ixl); state->r_pc++; break;
    case 0x9e: sbc8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;

    case 0xa4: and8(state, &state->r_ixh); state->r_pc++; break;
    case 0xa5: and8(state, &state->r_ixl); state->r_pc++; break;
    case 0xa6: and8(state, &state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;
    case 0xac: xor8(state, state->r_ixh); state->r_pc++; break;
    case 0xad: xor8(state, state->r_ixl); state->r_pc++; break;
    case 0xae: xor8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;

    case 0xb4: or8(state, &state->r_ixh); state->r_pc++; break;
    case 0xb5: or8(state, &state->r_ixl); state->r_pc++; break;
    case 0xb6: or8(state, &state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;
    case 0xbc: cp8(state, state->r_ixh); state->r_pc++; break;
    case 0xbd: cp8(state, state->r_ixl); state->r_pc++; break;
    case 0xbe: cp8(state, state->memory[(uint16_t)(state->r_ix + code[2])]); state->r_pc += 2; break;
      
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
    state->flags.n = 1;
    state->flags.pv = ((*r & 0x80) != 0 && ((*r - 1) & 0x80) == 0);
    *r = *r - 1;
    state->flags.h = ((*r & 0xf) == 0xf);
    state->flags.z = (*r == 0);
    state->flags.s = ((*r & 0x80) == 0x80);
    state->r_pc += 1;
    state->flags.f3 = (*r & 0b0001000) >> 3;
    state->flags.f5 = (*r & 0b0100000) >> 5;
}

void inc8(state_t *state, uint8_t *r) {
    state->flags.n = 0;
    state->flags.pv = ((*r & 0x80) == 0 && ((*r + 1) & 0x80) != 0);
    *r = *r + 1;
    state->flags.h = ((*r & 0xf) == 0);
    state->flags.z = (*r == 0);
    state->flags.s = ((*r & 0x80) != 0);
    state->r_pc += 1;
    state->flags.f3 = (*r & 0b0001000) >> 3;
    state->flags.f5 = (*r & 0b0100000) >> 5;
}

int emulate_op(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];

    if (state->r_pc > 0x0522 && state->r_pc < 0x05e2) {
        printf("%04x %02x %04x %04x %04x %04x %04x |%02x|  ",
               state->r_pc, code[0], state->r_af, state->r_bc, state->r_de, state->r_hl,
               state->r_sp, state->memory[0x1d89]);
        //printf("\n");
        disassemble_op(state);
    }
    
    switch(*code) {
    case 0x00: state->r_pc++; break; // nop
    case 0x01: // ld bc, **
        {
            state->r_bc = *((uint16_t *)(&code[1]));
            state->r_pc += 3;
        }
        break;
    case 0x02: state->memory[state->r_bc] = state->r_a; state->r_pc++; break;
    case 0x03: state->r_bc++; state->r_pc++; break; // inc bc
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
            uint8_t hbit = ((state->r_a & 0x80) != 0);
            state->r_a = (state->r_a << 1) & 0xff;
            if (hbit)
                state->r_a |= 0x01;
            state->flags.n = 0;
            if (hbit)
                state->flags.c = 1;
            else
                state->flags.c = 0;
            state->flags.h = 0;
            state->r_pc++;
            state->flags.f3 = (state->r_a & 0b0001000) >> 3;
            state->flags.f5 = (state->r_a & 0b0100000) >> 5;
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
     case 0x09: add16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_bc); state->r_pc++; break; // add hl, bc
     case 0x0a: state->r_a = state->memory[state->r_bc]; state->r_pc++; break;
     case 0x0b: state->r_bc--; state->r_pc++; break; // dec bc
    case 0x0c: inc8(state, &state->r_c); break; // inc c
    case 0x0d: dec8(state, &state->r_c); break; // dec b
    case 0x0e: state->r_c = code[1]; state->r_pc += 2; break; // ld c, *
    case 0x0f: // rrca
        {
            state->flags.c = (1 == (state->r_a & 0x01));
            state->r_a >>= 1;
            if (state->flags.c)
                state->r_a |= 0x80;
            state->flags.h = 0;
            state->flags.n = 0;
            state->r_pc++;
            state->flags.f3 = (state->r_a & 0b0001000) >> 3;
            state->flags.f5 = (state->r_a & 0b0100000) >> 5;
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
    case 0x15: dec8(state, &state->r_d); break; // dec d
     case 0x16: // ld d, *
         {
             state->r_d = code[1];
             state->r_pc += 2;
         }
         break;
     case 0x19: add16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_de); state->r_pc++; break; // add hl, de
     case 0x1a: state->r_a = state->memory[state->r_de]; state->r_pc++; break; // ld a, (de)
     case 0x1c: inc8(state, &state->r_e); break; // inc e
     case 0x1d: dec8(state, &state->r_e); break; // dec e
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
    case 0x22: // ld (**), hl
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
            state->memory[addr] = state->r_l;
            state->memory[addr+1] = state->r_h;
            state->r_pc += 3;
        }
        break;
    case 0x23: state->r_hl++; state->r_pc++; break; // inc hl
    case 0x24: inc8(state, &state->r_h); break; // inc h
    case 0x25: dec8(state, &state->r_h); break; // dec h
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
    case 0x27: // DAA
        /*
          Decimal Adjust Accumulator
          
          The eight-bit number in the accumulator is adjusted to form
          two four-bit Binary-Coded-Decimal digits by the following process:
          1. If the value of the least significant 4 bits of the accumulator
          is greater than 9 or if the AC flag is set, 6 is added to the accumulator.
          2. If the value of the most significant 4 bits of the accumulator
          is now greater than 9, or if the CY flag is set, 6 is added to
          the most significant 4 bits of the accumulator.
          NOTE: All flags are affected: Z, S, P, CY, AC
        */
        {
            // Step 1
            uint16_t res = state->r_a;
            if ((res & 0xf) > 9 || state->flags.c == 1) {
                res += 6;
            }

            // Step 2
            if (((res & 0xf0) >> 4) > 9 || state->flags.h == 1) {
                res += (6 << 4);
            }

            state->r_a = res & 0xff;
            state->flags.h = (res > 0xff);

            state->r_pc++;
        }
        break;
    case 0x29: add16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_hl); state->r_pc++; break; // add hl, hl
    case 0x2a: // ld hl, (**)
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
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
    case 0x2c: inc8(state, &state->r_l); break; // inc l
    case 0x2d: dec8(state, &state->r_l); break; // dec l
    case 0x2e: state->r_l = code[1]; state->r_pc += 2; break;
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
    case 0x33: state->r_sp++; state->r_pc++; break; // inc sp
    case 0x34: inc8(state, &state->memory[state->r_hl]); break; // inc (hl)
     case 0x36: state->memory[state->r_hl] = code[1]; state->r_pc += 2; break; // ld (hl), *
    case 0x38: // jr c, *
      {
        int8_t c = code[1];
        if (state->flags.c == 1)
          state->r_pc += c + 2;
        else
          state->r_pc += 2;
      }
      break;
     case 0x39: add16(state, (uint16_t *)(void *)&state->r_hl, (uint16_t *)(void *)&state->r_sp); state->r_pc++; break; // add hl, hl
    case 0x3a: state->r_a = state->memory[*((uint16_t *)(&code[1]))]; state->r_pc += 3; break; // ld a, (**)
    case 0x3b: state->r_sp--; state->r_pc++; break; // dec sp
    case 0x3c: inc8(state, &state->r_a); break; // inc a
    case 0x3d: dec8(state, &state->r_a); break; // dec a
    case 0x3e: state->r_a = code[1]; state->r_pc += 2; break; // ld a, *
    case 0x3f: // ccf
      {
        state->flags.h = state->flags.c;
        state->flags.c = !state->flags.c;
        state->flags.n = 0;
        state->flags.f3 = (state->r_a & 0b0001000) >> 3;
        state->flags.f5 = (state->r_a & 0b0100000) >> 5;
        state->r_pc++;
      }
      break;  
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

    case 0x50: state->r_d = state->r_b; state->r_pc++; break;
    case 0x51: state->r_d = state->r_c; state->r_pc++; break;
    case 0x52: state->r_d = state->r_d; state->r_pc++; break;
    case 0x53: state->r_d = state->r_e; state->r_pc++; break;
    case 0x54: state->r_d = state->r_h; state->r_pc++; break; // ld d, h
    case 0x55: state->r_d = state->r_l; state->r_pc++; break;
    case 0x56: state->r_d = state->memory[state->r_hl]; state->r_pc++; break;
    case 0x57: state->r_d = state->r_a; state->r_pc++; break;
    case 0x58: state->r_e = state->r_b; state->r_pc++; break;  // ld e, l
    case 0x59: state->r_e = state->r_c; state->r_pc++; break;  // ld e, l
    case 0x5a: state->r_e = state->r_d; state->r_pc++; break;  // ld e, l
    case 0x5b: state->r_e = state->r_e; state->r_pc++; break;  // ld e, l
    case 0x5c: state->r_e = state->r_h; state->r_pc++; break;  // ld e, l
    case 0x5d: state->r_e = state->r_l; state->r_pc++; break;  // ld e, l
    case 0x5e: state->r_e = state->memory[state->r_hl]; state->r_pc++; break; // ld e, (hl)
    case 0x5f: state->r_e = state->r_a; state->r_pc++; break; // ld e, a

     case 0x60: state->r_h = state->r_b; state->r_pc++; break; // ld h, l
    case 0x61: state->r_h = state->r_c; state->r_pc++; break; // ld h, l
    case 0x62: state->r_h = state->r_d; state->r_pc++; break; // ld h, l
    case 0x63: state->r_h = state->r_e; state->r_pc++; break; // ld h, l
    case 0x64: state->r_h = state->r_h; state->r_pc++; break; // ld h, l
    case 0x65: state->r_h = state->r_l; state->r_pc++; break; // ld h, l
    case 0x66: state->r_h = state->memory[state->r_hl]; state->r_pc++; break; // ld h, (hl)
     case 0x67: state->r_h = state->r_a; state->r_pc++; break;
     case 0x68: state->r_l = state->r_b; state->r_pc++; break; // ld l, a
     case 0x69: state->r_l = state->r_c; state->r_pc++; break; // ld l, a
     case 0x6a: state->r_l = state->r_d; state->r_pc++; break; // ld l, a
     case 0x6b: state->r_l = state->r_e; state->r_pc++; break; // ld l, a
     case 0x6c: state->r_l = state->r_h; state->r_pc++; break; // ld l, a
     case 0x6d: state->r_l = state->r_l; state->r_pc++; break; // ld l, a
     case 0x6e: state->r_l = state->memory[state->r_hl]; state->r_pc++; break; // ld l, a
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
    case 0x81: add8(state, state->r_c); break;
    case 0x82: add8(state, state->r_d); break;
    case 0x83: add8(state, state->r_e); break;
    case 0x84: add8(state, state->r_h); break;
    case 0x85: add8(state, state->r_l); break;
    case 0x86: add8(state, state->memory[state->r_hl]); break;
    case 0x87: add8(state, state->r_a); break;

    case 0x88: adc8(state, state->r_b); break;
    case 0x89: adc8(state, state->r_c); break;
    case 0x8a: adc8(state, state->r_d); break;
    case 0x8b: adc8(state, state->r_e); break;
    case 0x8c: adc8(state, state->r_h); break;
    case 0x8d: adc8(state, state->r_l); break;
    case 0x8e: adc8(state, state->memory[state->r_hl]); break;
    case 0x8f: adc8(state, state->r_a); break;

    case 0x90: sub8(state, state->r_b); break;
    case 0x91: sub8(state, state->r_c); break;
    case 0x92: sub8(state, state->r_d); break;
    case 0x93: sub8(state, state->r_e); break;
    case 0x94: sub8(state, state->r_h); break;
    case 0x95: sub8(state, state->r_l); break;
    case 0x96: sub8(state, state->memory[state->r_hl]); break;
    case 0x97: sub8(state, state->r_a); break;
    case 0x98: sbc8(state, state->r_b); break;
    case 0x99: sbc8(state, state->r_c); break;
    case 0x9a: sbc8(state, state->r_d); break;
    case 0x9b: sbc8(state, state->r_e); break;
    case 0x9c: sbc8(state, state->r_h); break;
    case 0x9d: sbc8(state, state->r_l); break;
    case 0x9e: sbc8(state, state->memory[state->r_hl]); break;
    case 0x9f: sbc8(state, state->r_a); break;
      
      
    case 0xa0: and8(state, &state->r_b); break; // and b
    case 0xa1: and8(state, &state->r_c); break; // and c
    case 0xa2: and8(state, &state->r_d); break; // and c
    case 0xa3: and8(state, &state->r_e); break; // and c
    case 0xa4: and8(state, &state->r_h); break; // and c
    case 0xa5: and8(state, &state->r_l); break; // and c
    case 0xa6: and8(state, &state->memory[state->r_hl]); break; // and c
    case 0xa7: and8(state, &state->r_a); break; // and c
    case 0xa8: xor8(state, state->r_b); break; // xor b
    case 0xa9: xor8(state, state->r_c); break; // xor c
    case 0xaa: xor8(state, state->r_d); break; // xor d
    case 0xab: xor8(state, state->r_e); break; // xor e
    case 0xac: xor8(state, state->r_h); break; // xor h
    case 0xad: xor8(state, state->r_l); break; // xor l
    case 0xae: xor8(state, state->memory[state->r_hl]); break; // xor (hl)
    case 0xaf: xor8(state, state->r_a); break; // xor a

    case 0xb0: or8(state, &state->r_b); break; // or a
    case 0xb1: or8(state, &state->r_c); break; // or a
    case 0xb2: or8(state, &state->r_d); break; // or a
    case 0xb3: or8(state, &state->r_e); break; // or a
    case 0xb4: or8(state, &state->r_h); break; // or a
    case 0xb5: or8(state, &state->r_l); break; // or a
    case 0xb6: or8(state, &state->memory[state->r_hl]); break;// or (hl)
    case 0xb7: or8(state, &state->r_a); break; // or a
    case 0xb8: cp8(state, state->r_b); break; // cp (hl)
    case 0xb9: cp8(state, state->r_c); break; // cp (hl)
    case 0xba: cp8(state, state->r_d); break; // cp (hl)
    case 0xbb: cp8(state, state->r_e); break; // cp (hl)
    case 0xbc: cp8(state, state->r_h); break; // cp (hl)
    case 0xbd: cp8(state, state->r_l); break; // cp (hl)
    case 0xbe: cp8(state, state->memory[state->r_hl]); break; // cp (hl)
    case 0xbf: cp8(state, state->r_a); break; // cp (hl)

     case 0xc0: ret(state, COND_NZ); break; // ret nz
    case 0xc1: state->r_bc = stack_get16(state); state->r_pc++; break; // pop bc
     case 0xc2: jp(state, COND_NZ, *((uint16_t *)(&code[1]))); break; // jp nz, **
    case 0xc3: state->r_pc = *((uint16_t *)(&code[1])); break; // jp **
     case 0xc4: call(state, COND_NZ, *((uint16_t *)(&code[1]))); break; // call nz, **
     case 0xc5: stack_put16(state, state->r_bc); state->r_pc++; break;// push bc
    case 0xc6: add8(state, code[1]); state->r_pc++; break;
     case 0xc8: ret(state, COND_Z); break; // ret z
    case 0xc9: state->r_pc = stack_get16(state); break; // ret
    case 0xca: jp(state, COND_Z, *((uint16_t *)(&code[1]))); break; // jp z, **
    case 0xcb: emulate_op_cb(state); break;
     case 0xcc: call(state, COND_Z, *((uint16_t *)(&code[1]))); break; // call z, **
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
    case 0xce: adc8(state, code[1]); state->r_pc++; break; // adc a, *
     case 0xd0: ret(state, COND_NC); break; // ret nc
     case 0xd1: state->r_de = stack_get16(state); state->r_pc++; break; // pop de
     case 0xd2: jp(state, COND_NC, *((uint16_t *)(&code[1]))); break; // jp nc, **
     case 0xd4: call(state, COND_NC, *((uint16_t *)(&code[1]))); break; // call nc, **
     case 0xd5: stack_put16(state, state->r_de); state->r_pc++; break;// push de
    case 0xd6: sub8(state, code[1]); state->r_pc++; break;
     case 0xd8: ret(state, COND_C); break; // ret c
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
     case 0xda: jp(state, COND_C, *((uint16_t *)(&code[1]))); break; // jp c, **
     case 0xdc: call(state, COND_C, *((uint16_t *)(&code[1]))); break; // call c, **
     case 0xdd: emulate_op_dd(state); break;
     case 0xde: sbc8(state, code[1]); state->r_pc++; break; // sbc a, *
     case 0xe0: ret(state, state->flags.pv != 1); break; // ret po
     case 0xe1: state->r_hl = stack_get16(state); state->r_pc++; break; // pop hl
     case 0xe2: jp(state, COND_PO, *((uint16_t *)(&code[1]))); break; // jp po, **
     case 0xe4: call(state, COND_PO, *((uint16_t *)(&code[1]))); break; // call po, **
     case 0xe5: stack_put16(state, state->r_hl); state->r_pc++; break; // push hl
     case 0xe6: and8(state, &code[1]); state->r_pc++; break; // and *
     case 0xe8: ret(state, COND_PE); break; // ret pe
    case 0xe9: state->r_pc = state->r_hl; break; // jp (hl) ????
     case 0xea: jp(state, COND_PE, *((uint16_t *)(&code[1]))); break; // jp pe, **
    case 0xeb: //ex de, hl
        {
            uint16_t temp = state->r_de;
            state->r_de = state->r_hl;
            state->r_hl = temp;
            state->r_pc++;
        }
        break;
     case 0xec: call(state, COND_PE, *((uint16_t *)(&code[1]))); break; // call pe, **
    case 0xed: emulate_op_ed(state); break; // extended op
    case 0xee: xor8(state, code[1]); state->r_pc++; break; // xor *
     case 0xf0: ret(state, COND_P); break; // ret p
    case 0xf1: state->r_af = stack_get16(state); state->r_pc++; break; // pop af
     case 0xf2: jp(state, COND_P, *((uint16_t *)(&code[1]))); break; // jp p, **
    case 0xf3: /* printf("DI not implemented. Skipped\n"); */ state->r_pc++; break;
     case 0xf4: call(state, COND_P, *((uint16_t *)(&code[1]))); break; // call p, **
     case 0xf5: stack_put16(state, state->r_af); state->r_pc++; break; // push af
    case 0xf6: or8(state, &code[1]); state->r_pc++; break; // or *
     case 0xf8: ret(state, COND_M); break; // ret m
    case 0xf9: state->r_sp = state->r_hl; state->r_pc++; break; // ld sp, hl
     case 0xfa: jp(state, COND_M, *((uint16_t *)(&code[1]))); break; // jp m, **
    case 0xfb: state->r_pc++; break; // ei // TODO
     case 0xfc: call(state, COND_M, *((uint16_t *)(&code[1]))); break; // call m, **
    case 0xfd: emulate_op_fd(state); break;
    case 0xfe: cp8(state, code[1]); state->r_pc++; break; // cp *
    default: emul_unimplemented_op(*code); return 1; break;
    }

    // print_state(state);
    if (state->r_pc == 0x0000) {
        printf("\nREACHED WBOOT - NORMAL EXIT");
        print_state(state);
        return 1;
    }

    
    return 0;
};
