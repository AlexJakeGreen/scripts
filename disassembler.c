#include <stdio.h>
#include "disassembler.h"

void unimplemented_op(uint8_t code) {
    printf("disassm: unimplemented instruction: %02x\n", code);
    exit(EXIT_FAILURE);
}

void decode_ed(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x42: printf("42 sbc hl, bc"); break;
    case 0x73: printf("73 ld (%04x), sp", *((uint16_t *)(&code[2]))); break;
    case 0x7b: printf("7b ld sp, (%04x)", *((uint16_t *)(&code[2]))); break;
    case 0xa0: printf("a0 ldi"); break;
    case 0xb0: printf("b0 ldir"); break;
    default: printf("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void decode_fd(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x09: printf("add iy, bc"); break;
    case 0xe1: printf("pop iy"); break;
    case 0xe5: printf("push iy"); break;
    default: printf("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void decode_dd(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];
    switch(code[1]) {
    case 0x09: printf("add ix, bc"); break;
    case 0xe1: printf("pop ix"); break;
    case 0xe5: printf("push ix"); break;
    default: printf("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void disassemble_op(state_t *state) {
    uint8_t *code = &state->memory[state->register_pc];
    printf("%04x %02x  ", state->register_pc, code[0]);

    switch (*code) {

    case 0x00: printf("NOP"); break;
    case 0x01: printf("ld bc, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x02: printf("ld (bc), a"); break;
    case 0x03: printf("inc bc"); break;
    case 0x04: printf("inc b"); break;
    case 0x05: printf("dec b"); break;
    case 0x06: printf("ld b, %02x", code[1]); break;
    case 0x07: printf("rlca"); break;
    case 0x08: printf("ex af, af'"); break;
    case 0x09: printf("add hl, bc"); break;
    case 0x0a: printf("ld a, (bc)"); break;
    case 0x0b: printf("dec bc"); break;
    case 0x0c: printf("inc c"); break;
    case 0x0d: printf("dec c"); break;
    case 0x0e: printf("ld c, %02x", code[1]); break;
    case 0x0f: printf("rrca"); break;

    case 0x11: printf("ld de, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x13: printf("inc de"); break;
    case 0x14: printf("inc d"); break;
    case 0x16: printf("ld d, %02x", code[1]); break;
    case 0x19: printf("add hl, de"); break;
    case 0x1a: printf("ld a, (de)"); break;
    case 0x1b: printf("dec de"); break;
    case 0x1c: printf("inc e"); break;
    case 0x1d: printf("dec e"); break;
    case 0x1e: printf("ld e, %02x", code[1]); break;
    case 0x1f: printf("rra"); break;
        
    case 0x20: printf("jr nz, %0x2", code[1]); break;
    case 0x21: printf("ld hl, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x22: printf("ld (%04x), hl", *((uint16_t *)(&code[1]))); break;
    case 0x23: printf("inc hl"); break;
    case 0x26: printf("ld h, %02x", code[1]); break;
    case 0x29: printf("add hl, hl"); break;
    case 0x2b: printf("dec hl"); break;
    case 0x2a: printf("ld hl, (%04x)", *((uint16_t *)(&code[1]))); break;

    case 0x30: printf("jr nc, %02x", code[1]); break;
    case 0x31: printf("ld sp, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x32: printf("ld (%04x), a", *((uint16_t *)(&code[1]))); break;
    case 0x34: printf("inc (hl)"); break;
    case 0x36: printf("ld (hl), %02x", code[1]); break;
    case 0x3a: printf("ld a, (%04x)", *((uint16_t *)(&code[1]))); break;
    case 0x3c: printf("inc a"); break;
    case 0x3d: printf("dec a"); break;
    case 0x3e: printf("ld a, %02x", code[1]); break;
        
    case 0x40: printf("ld b, b"); break;
    case 0x41: printf("ld b, c"); break;
    case 0x42: printf("ld b, d"); break;
    case 0x43: printf("ld b, e"); break;
    case 0x44: printf("ld b, h"); break;
    case 0x45: printf("ld b, l"); break;
    case 0x46: printf("ld b, (hl)"); break;
    case 0x47: printf("ld b, a"); break;
    case 0x48: printf("ld c, b"); break;
    case 0x49: printf("ld c, c"); break;
    case 0x4a: printf("ld c, d"); break;
    case 0x4b: printf("ld c, e"); break;
    case 0x4c: printf("ld c, h"); break;
    case 0x4d: printf("ld c, l"); break;
    case 0x4e: printf("ld c, (hl)"); break;
    case 0x4f: printf("ld c, a"); break;

    case 0x50: printf("ld d, b"); break;
    case 0x51: printf("ld d, c"); break;
    case 0x52: printf("ld d, d"); break;
    case 0x53: printf("ld d, e"); break;
    case 0x54: printf("ld d, h"); break;
    case 0x55: printf("ld d, l"); break;
    case 0x56: printf("ld d, (hl)"); break;
    case 0x57: printf("ld d, a"); break;
    case 0x58: printf("ld e, b"); break;
    case 0x59: printf("ld e, c"); break;
    case 0x5a: printf("ld e, d"); break;
    case 0x5b: printf("ld e, e"); break;
    case 0x5c: printf("ld e, h"); break;
    case 0x5d: printf("ld e, l"); break;
    case 0x5e: printf("ld e, (hl)"); break;
    case 0x5f: printf("ld e, a"); break;

    case 0x66: printf("ld h,(hl)"); break;
    case 0x6f: printf("ld l, a"); break;

    case 0x70: printf("ld (hl), b"); break;
    case 0x71: printf("ld (hl), c"); break;
    case 0x72: printf("ld (hl), d"); break;
    case 0x73: printf("ld (hl), e"); break;
    case 0x74: printf("ld (hl), h"); break;
    case 0x75: printf("ld (hl), l"); break;
    case 0x76: printf("halt"); break;
    case 0x77: printf("ld (hl), a"); break;
    case 0x78: printf("ld a, b"); break;
    case 0x79: printf("ld a, c"); break;
    case 0x7a: printf("ld a, d"); break;
    case 0x7b: printf("ld a, e"); break;
    case 0x7c: printf("ld a, h"); break;
    case 0x7d: printf("ld a, l"); break;
    case 0x7e: printf("ld a, (hl)"); break;
    case 0x7f: printf("ld a, a"); break;

    case 0xa0: printf("and b"); break;
    case 0xa1: printf("and c"); break;
    case 0xa2: printf("and d"); break;
    case 0xa3: printf("and e"); break;
    case 0xa4: printf("and h"); break;
    case 0xa5: printf("and l"); break;
    case 0xa6: printf("and (hl)"); break;
    case 0xa7: printf("and a"); break;
    case 0xa8: printf("xor b"); break;
    case 0xa9: printf("xor c"); break;
    case 0xaa: printf("xor d"); break;
    case 0xab: printf("xor e"); break;
    case 0xac: printf("xor h"); break;
    case 0xad: printf("xor l"); break;
    case 0xae: printf("xor (hl)"); break;
    case 0xaf: printf("xor a"); break;

    case 0xb6: printf("or (hl)"); break;
    case 0xb7: printf("or a"); break;
    case 0xbe: printf("cp (hl)"); break;
        
    case 0xc1: printf("pop bc"); break;
    case 0xc2: printf("jp nz, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc3: printf("jp %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc4: printf("call %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc5: printf("push bc"); break;
    case 0xc6: printf("add a, %02x", code[1]); break;
    case 0xc9: printf("ret"); break;
    case 0xca: printf("jp z, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xcd: printf("call %04x", *((uint16_t *)(&code[1]))); break;

    case 0xd1: printf("pop de"); break;
    case 0xd5: printf("push de"); break;
    case 0xda: printf("jp c, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xdd: decode_dd(state); break;
        
    case 0xe1: printf("pop hl"); break;
    case 0xe5: printf("push hl"); break;
    case 0xe6: printf("and %02x", code[1]); break;
    case 0xeb: printf("ex de, hl"); break;
    case 0xed: decode_ed(state); break;

    case 0xf1: printf("pop af"); break;
    case 0xf3: printf("di"); break;
    case 0xf5: printf("push af"); break;
    case 0xf9: printf("ld sp, hl"); break;
    case 0xfb: printf("ei"); break;
    case 0xfd: decode_fd(state); break;
    case 0xfe: printf("cp %02x", code[1]); break;
    default: unimplemented_op(*code); break;
    };
    printf("\n");
}
