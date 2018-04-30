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
    printf("\t==PC:%04x SP:%04x A:%02x F:%02x B:%02x C:%02x D:%02x E:%02x H:%02x L:%02x\n",
           state->register_pc, state->register_sp, state->register_a, state->register_f, state->register_b, state->register_c,
           state->register_d, state->register_e, state->register_h, state->register_l);
    // printf("\t==PCH:%02x PCL:%02x\n", state->register_pch, state->register_pcl);
    printf("\t == C: %02x\n", state->flags.c);
    /* printf("\t==Z:%01x S:%01x P:%01x Cy:%01x Ac:%01x\n", */
    /*        state->cc.z, state->cc.s, state->cc.p, state->cc.cy, state->cc.ac); */
    print_stack(state);
}

void add8(state_t *state, uint8_t data) {
            if (((state->register_a ^ data) & 0x80) == 0 
                && ((1 ^ (state->register_a + data)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0;
            if ((state->register_a + data) > 0xffff)
                state->flags.c = 1;
            else
                state->flags.c = 0;
            state->flags.h = (((state->register_a & 0xf) + (data & 0xf)) & 0x10) > 1;
            state->register_a += data;
            state->flags.n = 0;
            state->flags.z = (state->register_a == 0);
            state->flags.s = ((state->register_a & 0x80) == 0x80);
            state->register_pc += 1;
}

void add16(state_t *state, uint16_t *lft, uint16_t *rgt) {
            uint32_t res = *lft + *rgt;
            state->flags.h = (((*lft & 0xf) + (*rgt & 0xf)) & 0x10) > 1;
            *lft = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->register_pc += 1;
}

void and8(state_t *state, uint8_t *r) {
    state->register_a = state->register_a & *r;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->register_a);
    state->flags.h = 1;
    state->flags.z = (state->register_a == 0);
    state->flags.s = ((state->register_a & 0x80) == 0x80);
    state->register_pc++;
}

void cp8(state_t *state, uint8_t data) {
    uint8_t res = state->register_a - data;
    state->flags.c = (state->register_a < data);
    state->flags.n = 1;
    state->flags.h = (((res & 0xf) + (data & 0xf)) & 0x10) > 1;
    state->flags.z = (res == 0);
    state->flags.s = ((res & 0x80) == 0x80);
    if (((state->register_a ^ data) & 0x80) != 0 /* Not same sign */ 
        && ((data ^ res) & 0x80) == 0) /* Same sign */ 
        state->flags.pv = 1;
    else
        state->flags.pv = 0; 
    state->register_pc += 1;
}

void or8(state_t *state, uint8_t *r) {
    state->register_a = state->register_a | *r;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->register_a);
    state->flags.h = 0;
    state->flags.z = (state->register_a == 0);
    state->flags.s = ((state->register_a & 0x80) == 0x80);
    state->register_pc += 1;
}

void xor8(state_t *state, uint8_t data) {
    state->register_a = state->register_a ^ data;
    state->flags.c = 0;
    state->flags.n = 0;
    state->flags.pv = parity8(state->register_a);
    state->flags.h = 0;
    state->flags.z = (state->register_a == 0);
    state->flags.s = ((state->register_a & 0x80) == 0x80);
    state->register_pc++;
}

void stack_put16(state_t *state, uint16_t data) {
    state->memory[(uint16_t)(--state->register_sp)] = (data >> 8) & 0xff;
    state->memory[(uint16_t)(--state->register_sp)] = data & 0xff;
}

uint16_t stack_get16(state_t *state) {
    uint16_t res = state->memory[state->register_sp++] | (state->memory[state->register_sp++] << 8);
    return res;
}

void emul_unimplemented_op(uint8_t code) {
    printf("\nemul: unimplemented instruction: %02x\n", code);
    exit(EXIT_FAILURE);
}

void emulate_op_ed(state_t * state) {
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x42: // sbc hl, bc
        {
            uint32_t res = state->register_hl - state->register_bc - state->flags.c;
            state->register_hl = (uint16_t)(res & 0xffff);
            state->flags.n = 1;
            if (((state->register_hl ^ (state->register_bc + state->flags.c)) & 0x80) != 0 /* Not same sign */ 
                && ((1 ^ (state->register_hl - (state->register_bc + state->flags.c))) & 0x80) == 0) /* Same sign */ 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->flags.c = (1 == ((res >> 16) & 0x1));
            state->flags.h = (((state->register_hl & 0xf) - ((state->register_bc + 1) & 0xf)) & 0x10) > 1; // TODO
            state->flags.z = (state->register_hl == 0);
            state->flags.s = ((state->register_hl & 0x80) == 0x80);
            state->register_pc += 2;
        }
        break;
    case 0x73: // ld (**), sp
        { 
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->memory[addr] = state->register_spl;
            state->memory[addr+1] = state->register_sph;
            state->register_pc += 4;
        }
        break;
    case 0x7b: // ld sp, (**)
        {
            uint16_t addr = *((uint16_t *)(&code[2]));
            state->register_spl = state->memory[addr];
            state->register_sph = state->memory[addr+1];
            state->register_pc += 4;
        }
        break;
    case 0xb0: // ldir
        {
            do
            {
              state->memory[state->register_de] = state->memory[state->register_hl];
              state->register_de++;
              state->register_hl++;
              state->register_bc--;
            } while (state->register_bc > 0);
            state->flags.n = 0;
            state->flags.pv = 0;
            state->flags.h = 0;
            state->register_pc += 2;
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
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x09: // add iy, bc
        {
            uint32_t res = state->register_iy + state->register_bc;
            state->flags.h = (((state->register_iy & 0xf) + (state->register_bc & 0xf)) & 0x10) > 1;
            state->register_iy = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->register_pc += 2;
        }
        break;
    case 0xe1: state->register_iy = stack_get16(state); state->register_pc += 2; break;
    case 0xe5: stack_put16(state, state->register_iy); state->register_pc += 2; break;
    default:
        {
            printf("\nemul: unimplemented instruction: %02x %02x\n", code[0], code[1]);
            exit(EXIT_FAILURE);
        }
        break;
    }
}

void emulate_op_cb(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x40: // bit 0, b
        {
            state->flags.n = 0;
            state->flags.h = 1;
            state->flags.z = (1 == (state->register_b & 0x1));
            state->register_pc += 2;
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
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[3]) {
    case 0x46: // bit 0, (ix + *)
        {
            uint8_t addr = state->register_ix + code[2];
            uint8_t res = state->memory[addr];
            state->flags.z = (1 == (res & 0x1));
            state->flags.n = 0;
            state->flags.h = 1;
            state->register_pc += 4;
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
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x09: // add ix, bc
        {
            uint32_t res = state->register_ix + state->register_bc;
            state->flags.h = (((state->register_ix & 0xf) + (state->register_bc & 0xf)) & 0x10) > 1;
            state->register_ix = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->register_pc += 2;
        }
        break;
    case 0x84: add8(state, state->register_ixh); state->register_pc++; break; // add ixh
    case 0x86: add8(state, state->memory[(uint16_t)(state->register_ix + code[2])]); state->register_pc += 2; break; // add (ix + *)
    case 0xcb: emulate_op_dd_cb(state); break; // IX bit instructions
    case 0xe1: state->register_ix = stack_get16(state); state->register_pc += 2; break;
    case 0xe5: stack_put16(state, state->register_ix); state->register_pc += 2; break;
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
    state->register_pc += 1;
}

int emulate_op(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];

    if (state->register_pc == 0) {
        printf("REACHED WBOOT - NORMAL EXIT");
        print_state(state);
        exit(EXIT_SUCCESS);
    }

    // disassemble_op(state);

    switch(*code) {
    case 0x00: state->register_pc++; break; // nop
    case 0x01: // ld bc, **
        {
            state->register_bc = *((uint16_t *)(&code[1]));
            state->register_pc += 3;
        }
        break;
    case 0x05: dec8(state, &state->register_b); break; // dec b
    case 0x06: // ld b, *
        {
            state->register_b = state->memory[state->register_pc + 1];
            state->register_pc += 2;
        }
        break;
    case 0x07: // rlca
        {
            uint16_t res = (state->register_a << 1) | (state->register_a >> 7);
            state->register_a = res & 0xff;
            state->flags.n = 0;
            state->flags.c = res >> 8;
            state->flags.h = 0;
            state->register_pc++;
        }
        break;
    case 0x08: // ex af, af'
      {
        uint16_t tmp = state->register_af;
        state->register_af = state->_register_af;
        state->_register_af = tmp;
        state->register_pc++;
      }
      break;
    case 0x09: // add hl, bc
        {
            uint32_t res = state->register_hl + state->register_bc;
            state->flags.h = (((state->register_hl & 0xf) + (state->register_bc & 0xf)) & 0x10) > 1;
            state->register_hl = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->register_pc += 1;
        }
        break;
    case 0x0b: // dec bc
        {
            state->register_bc--;
            state->register_pc++;
        }
        break;
    case 0x0d: dec8(state, &state->register_c); break; // dec b
    case 0x0e: // ld c, *
        {
            state->register_c = state->memory[state->register_pc + 1];
            state->register_pc += 2;
        }
        break;
    case 0x0f: // rrca
        {
            uint8_t x = state->register_a;
            state->register_a = ((x & 1) << 7) | (x >> 1);
            state->flags.h = 0;
            state->flags.n = 0;
            state->flags.c = (1 == (x & 0x1));
            state->register_pc++;
        }
        break;
    case 0x11: // ld de, **
        {
            state->register_de = *((uint16_t *)(&code[1]));
            state->register_pc += 3;
        }
        break;
    case 0x13: // inc de
        {
            state->register_de++;
            state->register_pc++;
        }
        break;
    case 0x14: // inc b
        {
            if (((state->register_b ^ 1) & 0x80) == 0 
                && ((1 ^ (state->register_b + 1)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->register_b++;
            state->flags.n = 0;
            state->flags.h = (((state->register_b & 0xf) + (1 & 0xf)) & 0x10) > 1;
            state->flags.z = (state->register_b == 0);
            state->flags.s = ((state->register_b & 0x80) == 0x80);
            state->register_pc += 1;
        }
        break;
    case 0x16: // ld d, *
        {
            state->register_d = state->memory[state->register_pc + 1];
            state->register_pc += 2;
        }
        break;
    case 0x19: // add hl, de
        {
            uint32_t res = state->register_hl + state->register_de;
            state->flags.h = (((state->register_hl & 0xf) + (state->register_de & 0xf)) & 0x10) > 1;
            state->register_hl = res & 0xffff;
            state->flags.c = (res >> 16) & 0x1;
            state->flags.n = 0;
            state->register_pc += 1;
        }
        break;
    case 0x1a: state->register_a = state->memory[state->register_de]; state->register_pc++; break; // ld a, (de)
    case 0x21: // ld hl, **
        {
            state->register_hl = *((uint16_t *)(&code[1]));
            state->register_pc += 3;
        }
        break;
    case 0x22: state->memory[*((uint16_t *)(&code[1]))] = state->register_hl; state->register_pc += 3; break; // ld (**), hl
    case 0x23: // inc hl
        {
            state->register_hl += 1;
            state->register_pc += 1;
        }
        break;
    case 0x26: state->register_h = code[1]; state->register_pc += 2; break; // ld h, *
    case 0x29: // add hl, hl
        {
            uint32_t res = state->register_hl + state->register_hl;
            state->flags.c = (res >> 16) & 0x1;;
            state->flags.n = 0;
            state->flags.h = (((state->register_hl & 0xf) + (state->register_hl & 0xf)) & 0x10) > 1;;
            state->register_hl = (uint16_t)(res & 0xffff);
            state->register_pc++;
        }
        break;
    case 0x2a: // ld hl, (**)
        {
            uint8_t addr = *((uint16_t *)(&code[1]));
            state->register_l = state->memory[addr];
            state->register_h = state->memory[addr + 1];
            state->register_pc += 3;
        }
        break;
    case 0x2b: // dec hl
        {
            state->register_hl--;
            state->register_pc++;
        }
        break;
    case 0x31: state->register_sp = *((uint16_t *)(&code[1])); state->register_pc += 3; break; // ld sp, **
    case 0x32: // ld (**), a
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
            state->memory[addr] = state->register_a;
            state->register_pc += 3;
        }
        break;
    case 0x34: // inc (hl)
        {
            if (((state->memory[state->register_hl] ^ 1) & 0x80) == 0 
                && ((1 ^ (state->memory[state->register_hl] + 1)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->memory[state->register_hl]++;
            state->flags.n = 0;
            state->flags.h = (((state->memory[state->register_hl] & 0xf) + (1 & 0xf)) & 0x10) > 1;
            state->flags.z = (state->memory[state->register_hl] == 0);
            state->flags.s = ((state->memory[state->register_hl] & 0x80) == 0x80);
            state->register_pc += 1;
        }
        break;
    case 0x36: // ld (hl), *
        {
            state->memory[state->register_hl] = code[1];
            state->register_pc += 2;
        }
        break;
    case 0x3a: // ld a, (**)
        {
            state->register_a = state->memory[*((uint16_t *)(&code[1]))];
            state->register_pc += 3;
        }
        break;
    case 0x3c: // inc a
        {
            if (((state->register_a ^ 1) & 0x80) == 0 
                && ((1 ^ (state->register_a + 1)) & 0x80) != 0) 
                state->flags.pv = 1;
            else
                state->flags.pv = 0; 
            state->register_a++;
            state->flags.n = 0;
            state->flags.h = (((state->register_a & 0xf) + (1 & 0xf)) & 0x10) > 1;
            state->flags.z = (state->register_a == 0);
            state->flags.s = ((state->register_a & 0x80) == 0x80);
            state->register_pc += 1;
        }
        break;
    case 0x3e: state->register_a = code[1]; state->register_pc += 2; break; // ld a, *
        
    case 0x40: state->register_b = state->register_b; state->register_pc++; break; // ld b, b
    case 0x41: state->register_b = state->register_c; state->register_pc++; break; // ld b, c
    case 0x42: state->register_b = state->register_d; state->register_pc++; break; // ld b, d
    case 0x43: state->register_b = state->register_e; state->register_pc++; break; // ld b, e
    case 0x44: state->register_b = state->register_h; state->register_pc++; break; // ld b, h
    case 0x45: state->register_b = state->register_l; state->register_pc++; break; // ld b, l
    case 0x46: state->register_b = state->memory[state->register_hl]; state->register_pc++; break; // ld b, (hl)
    case 0x47: state->register_b = state->register_a; state->register_pc++; break; // ld b, a
    case 0x48: state->register_c = state->register_b; state->register_pc++; break; // ld c, b
    case 0x49: state->register_c = state->register_c; state->register_pc++; break; // ld c, c
    case 0x4a: state->register_c = state->register_d; state->register_pc++; break; // ld c, d
    case 0x4b: state->register_c = state->register_e; state->register_pc++; break; // ld c, e
    case 0x4c: state->register_c = state->register_h; state->register_pc++; break; // ld c, h
    case 0x4d: state->register_c = state->register_l; state->register_pc++; break; // ld c, l
    case 0x4e: state->register_c = state->memory[state->register_hl]; state->register_pc++; break; // ld c, (hl)
    case 0x4f: state->register_c = state->register_a; state->register_pc++; break; // ld c, a

    case 0x54: // ld d, h
        {
            state->register_d = state->register_h;
            state->register_pc += 1;
        }
        break;
    case 0x5d: // ld e, l
        {
            state->register_e = state->register_l;
            state->register_pc += 1;
        }
        break;
    case 0x5e: // ld e, (hl)
        {
            state->register_e = state->memory[state->register_hl];
            state->register_pc += 1;
        }
        break;
    case 0x5f: state->register_e = state->register_a; state->register_pc++; break; // ld e, a
    case 0x66: // ld h, (hl)
        {
            state->register_h = state->memory[state->register_hl];
            state->register_pc += 1;
        }
        break;
    case 0x6f: state->register_l = state->register_a; state->register_pc++; break; // ld l, a

    case 0x77: state->memory[state->register_hl] = state->register_a; state->register_pc++; break; // ld (hl), a
    case 0x78: state->register_a = state->register_b; state->register_pc++; break; // ld a, b
    case 0x79: state->register_a = state->register_c; state->register_pc++; break; // ld ld a, c
    case 0x7a: state->register_a = state->register_d; state->register_pc++; break; // ld ld a, d
    case 0x7b: state->register_a = state->register_e; state->register_pc++; break; // ld a, e
    case 0x7c: state->register_a = state->register_h; state->register_pc++; break; // ld a, h
    case 0x7d: state->register_a = state->register_l; state->register_pc++; break; // ld a, l
    case 0x7e: state->register_a = state->memory[state->register_hl]; state->register_pc++; break; // ld a, (hl)

    case 0x80: add8(state, state->register_b); break;

    case 0xa0: and8(state, &state->register_b); break; // and b
    case 0xa1: and8(state, &state->register_c); break; // and c
    case 0xa8: xor8(state, state->register_b); break; // xor b
    case 0xae: xor8(state, state->memory[state->register_hl]); break; // xor (hl)
    case 0xb6: or8(state, &state->memory[state->register_hl]); break;// or (hl)
    case 0xb7: or8(state, &state->register_a); break; // or a
    case 0xbe: cp8(state, state->memory[state->register_hl]); break; // cp (hl)

    case 0xc1: state->register_bc = stack_get16(state); state->register_pc += 1; break; // pop bc
    case 0xc2: // jp nz, **
        {
            if (state->flags.z != 1)
                state->register_pc = *((uint16_t *)(&code[1]));
            else
                state->register_pc += 3;
        }
        break;
    case 0xc3: state->register_pc = *((uint16_t *)(&code[1])); break; // jp **
    case 0xc4: // call nz, **
        {
            state->register_pc += 3;
            if (state->flags.z != 1) {
                stack_put16(state, state->register_pc);
                state->register_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xc5: // push bc
        {
            stack_put16(state, state->register_bc);
            state->register_pc += 1;
        }
        break;
    case 0xc6: add8(state, code[1]); state->register_pc++; break; // add a, *
    case 0xc9: // ret
        {
            state->register_pc = stack_get16(state);
        }
        break;
    case 0xca: // jp z, **
        {
            if (state->flags.z == 1) {
                state->register_pc = *((uint16_t *)(&code[1]));
            } else {
                state->register_pc += 3;
            }
        }
        break;
    case 0xcb: emulate_op_cb(state); break;
    case 0xcd: // call **
        {
            uint16_t addr = *((uint16_t *)(&code[1]));
            state->register_pc += 3;
            if (5 == addr) {
                cpm_bdos(state);
            } else if (0 == addr) {
                exit(EXIT_SUCCESS);
            } else {
                stack_put16(state, state->register_pc);
                state->register_pc = addr;
            }
        }
        break;
    case 0xd0: // ret nc
      {
        if (state->flags.c != 1)
          state->register_pc = stack_get16(state);
        else
          state->register_pc++;
      }
      break;
    case 0xd1: // pop de
        {
            state->register_de = stack_get16(state);
            state->register_pc += 1;
        }
        break;
    case 0xd2: // jp nc, **
        {
            if (state->flags.c != 1) {
                state->register_pc = *((uint16_t *)(&code[1]));
            } else {
                state->register_pc += 3;
            }
        }
        break;
    case 0xd4: // call nc, **
        {
            state->register_pc += 3;
            if (state->flags.c != 1) {
                stack_put16(state, state->register_pc);
                state->register_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xd5: // push de
        {
            stack_put16(state, state->register_de);
            state->register_pc += 1;
        }
        break;
    case 0xd8: // ret c
      {
        if (state->flags.c == 1)
          state->register_pc = stack_get16(state);
        else
          state->register_pc++;
      }
      break;
    case 0xd9: // exx
      {
        uint16_t tmp;
        tmp = state->register_bc;
        state->register_bc = state->_register_bc;
        state->_register_bc = tmp;
        tmp = state->register_de;
        state->register_de = state->_register_de;
        state->_register_de = tmp;
        tmp = state->register_hl;
        state->register_hl = state->_register_hl;
        state->_register_hl = tmp;
        state->register_pc++;
      }
      break;
    case 0xda: // jp c, **
        {
            if (state->flags.c == 1) {
                state->register_pc = *((uint16_t *)(&code[1]));
            } else {
                state->register_pc += 3;
            }
        }
        break;
    case 0xdc: // call c, **
        {
            state->register_pc += 3;
            if (state->flags.c == 1) {
                stack_put16(state, state->register_pc);
                state->register_pc = *((uint16_t *)(&code[1]));
            }
        }
        break;
    case 0xdd: emulate_op_dd(state); break;
        
    case 0xe1: // pop hl
        {
            state->register_hl = stack_get16(state);
            state->register_pc += 1;
        }
        break;
    case 0xe5: // push hl
        {
            stack_put16(state, state->register_hl);
            state->register_pc += 1;
        }
        break;
    case 0xe6: // and *
        {
            state->register_a &= code[1];
            state->flags.c = 0;
            state->flags.n = 0;
            state->flags.h = 1;
            state->flags.pv = parity8(state->register_a);
            state->flags.z = (state->register_a == 0);
            state->flags.s = ((state->register_a & 0x80) == 0x80);
            state->register_pc += 2;
        }
        break;
    case 0xeb: //ex de, hl
        {
            uint16_t temp = state->register_de;
            state->register_de = state->register_hl;
            state->register_hl = temp;
            state->register_pc += 1;
        }
        break;
    case 0xed: // extended op
        {
            emulate_op_ed(state);
        }
        break;
    case 0xf1: // pop af
        {
            state->register_af = stack_get16(state);
            state->register_pc += 1;
        }
        break;
    case 0xf3: /* printf("DI not implemented. Skipped\n"); */ state->register_pc++; break;
    case 0xf5: // push af
        {
            stack_put16(state, state->register_af);
            state->register_pc += 1;
        }
        break;
    case 0xf9: // ld sp, hl
        {
            state->register_sp = state->register_hl;
            state->register_pc += 1;
        }
        break;
    case 0xfb: state->register_pc++; break; // ei // TODO
    case 0xfd: emulate_op_fd(state); break;
    case 0xfe: cp8(state, code[1]); state->register_pc++; break; // cp *
    default: emul_unimplemented_op(*code); break;
    }

    // print_state(state);

    
    return 0;
};
