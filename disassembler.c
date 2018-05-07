#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "disassembler.h"
/* #include "log.h" */

void log_message(const char *format, ...) {
  va_list arg;
  char message[16 * 1024] = "";
  va_start(arg, format);
  vsprintf(message, format, arg);
  va_end(arg);
  /* print_log(message); */
  printf("%s", message);
}



void unimplemented_op(uint8_t code) {
    log_message("disassm: unimplemented instruction: %02x\n", code);
    exit(EXIT_FAILURE);
}

void decode_ed(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x42: log_message("42 sbc hl, bc"); break;
    case 0x4a: log_message("4a adc hl, bc"); break;
    case 0x52: log_message("52 sbc hl, de"); break;
    case 0x5a: log_message("5a adc hl, de"); break;
    case 0x62: log_message("62 sbc hl, hl"); break;
    case 0x6a: log_message("6a adc hl, hl"); break;
    case 0x72: log_message("72 sbc hl, sp"); break;
    case 0x73: log_message("73 ld (%04x), sp", *((uint16_t *)(&code[2]))); break;
    case 0x7a: log_message("7a adc hl, sp"); break;
    case 0x7b: log_message("7b ld sp, (%04x)", *((uint16_t *)(&code[2]))); break;
    case 0xa0: log_message("a0 ldi"); break;
    case 0xa9: log_message("cpd"); break;
    case 0xb0: log_message("b0 ldir"); break;
    default: log_message("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void decode_fd(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x09: log_message("add iy, bc"); break;
    case 0x21: log_message("ld iy, %04x", *((uint16_t *)(&code[2]))); break;
    case 0x23: log_message("inc iy"); break;
    case 0x7e: log_message("ld a, (iy + %02x)", code[2]); break;
    case 0xe1: log_message("pop iy"); break;
    case 0xe5: log_message("push iy"); break;
    case 0xe9: log_message("jp (iy)"); break;
    default: log_message("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void decode_cb(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x58: log_message("bit 3, b"); break;
    default: log_message("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }    
}

void decode_dd(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    switch(code[1]) {
    case 0x09: log_message("add ix, bc"); break;
    case 0x21: log_message("ld ix, %04x", *((uint16_t *)(&code[2]))); break;
    case 0x23: log_message("inc ix"); break;
    case 0x7e: log_message("ld a, (ix + %02x)", code[2]); break;
    case 0xe1: log_message("pop ix"); break;
    case 0xe5: log_message("push ix"); break;
    case 0xe9: log_message("jp (ix)"); break;
    default: log_message("disassm: unimplemented %02x %02x\n", code[0], code[1]); exit(EXIT_SUCCESS); break;
    }
}

void disassemble_op(state_t *state) {
    uint8_t *code = &state->memory[state->r_pc];
    log_message("%04x %02x  ", state->r_pc, code[0]);

    switch (*code) {

    case 0x00: log_message("NOP"); break;
    case 0x01: log_message("ld bc, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x02: log_message("ld (bc), a"); break;
    case 0x03: log_message("inc bc"); break;
    case 0x04: log_message("inc b"); break;
    case 0x05: log_message("dec b"); break;
    case 0x06: log_message("ld b, %02x", code[1]); break;
    case 0x07: log_message("rlca"); break;
    case 0x08: log_message("ex af, af'"); break;
    case 0x09: log_message("add hl, bc"); break;
    case 0x0a: log_message("ld a, (bc)"); break;
    case 0x0b: log_message("dec bc"); break;
    case 0x0c: log_message("inc c"); break;
    case 0x0d: log_message("dec c"); break;
    case 0x0e: log_message("ld c, %02x", code[1]); break;
    case 0x0f: log_message("rrca"); break;

    case 0x10: log_message("djnz %02x", code[1]); break;
    case 0x11: log_message("ld de, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x12: log_message("ld (de), a"); break;
    case 0x13: log_message("inc de"); break;
    case 0x14: log_message("inc d"); break;
    case 0x15: log_message("dec d"); break;
    case 0x16: log_message("ld d, %02x", code[1]); break;
    case 0x17: log_message("rla"); break;
    case 0x18: log_message("jr *"); break;
    case 0x19: log_message("add hl, de"); break;
    case 0x1a: log_message("ld a, (de)"); break;
    case 0x1b: log_message("dec de"); break;
    case 0x1c: log_message("inc e"); break;
    case 0x1d: log_message("dec e"); break;
    case 0x1e: log_message("ld e, %02x", code[1]); break;
    case 0x1f: log_message("rra"); break;

    case 0x20: log_message("jr nz, %0x2", code[1]); break;
    case 0x21: log_message("ld hl, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x22: log_message("ld (%04x), hl", *((uint16_t *)(&code[1]))); break;
    case 0x23: log_message("inc hl"); break;
    case 0x24: log_message("inc h"); break;
    case 0x25: log_message("dec h"); break;
    case 0x26: log_message("ld h, %02x", code[1]); break;
    case 0x27: log_message("daa"); break;
    case 0x28: log_message("jr z, %02x", code[1]); break;
    case 0x29: log_message("add hl, hl"); break;
    case 0x2a: log_message("ld hl, (%04x)", *((uint16_t *)(&code[1]))); break;
    case 0x2b: log_message("dec hl"); break;
    case 0x2c: log_message("inc l"); break;
    case 0x2d: log_message("dec l"); break;
    case 0x2e: log_message("ld l, %02x", code[1]); break;
    case 0x2f: log_message("cpl"); break;

    case 0x30: log_message("jr nc, %02x", code[1]); break;
    case 0x31: log_message("ld sp, %04x", *((uint16_t *)(&code[1]))); break;
    case 0x32: log_message("ld (%04x), a", *((uint16_t *)(&code[1]))); break;
    case 0x33: log_message("inc sp"); break;
    case 0x34: log_message("inc (hl)"); break;
    case 0x35: log_message("dec (hl)"); break;
    case 0x36: log_message("ld (hl), %02x", code[1]); break;
    case 0x37: log_message("scf"); break;
    case 0x38: log_message("jr c, %s%02x", (signed)code[1] < 0 ? "-" : "+", (signed)code[1]); break;
    case 0x39: log_message("add hl, sp"); break;
    case 0x3a: log_message("ld a, (%04x)", *((uint16_t *)(&code[1]))); break;
    case 0x3b: log_message("dec sp"); break;
    case 0x3c: log_message("inc a"); break;
    case 0x3d: log_message("dec a"); break;
    case 0x3e: log_message("ld a, %02x", code[1]); break;
    case 0x3f: log_message("ccf"); break;

    case 0x40: log_message("ld b, b"); break;
    case 0x41: log_message("ld b, c"); break;
    case 0x42: log_message("ld b, d"); break;
    case 0x43: log_message("ld b, e"); break;
    case 0x44: log_message("ld b, h"); break;
    case 0x45: log_message("ld b, l"); break;
    case 0x46: log_message("ld b, (hl)"); break;
    case 0x47: log_message("ld b, a"); break;
    case 0x48: log_message("ld c, b"); break;
    case 0x49: log_message("ld c, c"); break;
    case 0x4a: log_message("ld c, d"); break;
    case 0x4b: log_message("ld c, e"); break;
    case 0x4c: log_message("ld c, h"); break;
    case 0x4d: log_message("ld c, l"); break;
    case 0x4e: log_message("ld c, (hl)"); break;
    case 0x4f: log_message("ld c, a"); break;

    case 0x50: log_message("ld d, b"); break;
    case 0x51: log_message("ld d, c"); break;
    case 0x52: log_message("ld d, d"); break;
    case 0x53: log_message("ld d, e"); break;
    case 0x54: log_message("ld d, h"); break;
    case 0x55: log_message("ld d, l"); break;
    case 0x56: log_message("ld d, (hl)"); break;
    case 0x57: log_message("ld d, a"); break;
    case 0x58: log_message("ld e, b"); break;
    case 0x59: log_message("ld e, c"); break;
    case 0x5a: log_message("ld e, d"); break;
    case 0x5b: log_message("ld e, e"); break;
    case 0x5c: log_message("ld e, h"); break;
    case 0x5d: log_message("ld e, l"); break;
    case 0x5e: log_message("ld e, (hl)"); break;
    case 0x5f: log_message("ld e, a"); break;

    case 0x60: log_message("ld h, b"); break;
    case 0x61: log_message("ld h, c"); break;
    case 0x62: log_message("ld h, d"); break;
    case 0x63: log_message("ld h, e"); break;
    case 0x64: log_message("ld h, h"); break;
    case 0x65: log_message("ld h, l"); break;
    case 0x66: log_message("ld h,(hl)"); break;
    case 0x67: log_message("ld h, a"); break;
    case 0x6f: log_message("ld l, a"); break;

    case 0x70: log_message("ld (hl), b"); break;
    case 0x71: log_message("ld (hl), c"); break;
    case 0x72: log_message("ld (hl), d"); break;
    case 0x73: log_message("ld (hl), e"); break;
    case 0x74: log_message("ld (hl), h"); break;
    case 0x75: log_message("ld (hl), l"); break;
    case 0x76: log_message("halt"); break;
    case 0x77: log_message("ld (hl), a"); break;
    case 0x78: log_message("ld a, b"); break;
    case 0x79: log_message("ld a, c"); break;
    case 0x7a: log_message("ld a, d"); break;
    case 0x7b: log_message("ld a, e"); break;
    case 0x7c: log_message("ld a, h"); break;
    case 0x7d: log_message("ld a, l"); break;
    case 0x7e: log_message("ld a, (hl)"); break;
    case 0x7f: log_message("ld a, a"); break;

    case 0x80 ... 0x9f: log_message("add a, r"); break;
        
    case 0xa0: log_message("and b"); break;
    case 0xa1: log_message("and c"); break;
    case 0xa2: log_message("and d"); break;
    case 0xa3: log_message("and e"); break;
    case 0xa4: log_message("and h"); break;
    case 0xa5: log_message("and l"); break;
    case 0xa6: log_message("and (hl)"); break;
    case 0xa7: log_message("and a"); break;
    case 0xa8: log_message("xor b"); break;
    case 0xa9: log_message("xor c"); break;
    case 0xaa: log_message("xor d"); break;
    case 0xab: log_message("xor e"); break;
    case 0xac: log_message("xor h"); break;
    case 0xad: log_message("xor l"); break;
    case 0xae: log_message("xor (hl)"); break;
    case 0xaf: log_message("xor a"); break;

    case 0xb0 ... 0xbf: break;
    /* case 0xb6: log_message("or (hl)"); break; */
    /* case 0xb7: log_message("or a"); break; */
    /* case 0xbe: log_message("cp (hl)"); break; */
        
    case 0xc0: log_message("ret nz"); break;
    case 0xc1: log_message("pop bc"); break;
    case 0xc2: log_message("jp nz, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc3: log_message("jp %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc4: log_message("call %04x", *((uint16_t *)(&code[1]))); break;
    case 0xc5: log_message("push bc"); break;
    case 0xc6: log_message("add a, %02x", code[1]); break;
    case 0xc8: log_message("ret z"); break;
    case 0xc9: log_message("ret"); break;
    case 0xca: log_message("jp z, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xcb: decode_cb(state); break;
    case 0xcc: log_message("call z, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xcd: log_message("call %04x", *((uint16_t *)(&code[1]))); break;

    case 0xd0: log_message("ret nc"); break;
    case 0xd1: log_message("pop de"); break;
    case 0xd2: log_message("jp nc, %04x", *((uint16_t *)(&code[1]))); break;
      
    case 0xd4: log_message("call nc, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xd5: log_message("push de"); break;
    case 0xd6: log_message("sub %02x", code[1]); break;
    case 0xd8: log_message("ret c"); break;
    case 0xd9: log_message("exx"); break;
    case 0xda: log_message("jp c, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xdb: log_message("in a, (%02x)", code[1]); break;
    case 0xdc: log_message("call c, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xdd: decode_dd(state); break;
        
    case 0xe0: log_message("ret po"); break;
    case 0xe1: log_message("pop hl"); break;
    case 0xe2: log_message("jp po, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xe4: log_message("call po, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xe5: log_message("push hl"); break;
    case 0xe6: log_message("and %02x", code[1]); break;
    case 0xe8: log_message("ret pe"); break;
    case 0xe9: log_message("jp (hl)"); break;
    case 0xea: log_message("jp pe, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xeb: log_message("ex de, hl"); break;
    case 0xec: log_message("call pe, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xed: decode_ed(state); break;

    case 0xf0: log_message("ret p"); break;
    case 0xf1: log_message("pop af"); break;
    case 0xf2: log_message("jp p, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xf3: log_message("di"); break;
    case 0xf4: log_message("call p, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xf5: log_message("push af"); break;
    case 0xf8: log_message("ret m"); break;
    case 0xf9: log_message("ld sp, hl"); break;
    case 0xfa: log_message("jp m, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xfb: log_message("ei"); break;
    case 0xfc: log_message("call m, %04x", *((uint16_t *)(&code[1]))); break;
    case 0xfd: decode_fd(state); break;
    case 0xfe: log_message("cp %02x", code[1]); break;
    default: unimplemented_op(*code); break;
    };
    log_message("\n");
}
