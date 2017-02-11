
#include <stdio.h>
#include "disassembler.h"

// opcode extensions
const char *grp1[] = {"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"};
const char *grp2[] = {"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "-", "SAR"};

const char *grp4[] = {"INC", "DEC"};
const char *grp5[] = {"INC", "DEC", "CALL", "CALL Mp", "JMP", "JMP Mp", "PUSH"};

const char *reg_w0[] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};
const char *reg_w1[] = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};

const char *rm_table1[] = {"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "Drc't Addr" /* Direct addressing */, "[BX]"}; // mod:00
const char *rm_table2[] = {"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "[BP]", "[BX]"}; // mod:01


int disassemble_opcode_8086(state_t *state, int ip) {

  unsigned char *code = &state->memory[ip];
  int opbytes = 1;
  printf("%04x %02x ", ip, code[0]);

  switch (*code) {

  case 0x00: // ADD Eb, Gb
    // d:0 (rm <- reg), w:0
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADD [%02x], %s", code[2], reg_w0[reg]);
          opbytes += 2;
        } else {
          printf("ADD %s, %s", rm_table1[rm], reg_w0[reg]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADD %s, %s", rm_table2[rm], reg_w0[reg]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADD %s, %s", reg_w0[rm], reg_w0[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    // unimplemented_instruction(*code);
    break;

  case 0x01: // ADD Ev, Gv
    // d:0 (rm <- reg), w:1
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADD [%04x], %s", (code[3] << 8) | code[2], reg_w1[reg]);
          opbytes += 2;
        } else {
          printf("ADD %s, %s", rm_table1[rm], reg_w1[reg]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADD %s, %s", reg_w1[rm], reg_w1[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    break;

  case 0x02: // ADD Gb, Eb
    // d:1 (reg <- rm), w:1
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADD %s, [%02x]", reg_w0[reg], code[2]);
          opbytes += 1;
        } else {
          printf("ADD %s, %s", reg_w0[reg], rm_table1[rm]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADD %s, %s", reg_w0[reg], rm_table2[rm]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADD %s, %s", reg_w0[reg], reg_w0[rm]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 2;
    }
    // unimplemented_instruction(*code);
    break;

    
  case 0x03: // ADD Gv, Ev
    // d:1 (reg <- rm), w:1
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADD %s, [%04x]", reg_w1[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("ADD %s, %s", reg_w1[reg], rm_table1[rm]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADD %s, %s", reg_w1[reg], rm_table2[rm]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADD %s, %s", reg_w1[reg], reg_w1[rm]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    break;

  case 0x04: // ADD AL, Ib
    printf("ADD AL, %02x", code[1]);
    opbytes += 1;
    break;
    
  case 0x05: // ADD AX, Iv
    printf("ADD AX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;


  case 0x10: // ADC Eb, Gb
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADC [%04x], %s", (code[3] << 8) | code[2], reg_w0[reg]);
          opbytes += 2;
        } else {
          printf("ADC %s, %s", rm_table1[rm], reg_w0[reg]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADC %s, %s", rm_table2[rm], reg_w0[reg]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADC %s, %s", reg_w0[rm], reg_w0[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
      // unimplemented_instruction(*code);
    }
    break;


    

  case 0x11: // ADC Ev, Gv
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADC [%04x], %s", (code[3] << 8) | code[2], reg_w1[reg]);
          opbytes += 2;
        } else {
          printf("ADC %s, %s", rm_table1[rm], reg_w1[reg]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADC %s, %s", rm_table2[rm], reg_w1[reg]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADC %s, %s", reg_w1[rm], reg_w1[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
      // unimplemented_instruction(*code);
    }
    break;

  case 0x12: // ADC Gb, Eb
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADC %s, [%04x]", reg_w0[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("ADC %s, %s", reg_w0[reg], rm_table1[rm]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADC %s, %s", reg_w0[reg], rm_table2[rm]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADC %s, %s", reg_w0[reg], reg_w0[rm]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
      // unimplemented_instruction(*code);
    }
    break;


    
  case 0x13: // ADC Gv, Ev
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("ADC %s, [%04x]", reg_w1[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("ADC %s, %s", reg_w1[reg], rm_table1[rm]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("ADC %s, %s", reg_w1[reg], rm_table2[rm]);        
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("ADC %s, %s", reg_w1[reg], reg_w1[rm]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
      // unimplemented_instruction(*code);
    }
    break;

  case 0x14: // ADC AL, Ib
    printf("ADC AL, %02x", code[1]);
    opbytes += 1;
    // unimplemented_instruction(*code);
    break;
    
  case 0x15: // ADC AX, Iv
    printf("ADC AX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    // unimplemented_instruction(*code);
    break;


  case 0x47: // INC DI
    printf("INC DI");
    break;

    
  case 0x80: //GRP1 Ev, Iv
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("%s [%04x], %02x", grp1[reg], (code[3] << 8) | code[2], code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %02x", grp1[reg], rm_table1[rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("%s %s, %02x", grp1[reg], rm_table2[rm], code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %02x", grp1[reg], reg_w0[rm], code[2]);
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
          printf("%s [%04x], %04x", grp1[reg], (code[3] << 8) | code[2], (code[5] << 8) | code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %04x", grp1[reg], rm_table1[rm], (code[3] << 8) | code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("%s %s, %04x", grp1[reg], rm_table2[rm], (code[3] << 8) | code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %04x", grp1[reg], reg_w1[rm], (code[3] << 8) | code[2]);
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
          printf("%s [%04x], %02x", grp1[reg], (code[3] << 8) | code[2], code[4]);
          opbytes += 2;
        } else {
          printf("%s %s, %02x", grp1[reg], rm_table1[rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("ADD %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("%s %s, %02x", grp1[reg], rm_table2[rm], code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table   
        printf("%s %s, %02x", grp1[reg], reg_w1[rm], code[2]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 2;
    }
    break;
    
  case 0x89: // MOV Ev, Gv
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;

      if (mod == 0b00) {
        // 00 Use R/M Table 1 for R/M operand
        if (rm == 6) {
          printf("MOV [%04x], %s", (code[3] << 8) | code[2], reg_w1[reg]);
          opbytes += 2;
        } else {
          printf("MOV %s, %s", rm_table1[rm], reg_w1[reg]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("MOV %s, %s", rm_table2[rm], reg_w1[reg]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table
        printf("MOV %s, %s", reg_w1[rm], reg_w1[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    //unimplemented_instruction(*code);
    break;

  case 0x8e: // MOV Sw Ew
    unimplemented_instruction(state);
    break;
    
  case 0x9c: // PUSHF
    printf("PUSHF");
    break;
    
  case 0xa3: // MOV Ov, AX
    printf("MOV [%04x], AX", (code[2] << 8) | code[1]);
    opbytes += 2;
    //unimplemented_instruction(*code);
    break;

  case 0xb0: // MOV AL Ib
    printf("MOV AL, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb1: // MOV CL Ib
    printf("MOV CL, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb2: // MOV DL Ib
    printf("MOV DL, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb3: // MOV BL Ib
    printf("MOV BL, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb4: // MOV AH Ib
    printf("MOV AH, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb5: // MOV CH Ib
    printf("MOV CH, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb6: // MOV DH, Ib
    printf("MOV DH, %02x", code[1]);
    opbytes += 1;
    break;
  case 0xb7: // MOV BH, Ib
    printf("MOV BH, %02x", code[1]);
    opbytes += 1;
    break;
    

  case 0xb8: // MOV AX, Iv
    printf("MOV AX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xb9: // MOV CX, Iv
    printf("MOV CX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xba: // MOV DX, Iv
    printf("MOV DX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xbb: // MOV BX, Iv
    printf("MOV BX, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xbc: // MOV SP, Iv
    printf("MOV SP, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xbd: // MOV BP, Iv
    printf("MOV BP, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xbe: // MOV SI, Iv
    printf("MOV SI, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;
  case 0xbf: // MOV DI, Iv
    printf("MOV DI, %04x", (code[2] << 8) | code[1]);
    opbytes += 2;
    break;

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
          printf("MOV %s, %02x", rm_table1[rm], code[2]);
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("MOV %s, %02x", rm_table2[rm], code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table
        unimplemented_instruction(state); // second arg is Iv
        // printf("ADD %s, %s", reg_w1[rm], reg_w1[reg]);
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
          printf("MOV %s, %04x", rm_table1[rm], (code[3] << 8) | code[2]);
          opbytes += 2;
        }
      } else if (mod == 0b01) {
        // 01 Use R/M Table 2 with 8-bit displacement
        //printf("MOV %s, %s", rm_table2[rm], reg_w1[reg]);
        unimplemented_instruction(state);
      } else if (mod == 0b10) {
        // 10 Use R/M Table 2 with 16-bit displacement
        printf("MOV %s, %04x", rm_table2[rm], (code[3] << 8) | code[2]);
      } else if (mod == 0b11) {
        // 11 Two register instruction; use REG table
        unimplemented_instruction(state); // second arg is Iv
        // printf("ADD %s, %s", reg_w1[rm], reg_w1[reg]);
      } else {
        // should never be reached
        unimplemented_instruction(state);
      }
      opbytes += 1;
    }
    // unimplemented_instruction(*code);
    break;

  case 0xf4: // HLT
    printf("HLT");
    exit(EXIT_SUCCESS);
    break;

  case 0xfe: // GRP4 Eb
    {
      uint8_t mod = (code[1] & 0b11000000) >> 6;
      uint8_t reg = (code[1] & 0b00111000) >> 3;
      uint8_t rm = code[1] & 0b00000111;
      if(0 == mod) {
        if (6 == rm) {
          printf("%s [%04x]", grp4[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("%s %s", grp4[reg], rm_table1[rm]);
        }
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        printf("%s %s", grp4[reg], reg_w0[rm]);
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
          printf("%s [%04x]", grp5[reg], (code[3] << 8) | code[2]);
          opbytes += 2;
        } else {
          printf("%s %s", grp5[reg], rm_table1[rm]);
        }
      } else if (1 == mod) {
        unimplemented_instruction(state);
      } else if (2 == mod) {
        unimplemented_instruction(state);
      } else if (3 == mod) {
        printf("%s %s", grp5[reg], reg_w1[rm]);
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
