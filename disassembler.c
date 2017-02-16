
#include <stdio.h>
#include "disassembler.h"

// opcode extensions
const char *grp_table[6][8] = {
  {}, // padding
  {"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"}, // GRP1
  {"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "-", "SAR"}, // GRP2
  {"TEST", "-", "NOT", "NEG", "MUL", "IMUL", "DIV", "IDIV"}, // GRP3
  {"INC", "DEC"}, // GRP4
  {"INC", "DEC", "CALL", "CALL Mp", "JMP", "JMP Mp", "PUSH"}}; // GRP5

const char *reg_table[2][8] = {
  {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"}, // w = 0
  {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"}}; // w = 1

const char *sreg_table[] = {"ES", "CS", "SS", "DS"};

const char *rm_table[2][8] = {
  {"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "Drc't Addr", "[BX]"}, // mod:00
  {"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "[BP]", "[BX]"}}; // mod:01

void print_e(state_t *state) {
  uint8_t *code = &state->memory[state->ip];
  uint8_t w = code[0] & 0b1;
  uint8_t mod = (code[1] & 0b11000000) >> 6;
  uint8_t rm = code[1] & 0b00000111;
  if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
    if (rm == 6) {
      printf("[%04x]", (code[3] << 8) | code[2]);
    } else {
      printf("%s", rm_table[mod][rm]);
    }
  } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
    printf("[%s+%02x]", rm_table[mod][rm], code[2]);
  } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
    printf("[%s+%04x]", rm_table[mod][rm], (code[3] << 8) | code[2]);
  } else if (mod == 0b11) { // 11 Two register instruction; use REG table   
    printf("%s", reg_table[w][rm]);
  } else { // should never be reached
    unimplemented_instruction(state);
  }
}

void print_g(state_t *state) {
  uint8_t *code = &state->memory[state->ip];
  uint8_t w = code[0] & 0b1;
  // uint8_t mod = (code[1] & 0b11000000) >> 6;
  // uint8_t rm = code[1] & 0b00000111;
  uint8_t reg = (code[1] & 0b00111000) >> 3;

  printf("%s", reg_table[w][reg]);
}

int disassemble_opcode_8086(state_t *state, int ip) {

  unsigned char *code = &state->memory[ip];
  // uint8_t d = (code[0] & 0b10) >> 1;
  uint8_t w = code[0] & 0b1;
  
  int opbytes = 1;
  printf("%04x %02x ", ip, code[0]);

  switch (*code) {

  case 0x00: // ADD Eb, Gb
  case 0x01:
    printf("ADD ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x02: // ADD Gb, Eb
  case 0x03:
    printf("ADD ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;

  case 0x04: printf("ADD AL, %02x", code[1]); break; // ADD AL, Ib
  case 0x05: printf("ADD AX, %04x", (code[2] << 8) | code[1]); break; // ADD AX, Iv

  case 0x06: printf("PUSH ES"); break;
  case 0x07: printf("POP ES"); break;

  case 0x08:
  case 0x09:
    printf("ADC ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x0a:
  case 0x0b:
    printf("ADC ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;
        
  case 0x10: // ADC Eb, Gb
  case 0x11:
    printf("ADC ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x12: // ADC Gb, Eb
  case 0x13:
    printf("ADC ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;

  case 0x14: printf("ADC AL, %02x", code[1]); break; // ADC AL, Ib
  case 0x15: printf("ADC AX, %04x", (code[2] << 8) | code[1]); break; // ADC AX, Iv

  case 0x16: printf("PUSH SS"); break;
  case 0x17: printf("POP SS"); break;

  case 0x18:
  case 0x19:
    printf("SBB ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x1a:
  case 0x1b:
    printf("SBB ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;

    
  case 0x20: // AND Eb, Gb
  case 0x21:
    printf("AND ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x22: // AND Gb, Eb
  case 0x23:
    printf("AND ");
    print_g(state);
    printf(", "); 
    print_e(state);
    break;

  case 0x24: printf("AND AL, %02x", code[1]); break; // ADC AL, Ib
  case 0x25: printf("AND AX, %04x", (code[2] << 8) | code[1]); break; // ADC AX, Iv

  case 0x26: printf("ES:"); break;
  case 0x27: printf("DAA"); break;

  case 0x28:
  case 0x29:
    printf("SUB ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;

  case 0x2a:
  case 0x2b:
    printf("SUB ");
    print_g(state);
    printf(", "); 
    print_e(state);
    break;

    
  case 0x30: // XOR Eb, Gb
  case 0x31: printf("XOR "); print_e(state); printf(", "); print_g(state); break;
  case 0x32: // XOR Gb, Eb
  case 0x33: printf("XOR "); print_g(state); printf(", "); print_e(state); break;
  case 0x34: printf("XOR AL, %02x", code[1]); break; // ADC AL, Ib
  case 0x35: printf("XOR AX, %04x", (code[2] << 8) | code[1]); break; // ADC AX, Iv
  case 0x36: printf("SS:"); break;
  case 0x37: printf("AAA"); break;
  case 0x38: // ADC Eb, Gb
  case 0x39: printf("CMP "); print_e(state); printf(", "); print_g(state); break;
  case 0x3a: // ADC Eb, Gb
  case 0x3b: printf("CMP "); print_g(state); printf(", "); print_e(state); break;

  case 0x3c: printf("CMP AL, %02x", code[1]); break;
    
  case 0x40 ... 0x47: printf("INC %s", reg_table[1][code[0] & 0b111]); break; // INC reg16
  case 0x48 ... 0x4f: printf("DEC %s", reg_table[1][code[0] & 0b111]); break;  // DEC reg16
  case 0x50 ... 0x57: printf("PUSH %s", reg_table[1][code[0] & 0b111]); break; // PUSH reg16
  case 0x58 ... 0x5f: printf("POP %s", reg_table[1][code[0] & 0b111]); break; // POP reg16

  case 0x70 ... 0x7f: // Conditional jumps
    {
      const char *jump_name[] = {"JO", "JNO", "JB", "JNB", "JZ", "JNZ", "JBE", "JA", "JS", "JNS", "JPE", "JPO", "JL", "JGE", "JLE", "JG"};
      printf("%s %04x", jump_name[code[0] & 0b1111], state->ip + (int8_t)code[1] + 2);
    }
    break;
    
  case 0x80: // GRP1 Eb, Ib
  case 0x82: // GRP Eb, Ib
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("%s [%04x], %02x", grp_table[1][reg], (code[3] << 8) | code[2], code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %02x", grp_table[1][reg], rm_table[mod][rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("%s %s, %02x", grp_table[1][reg], rm_table[mod][rm], code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %02x", grp_table[1][reg], reg_table[w][rm], code[2]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 2;
    }
    //unimplemented_instruction(*code);
    break;


  case 0x81: //GRP1 Ev, Iv
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("%s [%04x], %04x", grp_table[1][reg], (code[3] << 8) | code[2], (code[5] << 8) | code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %04x", grp_table[1][reg], rm_table[mod][rm], (code[3] << 8) | code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("%s %s, %04x", grp_table[1][reg], rm_table[mod][rm], (code[3] << 8) | code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %04x", grp_table[1][reg], reg_table[w][rm], (code[3] << 8) | code[2]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 3;
    }
    break;

    
  case 0x83: //GRP1 Ev, Ib
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("%s [%04x], %02x", grp_table[1][reg], (code[3] << 8) | code[2], code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %02x", grp_table[1][reg], rm_table[mod][rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        //printf("%s %s, %02x", grp_table[1][reg], rm_table[mod][rm], code[2]);
        unimplemented_instruction(state);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %04x", grp_table[1][reg], reg_table[w][rm], (uint16_t)(int8_t)code[2]); // sign extension
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 2;
    }
    break;

  case 0x84: // TEST Gb, Ev
  case 0x85:
    printf("TEST ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;

  case 0x86: // XCHG Gb, Ev
  case 0x87: printf("XCHG "); print_g(state); printf(", "); print_e(state); break;

  case 0x88:
  case 0x89:
    printf("MOV ");
    print_e(state);
    printf(", ");
    print_g(state);
    break;
  case 0x8a:
  case 0x8b:
    printf("MOV ");
    print_g(state);
    printf(", ");
    print_e(state);
    break;
    
  case 0x8e: { // MOV Sw Ew
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;

      if (mod == 0b00) { // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("MOV %s, [%04x]", sreg_table[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("MOV %s, %s", sreg_table[reg], rm_table[mod][rm]);
        }
      } else if (mod == 0b01) { // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) { // 10 Use R/M Table 2 with 16-bit displacement
        //printf("MOV %s, %s", sreg_table[reg], rm_table[mod][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b11) { // 11 Two register instruction; use REG table
        printf("MOV %s, %s", sreg_table[reg], reg_table[w][rm]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    // unimplemented_instruction(state);
    break;

  case 0x90: printf("NOP"); break; // NOP

  case 0x91 ... 0x97: // XCHG reg16, reg16
    printf("XCHG %s, AX", reg_table[1][code[0] & 0b111]);
    break;
    
  case 0x9c: printf("PUSHF"); break; // PUSHF
  case 0x9d: printf("POPF"); break; // POPF

  case 0xa0: printf("MOV AL, [+%02x]", (int8_t)code[1]); break; // MOV AL, Ob
  case 0xa1: printf("MOV AX, [+%04x]", (int16_t)((code[2] << 8) | code[1])); break; // MOV AX, Ov
  case 0xa2: printf("MOV [+%02x], AL", (int8_t)code[1]); break; // MOV Ob, AL
  case 0xa3: printf("MOV [+%04x], AX", (int16_t)((code[2] << 8) | code[1])); break; // MOV Ov, AX

  case 0xa4: printf("MOVSB"); break;
  case 0xa5: printf("MOVSW"); break;
  case 0xa6: printf("CMPSB"); break;
  case 0xa7: printf("MOVSW"); break;
    
  case 0xb0 ... 0xb7: // MOV reg8, Ib
    {
      uint8_t reg = code[0] & 0b111;
      printf("MOV %s, %02x", reg_table[0][reg], code[1]);
    }
    break;

  case 0xb8 ... 0xbf: // MOV reg16, Iv
    {
      uint8_t reg = code[0] & 0b111;
      printf("MOV %s, %04x", reg_table[1][reg], (code[2] << 8) | code[1]);
    }
    break;

  case 0xc3: printf("RET"); break; // RET
    
  case 0xc6: // MOV Eb Ib
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t rm = code[1] & 0b00000111;

      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("MOV [%04x], %02x", (code[3] << 8) | code[2], code[4]);
          opbytes += 2;
        } else {
          printf("MOV %s, %02x", rm_table[mod][rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("MOV %s, %02x", rm_table[mod][rm], code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table
        unimplemented_instruction(state); // second arg is Iv
        // printf("ADD %s, %s", reg_table[w][rm], reg_table[w][reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 2;
    }
    //unimplemented_instruction(*code);
    break;
    
  case 0xc7: // MOV Ev, Iv
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t rm = code[1] & 0b00000111;

      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("MOV [%04x], %04x", (code[3] << 8) | code[2], (code[5] << 8) | code[4]);
          opbytes += 4;
        } else {
          printf("MOV %s, %04x", rm_table[mod][rm], (code[3] << 8) | code[2]);
          opbytes += 2;
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table[mod][rm], reg_table[w][reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("MOV %s, %04x", rm_table[mod][rm], (code[3] << 8) | code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table
        unimplemented_instruction(state); // second arg is Iv
        // printf("ADD %s, %s", reg_table[w][rm], reg_table[w][reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    // unimplemented_instruction(*code);
    break;

  case 0xe8: printf("CALL %04x", state->ip + (int16_t)((code[2] << 8) | code[1]) + 3); opbytes += 2; break;
  case 0xe9: printf("JMP Jv"); break;
  case 0xea: printf("JMP Ap"); break;
    
  case 0xeb: printf("JMP %04x", state->ip + code[1] + 1); break; // JMP Jb
    
    
  case 0xf4: printf("HLT"); break; // HLT
  case 0xf5: printf("CMC"); break; // CMC

  case 0xf8: printf("CLC"); break; // CLC
  case 0xf9: printf("STC"); break; // STC
  case 0xfa: printf("CLI"); break; // CLI
  case 0xfb: printf("STI"); break; // STI
  case 0xfc: printf("CLD"); break; // CLD
  case 0xfd: printf("STD"); break; // STD
    
  case 0xfe: // GRP4 Eb
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if(0 == mod) {
        if (6 == rm) {
          printf("%s [%04x]", grp_table[4][reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("%s %s", grp_table[4][reg], rm_table[mod][rm]);
        }
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        printf("%s %s", grp_table[4][reg], reg_table[w][rm]);
      } else {
        // should never be reached
      }
      opbytes += 1;
    }
    // unimplemented_instruction(*code);
    break;

    
  case 0xff: // GRP5 Ev
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if(0 == mod) {
        if (6 == rm) {
          printf("%s [%04x]", grp_table[5][reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("%s %s", grp_table[5][reg], rm_table[mod][rm]);
        }
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        printf("%s %s", grp_table[5][reg], reg_table[w][rm]);
      } else {
        // should never be reached
      }
      opbytes += 1;
      // unimplemented_instruction(*code);
    }
    break;
    
  default:
    unimplemented_instruction(state);
    exit(EXIT_FAILURE);
  }
    
  printf("\n");
  return opbytes;
}
