//
//  emulator.c
//  z80
//
//  Created by Alexander Litvinov on 1/21/17.
//  Copyright © 2017 Alexander Litvinov. All rights reserved.
//

#include "emulator.h"

#define CPUDIAG_CALL5
#define SHOW_DISASM

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

void maybe_set_condition_flag_cy_on_add(state_t *state, uint8_t arg1, uint8_t arg2) {
  /* Carry:
     If the instruction resulted in a carry (from addition),
     or a borrow (from sub- traction or a comparison) out of the high-
     order bit, this flag is set;
     otherwise it is reset.
  */
  /*
    a) two unsigned numbers were added and the result is larger than "capacity"
    of register where it is saved. Ex: we wanna add two 8 bit numbers
    and save result in 8 bit register. Example: 255 + 9 = 264 which is more
    that 8 bit register can store. So the value "8" will be
    saved there (264 & 255 = 8) and CF flag will be set.
  */
  state->cc.cy = ((arg1 + arg2) > 0xff);
}

void set_cy_flag_sub(state_t *state, uint8_t arg1, uint8_t arg2) {
  /*
    b) two unsigned numbers were substracted and we substracked
    bigger one from smaller one. Ex: 1-2 will give you 255 in result and CF flag will be set.
  */
  state->cc.cy = (arg1 < arg2);
}

void set_zsp_flags(state_t *state, uint16_t value) {
  /* Zero: If the result of an instruction has the value 0, this flag is set; otherwise it is reset. */
  state->cc.z = (value == 0);

  /* Sign: If the most significant bit of the result of the operation
     has the value 1, this flag is set; otherwise it is reset. */
  state->cc.s = ((value & 0x80) == 0x80);

  /* Parity: If the modulo 2 sum of the bits of the result of the operation is 0, (I.e., if the result
     has even parity), this flag is set; otherwise it is reset (I.e., if the result has odd parity). */
  state->cc.p = parity16(value);
}

void print_state(state_t *state) {
  printf("\t==PC:0x%04x SP:0x%04x A:0x%02x B:0x%02x C:0x%02x D:0x%02x E:0x%02x H:0x%02x L:0x%02x\n",
         state->pc, state->sp, state->a, state->b, state->c, state->d, state->e, state->h, state->l);
  printf("\t==Z:%01x S:%01x P:%01x Cy:%01x Ac:%01x\n",
         state->cc.z, state->cc.s, state->cc.p, state->cc.cy, state->cc.ac);
}

void unimplemented_instruction(state_t *state) {
  unsigned char opcode = state->memory[state->pc];
    
  printf("Error: Unimplemented instruction: 0x%02x\n", opcode);
  printf("--------------------------------\n");
  print_state(state);
  disassemble_opcode_8080(state->memory, state->pc);
    
  exit(EXIT_FAILURE);
}

void opcode_mvi(state_t *state, uint8_t *reg) {
  // (B) <- (byte2)
  *reg = state->memory[++state->pc];
}

void opcode_add(state_t *state, uint8_t reg) {
  // (A) <- (A) + (r)
  uint16_t res = state->a + reg;
  state->a = res & 0xff;
  set_zsp_flags(state, state->a);
  state->cc.cy = (res > 0xff);
}

void opcode_adc(state_t *state, uint8_t reg) {
  // (A) <- (A) + (r) + (CY)
  uint16_t res = state->a + reg + state->cc.cy;
  state->a = res & 0xff;
  set_zsp_flags(state, state->a);
  state->cc.cy = (res > 0xff);
}

void opcode_sub(state_t *state, uint8_t reg) {
  // (A) <- (A) - (r)
  uint16_t res = state->a - reg;
  state->a = res & 0xff;
  set_zsp_flags(state, state->a);
  state->cc.cy = (res > 0xff);
}

void opcode_sbb(state_t *state, uint8_t reg) {
  // (A) <- (A) - (r) - (CY)
  uint16_t res = state->a - reg - state->cc.cy;
  state->a = res & 0xff;
  set_zsp_flags(state, state->a);
  state->cc.cy = (res > 0xff);
}

void opcode_ana(state_t *state, uint8_t reg) {
  // (A) <- (A) & (r)
  state->a = state->a & reg;
  set_zsp_flags(state, state->a);
  state->cc.cy = state->cc.ac = 0;
}

void opcode_xra(state_t *state, uint8_t reg) {
  // (A) <- (A) ^ (r)
  state->a = state->a ^ reg;
  set_zsp_flags(state, state->a);
  state->cc.cy = state->cc.ac = 0;
}

void opcode_ora(state_t *state, uint8_t reg) {
  // (A) <- (A) | (r)
  state->a = state->a | reg;
  set_zsp_flags(state, state->a);  
  state->cc.cy = state->cc.ac = 0;
}

void opcode_cmp(state_t *state, uint8_t reg) {
  // (Z) <- 0 if (A) - (r) == 0
  uint16_t res = state->a - reg;
  set_zsp_flags(state, res);
  state->cc.cy = (res > 0xff);
}

void opcode_pop(state_t *state, uint8_t *reg) {
  *(reg - 1) = state->memory[state->sp++];
  *reg = state->memory[state->sp++];
}

void opcode_push(state_t *state, uint8_t *reg) {
  state->memory[--state->sp] = *reg;
  state->memory[--state->sp] = *(reg - 1);
}

void stack_put_addr16(state_t *state, uint16_t addr) {
  // stack helper to get the stuff less error prone
  // ((SP) - 1) <- (PCH)
  // ((SP) - 2) <- (PCL)
  // (SP) <- (SP) - 2
  state->memory[(uint16_t)(--state->sp)] = (addr >> 8) & 0xff;
  state->memory[(uint16_t)(--state->sp)] = (addr & 0xff);
}

uint16_t stack_take_addr16(state_t *state) {
  return state->memory[state->sp++] | (state->memory[state->sp++] << 8);
}

void decode_mov(uint8_t opcode) {
  // 0b01DDDSSS
  const char register_letters[] = {'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A'};
  uint8_t dst = (opcode & 0b00111000) >> 3;
  uint8_t src = opcode & 0b00000111;
  printf("MOV %c, %c", register_letters[dst], register_letters[src]);
}

int disassemble_opcode_8080(unsigned char *codebuffer, int pc) {
  unsigned char *code = &codebuffer[pc];
  int opbytes = 1;
  const char register_letters[] = {'B', 'C', 'D', 'E', 'H', 'L', 'M', 'A'};
  const char *register_pairs[] = {"B", "D", "H", "SP"};
  
  printf("%04x %02x ", pc, code[0]);
    
  switch (*code) {
  case 0x00: printf("NOP"); break;

  case 0x10:
  case 0x20:
  case 0x30:
    printf("-");
    break;

  case 0x01: printf("LXI    B, #$%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0x11: printf("LXI    D, #$%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0x21: printf("LXI    H, #$%02x%02x", code[2], code[1]); opbytes = 2; break;
  case 0x31: printf("LXI    SP, #$%02x%02x", code[2], code[1]); opbytes = 3; break;

  case 0x02: printf("STAX   B"); break;
  case 0x12: printf("STAX   D"); break;

  case 0x22: printf("SHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0x32: printf("STA    $%02x%02x", code[2], code[1]); opbytes = 3; break;

  case 0x03:
  case 0x13:
  case 0x23:
  case 0x33:
    {
      uint8_t rp = (*code & 0b00110000) >> 4;
      printf("INX    %s", register_pairs[rp]);
    }
    break;

  case 0x04: printf("INR    B"); break;
  case 0x14: printf("INR    D"); break;
  case 0x24: printf("INR    H"); break;
  case 0x34: printf("INR    M"); break;

  case 0x05: printf("DCR    B"); break;
  case 0x15: printf("DCR    D"); break;
  case 0x25: printf("DCR    H"); break;
  case 0x35: printf("DCR    M"); break;

  case 0x06: printf("MVI    B, #$%02x", code[1]); opbytes = 2; break;
  case 0x16: printf("MVI    D, #$%02x", code[1]); opbytes = 2; break;
  case 0x26: printf("MVI    H, #$%02x", code[1]); opbytes = 2; break;
  case 0x36: printf("MVI    M, #$%02x", code[1]); opbytes = 2; break;

  case 0x07: printf("RLC"); break;
  case 0x17: printf("RAL"); break;
  case 0x27: printf("DAA"); break;
  case 0x37: printf("STC"); break;

  case 0x08:
  case 0x18:
  case 0x28:
  case 0x38:
    printf("-");
    break;

  case 0x09: printf("DAD    B"); break;
  case 0x19: printf("DAD    D"); break;
  case 0x29: printf("DAD    H"); break;
  case 0x39: printf("DAD    SP"); break;

  case 0x0a: printf("LDAX   B"); break; // Load A indirect
  case 0x1a: printf("LDAX   D"); break;

  case 0x2a: printf("LHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0x3a: printf("LDA    $%02x%02x", code[2], code[1]); opbytes = 3; break;

  case 0x0b: printf("DCX    B"); break; // Decrement register
  case 0x1b: printf("DCX    D"); break;
  case 0x2b: printf("DCX    H"); break;
  case 0x3b: printf("DCX    SP"); break;

  case 0x0c: printf("INR    C"); break; // Increment register
  case 0x1c: printf("INR    E"); break;
  case 0x2c: printf("INR    L"); break;
  case 0x3c: printf("INR    A"); break;

  case 0x0d: printf("DCR    C"); break; // Decrement memory
  case 0x1d: printf("DCR    E"); break;
  case 0x2d: printf("DCR    L"); break;
  case 0x3d: printf("DCR    A"); break;
  
  case 0x0e: printf("MVI    C, #$%02x", code[1]); opbytes = 1; break;
  case 0x1e: printf("MVI    E, #$%02x", code[1]); opbytes = 2; break;
  case 0x2e: printf("MVI    L, #$%02x", code[1]); opbytes = 2; break;
  case 0x3e: printf("MVI    A, #0x%02x", code[1]); opbytes = 2; break;

  case 0x0f: printf("RRC"); break;
  case 0x1f: printf("RAR"); break;
  case 0x2f: printf("CMA"); break; // A <- !A
  case 0x3f: printf("CMC"); break;

  case 0x40 ... 0x75: // MOV dst, src
  case 0x77 ... 0x7f: // MOV dst, src
    decode_mov(*code);
    break;
  case 0x76: printf("HLT"); break;
  case 0x80 ... 0xbf:
    {
      const char *operations[] = {"ADD", "ADC", "SUB", "SBB", "ANA", "XRA", "ORA", "CMP"};
      uint8_t operation_id = (*code & 0b00111000) >> 3;
      uint8_t register_id = *code & 0b00000111;
      printf("%s %c", operations[operation_id], register_letters[register_id]);
    }
    break;

  case 0xc0: printf("RNZ"); break;
  case 0xd0: printf("RNC"); break;
  case 0xe0: printf("RPO"); break;
  case 0xf0: printf("RP"); break;

  case 0xc1: printf("POP    B"); break;
  case 0xc2: printf("JNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xc3: printf("JMP    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xc4: printf("CNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xc5: printf("PUSH   B"); break;
  case 0xc6: printf("ADI    #$%02x", code[1]); opbytes = 2; break;
  case 0xc7: printf("RST    0"); break; // CALL $0
  case 0xc8: printf("RZ"); break;
  case 0xc9: printf("RET"); break;
  case 0xca: printf("JZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xcb: printf("-"); break;
  case 0xcc: printf("CZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xcd: printf("CALL   $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xce: printf("ACI    #$%02x", code[1]); opbytes = 2; break;
  case 0xcf: printf("RST    1"); break; // CALL $8
  case 0xd1: printf("POP    D"); break;
  case 0xd2: printf("JNC    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xd3: printf("OUT    #$%02x", code[1]); opbytes = 2; break;
  case 0xd4: printf("CNC    $%02x%02x", code[2], code[1]); opbytes = 2; break;
  case 0xd5: printf("PUSH   D"); break;
  case 0xd6: printf("SUI    #$%02x", code[1]); opbytes = 2; break;
  case 0xd7: printf("RST    2"); break; // CALL $10
  case 0xd8: printf("RC"); break;
  case 0xd9: printf("-"); break;
  case 0xda: printf("JC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xdb: printf("IN     #$%02x", code[1]); opbytes = 2; break;
  case 0xdc: printf("CC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xdd: printf("-"); break;
  case 0xde: printf("SBI    #$%02x", code[1]); opbytes = 2; break;
  case 0xdf: printf("RST    3"); break; // CALL $18
  case 0xe1: printf("POP    H"); break;
  case 0xe2: printf("JPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xe3: printf("XTHL"); break;
  case 0xe4: printf("CPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xe5: printf("PUSH   H"); break;
  case 0xe6: printf("ANI    #$%02x", code[1]); opbytes = 2; break;
  case 0xe7: printf("RST    4"); break; //CALL $20
  case 0xe8: printf("RPE"); break;
  case 0xe9: printf("PCHL"); break;
  case 0xea: printf("JPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xeb: printf("XCHG"); break;
  case 0xec: printf("CPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xed: printf("0xED Extended Z80 instruction"); break;
  case 0xee: printf("XRI    #$%02x", code[1]); opbytes = 2; break;
  case 0xef: printf("RST    5"); break; // CALL $28
  case 0xf1: printf("POP    PSW"); break;
  case 0xf2: printf("JP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xf3: printf("DI"); break;
  case 0xf4: printf("CP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xf5: printf("PUSH   PSW"); break;
  case 0xf6: printf("ORI    #$%02x", code[1]); opbytes = 2; break;
  case 0xf7: printf("RST    6"); break; // CALL $30
  case 0xf8: printf("RM"); break;
  case 0xf9: printf("SPHL"); break;
  case 0xfa: printf("JM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xfb: printf("EI"); break;
  case 0xfc: printf("CM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
  case 0xfd: printf("-"); break;
  case 0xfe: printf("CPI    #$%02x", code[1]); opbytes = 2; break;
  case 0xff: printf("RST    7"); break;

  default:
    break;
  }
    
  printf("\n");
  return opbytes;
}

// Z80 ED extended opcode
void z80_opcode_ed(state_t *state, uint8_t opcode) {
  printf("Z80 special instruction: %02x\n", opcode);
      
      switch(opcode) {
      case 0xa0: // LDI
        // LD (DE), (HL)
        {
          uint16_t de = (state->d << 8) | state->e;
          uint16_t hl = (state->h << 8) | state->l;
          uint16_t bc = (state->b << 8) | state->c;
          state->memory[de] = state->memory[hl];
          de = de + 1;
          hl = hl + 1;
          bc = bc - 1;
          // TODO
          unimplemented_instruction(state);
        }
        break;
      case 0xa1: // CPI
        unimplemented_instruction(state);
        break;
      case 0xa2: // INI
        unimplemented_instruction(state);
        break;
      case 0xa3: // OUTI
        unimplemented_instruction(state);
        break;
      case 0xa8: // LDD
        unimplemented_instruction(state);
        break;
      case 0xa9: // CPD
        unimplemented_instruction(state);
        break;
      case 0xb0: // LDIR
        unimplemented_instruction(state);
        break;

      default:
        unimplemented_instruction(state);
        break;
      }
      unimplemented_instruction(state);
}

// Emulates opcode at once
int emulate_op(state_t *state) {

  unsigned char *opcode = &state->memory[state->pc];
  
  if (state->pc == 0) {
    printf("REACHED WBOOT - NORMAL EXIT");
    exit(EXIT_SUCCESS);
  }

  /* //Skip DAA test     */
  state->memory[0x59c] = 0xc3; //JMP
  state->memory[0x59d] = 0xc2;
  state->memory[0x59e] = 0x05;
   
  // print_state(state);
  disassemble_opcode_8080(state->memory, state->pc);
  // printf("\n");
  
  switch (*opcode) {
  case 0x00: break; // NOP
  case 0x01: // LXI B, data 16
    // (rh)(rl) <- (byte3)(byte2)
    state->c = opcode[1];
    state->b = opcode[2];
    state->pc += 2;
    break;
  case 0x11: // LXI D, data 16
    // (rh)(rl) <- (byte3)(byte2)
    state->e = opcode[1];
    state->d = opcode[2];
    state->pc += 2;
    break;
  case 0x21: // LXI H, data 16
    // (rh)(rl) <- (byte3)(byte2)
    state->l = opcode[1];
    state->h = opcode[2];
    state->pc += 2;
    break;
  case 0x02: // STAX B
    // ((rh)(rl)) <- (A)
    state->memory[state->bc] = state->a;
    break;
  case 0x03: // INX B
    // (rh)(rl) <- (rh)(rl) + 1
    state->bc++;
    break;
  case 0x04: set_zsp_flags(state, ++state->b); break; // INR B
  case 0x05: set_zsp_flags(state, --state->b); break; // DCR B
  case 0x06: opcode_mvi(state, &state->b); break; // MVI B, data 8
  case 0x07: // RLC
    // Rotate left
    // (An+1) <- (An); (AO) <- (A7)
    // (CY) <- (A7)
    {
      uint16_t res = (state->a << 1) | (state->a >> 7);
      state->a = res & 0xff;
      state->cc.cy = (res >> 8);
    }
    break;
  case 0x08: unimplemented_instruction(state); break; // No instruction
  case 0x09: // DAD B
    /*
      (H)(L) <- (H)(L) + (B)(C)
      Note: Only the CY flag is affected. It is set
      if there is a carry out of the double precision add;
      otherwise it is reset.
      Flags: CY
    */
    {
      uint32_t res = state->hl + state->bc;
      state->hl = res & 0xffff;
      state->cc.cy = ((res & 0xffff0000) != 0);
    }
    break;
  case 0x0a: // LDAX B
    state->a = state->memory[state->bc];
    break;
  case 0x0b: // DCX B
    state->bc--;
    break;
  case 0x0c: set_zsp_flags(state, ++state->c); break;// INR C
  case 0x0d: set_zsp_flags(state, --state->c); break; // DCR C
  case 0x0e: opcode_mvi(state, &state->c); break; // MVI C, data 8
  case 0x0f: // RRC
    //Rotate right
    {
      uint8_t x = state->a;
      state->a = ((x & 1) << 7) | (x >> 1);
      state->cc.cy = (1 == (x & 0x1));
    }
    break;
  case 0x10: unimplemented_instruction(state); break;  // No instruction
  case 0x12: // STAX D
    state->memory[state->de] = state->a;
    break;
  case 0x13: // INX D
    // (rh)(rl) <- (rh)(rl) + 1
    state->de++;
    break;
  case 0x14: set_zsp_flags(state, ++state->d); break; // INR D
  case 0x15: set_zsp_flags(state, --state->d); break; // DCR D
  case 0x16: opcode_mvi(state, &state->d); break; // MVI D, data
  case 0x17: // RAL
    // Rotate left through carry
    // (An+1) <- (An); (CY) <- (A7)
    // (AO) <- (CY)
    {
      uint16_t initial = state->a | (state->cc.cy << 8);
      uint16_t res = (initial << 1) | (initial >> 8);
      state->a = res & 0xff;
      state->cc.cy = (res >> 8) * 0xff;
    }
    break;
  case 0x18: unimplemented_instruction(state); break;  // No instruction
  case 0x19: // DAD D
    {
      uint32_t res = state->hl + state->de;
      state->hl = res & 0xffff;
      state->cc.cy = ((res & 0xffff0000) != 0);
    }
    break;
  case 0x1a: // LDAX D
    // (A) <- ((rp))
    state->a = state->memory[state->de];
    break;
  case 0x1b: // DCX D
    state->de--;
    break;
  case 0x1c: set_zsp_flags(state, ++state->e); break; // INR E
  case 0x1d: set_zsp_flags(state, --state->e); break; // DCR E
  case 0x1e: opcode_mvi(state, &state->e); break; // MVI E, data
  case 0x1f: // RAR
    // Rotate right through carry
    // (An) <- (An+l); (CY) <- (AO)
    // (A7) <- (CY)
    {
      uint16_t initial = state->a | (state->cc.cy << 8);
      uint16_t res = (initial >> 1) | (initial << 8);
      state->a = res & 0xff;
      state->cc.cy = (res >> 8) & 0x1;
    }
    break;
  case 0x20: unimplemented_instruction(state); break; // TODO: RIM (special)
  case 0x22: // SHLD addr
    // ((byte 3) (byte 2)) <- (L)
    // ((byte 3)(byte 2) + 1) <- (H)

    state->memory[(opcode[2] << 8) | opcode[1]] = state->l;
    state->memory[((opcode[2] << 8) | opcode[1]) + 1] = state->h;

    state->pc += 2;
    break;
  case 0x23: // INX H
    // (rh)(rl) <- (rh)(rl) + 1
    state->hl++;
    break;
  case 0x24: set_zsp_flags(state, ++state->h); break; // INR H
  case 0x25: set_zsp_flags(state, --state->h); break; // DCR H
  case 0x26: opcode_mvi(state, &state->h); break; // MVI H, data 8
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
      uint16_t res = state->a;
      if ((res & 0xf) > 9 || state->cc.ac == 1) {
        res += 6;
      }

      // Step 2
      if (((res & 0xf0) >> 4) > 9 || state->cc.cy == 1) {
        res += (6 << 4);
      }

      state->a = res &0xff;
      set_zsp_flags(state, res);
      state->cc.cy = (res > 0xff);
    }
    break;
  case 0x28: unimplemented_instruction(state); break;  // No instruction
  case 0x29: // DAD H
    {
      uint32_t res = state->hl + state->hl;
      state->hl = res & 0xffff;
      state->cc.cy = ((res & 0xffff0000) != 0);
    }
    break;
  case 0x2a: // LHLD addr
    /*
      (L) <- ((byte3)(byte2))
      (H) <- ((byte3)(byte2) + 1)
    */
    state->l = state->memory[(opcode[2] << 8) | opcode[1]];
    state->h = state->memory[((opcode[2] << 8) | opcode[1]) + 1];
    state->pc += 2;
    break;
  case 0x2b: // DCX H
    state->hl--;
    break;
  case 0x2c: set_zsp_flags(state, ++state->l); break; // INR L
  case 0x2d: set_zsp_flags(state, --state->l); break; // DCR L
  case 0x2e: opcode_mvi(state, &state->l); break; // MVI L, data 8
  case 0x2f: state->a = ~state->a; break; // CMA
  case 0x30: unimplemented_instruction(state); break; // TODO: SIM (special)
  case 0x31: // LXI SP, data 16
    /*
      Load register pair immediate
      Byte 3 of the instruction is moved into the high-order
      register of the register pair. Byte 2 of the instruction
      is moved into the low-order register of the register pair.
    */
    state->sp = (opcode[2] << 8) | opcode[1];
    state->pc += 2;
    break;
  case 0x32: // STA addr
    // ((byte 3)(byte 2)) <- (A)
    state->memory[(opcode[2] << 8) | opcode[1]] = state->a;
    state->pc += 2;
    break;
  case 0x33: state->sp = state->sp + 1; break; // INX SP
  case 0x34: // INR M
    {
      uint8_t res = state->memory[state->hl] + 1;
      set_zsp_flags(state, res);
      state->memory[state->hl] = res;
    }
    break;
  case 0x35: // DCR M
    {
      uint8_t res = state->memory[state->hl] - 1;
      state->memory[state->hl] = res;
      set_zsp_flags(state, res);
    }
    break;
  case 0x36: // MVI M, data 8
    // ((H)(L)) <- (byte 2)
    state->memory[state->hl] = opcode[1];
    state->pc++;
    break;
  case 0x37: state->cc.cy = 1; break; // STC. (CY) <- 1
  case 0x38: unimplemented_instruction(state); break; // No instruction
  case 0x39: // DAD SP
    // Add registerpair to H and L
    // (H)(L) <- (H)(L) + (rh)(rl)
    {
      uint32_t res = state->hl + state->sp;
      state->cc.cy = (res >> 16) & 0x1;
      state->hl = res & 0xffff;
    }
    break;
  case 0x3a: // LDA data 16
    // (A) <- ((byte3)(byte2))
    state->a = state->memory[(opcode[2] << 8) | opcode[1]];
    state->pc += 2;
    break;
  case 0x3b: state->sp--; break; // DCX SP
  case 0x3c: set_zsp_flags(state, ++state->a); break; // INR A
  case 0x3d: set_zsp_flags(state, --state->a); break; // DCR A
  case 0x3e: opcode_mvi(state, &state->a); break;// MVI A, data 8
  case 0x3f: state->cc.cy = (~ state->cc.cy) & 0x1; break; // CMC. (CY) <- ~(CY)
  case 0x40: state->b = state->b; break; // MOV B, B
  case 0x41: state->b = state->c; break; // MOV B, C
  case 0x42: state->b = state->d; break; // MOV B, D
  case 0x43: state->b = state->e; break; // MOV B, E
  case 0x44: state->b = state->h; break; // MOV B, H
  case 0x45: state->b = state->l; break; // MOV B, L
  case 0x46: state->b = state->memory[state->hl]; break; // MOV B, M
  case 0x47: state->b = state->a; break; // MOV B, A
  case 0x48: state->c = state->b; break; // MOV C, B
  case 0x49: state->c = state->c; break; // MOV C, C
  case 0x4a: state->c = state->d; break;  // MOV C, D
  case 0x4b: state->c = state->e; break; // MOV C, E
  case 0x4c: state->c = state->h; break; // MOV C, H
  case 0x4d: state->c = state->l; break; // MOV C, L
  case 0x4e: state->c = state->memory[state->hl]; break; // MOV C, M
  case 0x4f: state->c = state->a; break; // MOV C, A
  case 0x50: state->d = state->b; break; // MOV D, B
  case 0x51: state->d = state->c; break; // MOV D, C
  case 0x52: state->d = state->d; break; // MOV D, D
  case 0x53: state->d = state->e; break; // MOV D, E
  case 0x54: state->d = state->h; break; // MOV D, H
  case 0x55: state->d = state->l; break; // MOV D, L
  case 0x56: state->d = state->memory[state->hl]; break; // MOV D, M
  case 0x57: state->d = state->a; break; // MOV D, A
  case 0x58: state->e = state->b; break; // MOV E, B
  case 0x59: state->e = state->c; break; // MOV E, C
  case 0x5a: state->e = state->d; break; // MOV E, D
  case 0x5b: state->e = state->e; break; // MOV E, E
  case 0x5c: state->e = state->h; break; // MOV E, H
  case 0x5d: state->e = state->l; break; // MOV E, L
  case 0x5e: state->e = state->memory[state->hl]; break; // MOV E, M
  case 0x5f: state->e = state->a; break; // MOV E, A
  case 0x60: state->h = state->b; break; // MOV H, B
  case 0x61: state->h = state->c; break; // MOV H, C
  case 0x62: state->h = state->d; break; // MOV H, D
  case 0x63: state->h = state->e; break; // MOV H, E
  case 0x64: state->h = state->h; break; // MOV H, H
  case 0x65: state->h = state->l; break; // MOV H, L
  case 0x66: state->h = state->memory[state->hl]; break; // MOV H, M
  case 0x67: state->h = state->a; break; // MOV H, A
  case 0x68: state->l = state->b; break; // MOV L, B
  case 0x69: state->l = state->c; break; // MOV L, C
  case 0x6a: state->l = state->d; break; // MOV L, D
  case 0x6b: state->l = state->e; break; // MOV L, E
  case 0x6c: state->l = state->h; break; // MOV L, H
  case 0x6d: state->l = state->l; break; // MOV L, L
  case 0x6e: state->l = state->memory[state->hl]; break; // MOV L, M
  case 0x6f: state->l = state->a; break; // MOV L, A
  case 0x70: state->memory[state->hl] = state->b; break; // MOV M, B
  case 0x71: state->memory[state->hl] = state->d; break; // MOV M, C
  case 0x72: state->memory[state->hl] = state->d; break; // MOV M, D
  case 0x73: state->memory[state->hl] = state->e; break; // MOV M, E
  case 0x74: state->memory[state->hl] = state->h; break; // MOV M, H
  case 0x75: state->memory[state->hl] = state->l; break; // MOV M, L
  case 0x76: exit(EXIT_SUCCESS); break; // TODO: HLT
  case 0x77: state->memory[state->hl] = state->a; break; // MOV M, A
  case 0x78: state->a = state->b; break; // MOV A, B
  case 0x79: state->a = state->c; break; // MOV A, C
  case 0x7a: state->a = state->d; break; // MOV A, D
  case 0x7b: state->a = state->e; break; // MOV A, E
  case 0x7c: state->a = state->h; break; // MOV A, H
  case 0x7d: state->a = state->l; break; // MOV A, L
  case 0x7e: state->a = state->memory[state->hl]; break; // MOV A, M
  case 0x7f: state->a = state->a; break; // MOV A, A
  case 0x80: opcode_add(state, state->b); break; // ADD B
  case 0x81: opcode_add(state, state->c); break; // ADD C
  case 0x82: opcode_add(state, state->d); break; // ADD D
  case 0x83: opcode_add(state, state->e); break; // ADD E
  case 0x84: opcode_add(state, state->h); break; // ADD H
  case 0x85: opcode_add(state, state->l); break; // ADD L
  case 0x86: opcode_add(state, state->memory[state->hl]); break; // ADD M
  case 0x87: opcode_add(state, state->a); break; // ADD A
  case 0x88: opcode_adc(state, state->b); break; // ADC B
  case 0x89: opcode_adc(state, state->c); break; // ADC C
  case 0x8a: opcode_adc(state, state->d); break; // ADC D
  case 0x8b: opcode_adc(state, state->e); break; // ADC E
  case 0x8c: opcode_adc(state, state->h); break; // ADC H
  case 0x8d: opcode_adc(state, state->l); break; // ADC L
  case 0x8e: opcode_adc(state, state->memory[state->hl]); break; // ADC M
  case 0x8f: opcode_adc(state, state->a); break; // ADC A
  case 0x90: opcode_sub(state, state->b); break; // SUB B
  case 0x91: opcode_sub(state, state->c); break; // SUB C
  case 0x92: opcode_sub(state, state->d); break; // SUB D
  case 0x93: opcode_sub(state, state->e); break; // SUB E
  case 0x94: opcode_sub(state, state->h); break; // SUB H
  case 0x95: opcode_sub(state, state->l); break; // SUB L
  case 0x96: opcode_sub(state, state->memory[state->hl]); break; // SUB M
  case 0x97: opcode_sub(state, state->a); break; // SUB A. Return 0?
  case 0x98: opcode_sbb(state, state->b); break; // SBB B
  case 0x99: opcode_sbb(state, state->c); break; // SBB C
  case 0x9a: opcode_sbb(state, state->d); break; // SBB D
  case 0x9b: opcode_sbb(state, state->e); break; // SBB E
  case 0x9c: opcode_sbb(state, state->h); break; // SBB H
  case 0x9d: opcode_sbb(state, state->l); break; // SBB L
  case 0x9e: opcode_sbb(state, state->memory[state->hl]); break; // SBB M
  case 0x9f: opcode_sbb(state, state->a); break; // SBB A. Return 0xff?
  case 0xa0: opcode_ana(state, state->b); break; // ANA B
  case 0xa1: opcode_ana(state, state->c); break; // ANA C
  case 0xa2: opcode_ana(state, state->d); break; // ANA D
  case 0xa3: opcode_ana(state, state->e); break; // ANA E
  case 0xa4: opcode_ana(state, state->h); break; // ANA H
  case 0xa5: opcode_ana(state, state->l); break; // ANA L
  case 0xa6: opcode_ana(state, state->memory[state->hl]); break; // ANA M
  case 0xa7: opcode_ana(state, state->a); break; // ANA A
  case 0xa8: opcode_xra(state, state->b); break; // XRA B
  case 0xa9: opcode_xra(state, state->c); break; // XRA C
  case 0xaa: opcode_xra(state, state->d); break; // XRA D
  case 0xab: opcode_xra(state, state->e); break; // XRA E
  case 0xac: opcode_xra(state, state->h); break; // XRA H
  case 0xad: opcode_xra(state, state->l); break; // XRA L
  case 0xae: opcode_xra(state, state->memory[state->hl]); break; // XRA M
  case 0xaf: opcode_xra(state, state->a); break; // XRA A
  case 0xb0: opcode_ora(state, state->b); break; // ORA B
  case 0xb1: opcode_ora(state, state->c); break; // ORA C
  case 0xb2: opcode_ora(state, state->d); break; // ORA D
  case 0xb3: opcode_ora(state, state->e); break; // ORA E
  case 0xb4: opcode_ora(state, state->h); break; // ORA H
  case 0xb5: opcode_ora(state, state->l); break; // ORA L
  case 0xb6: opcode_ora(state, state->memory[state->hl]); break; // ORA M
  case 0xb7: opcode_ora(state, state->a); break; // ORA A
  case 0xb8: opcode_cmp(state, state->b); break; // CMP B
  case 0xb9: opcode_cmp(state, state->c); break; // CMP C
  case 0xba: opcode_cmp(state, state->d); break; // CMP D
  case 0xbb: opcode_cmp(state, state->e); break; // CMP E
  case 0xbc: opcode_cmp(state, state->h); break; // CMP H
  case 0xbd: opcode_cmp(state, state->l); break; // CMP L
  case 0xbe: opcode_cmp(state, state->memory[state->hl]); break; // CMP M
  case 0xbf: opcode_cmp(state, state->a); break; // CMP A
  case 0xc0: // RNZ
    // Conditional return
    if (state->cc.z == 0) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xc1: opcode_pop(state, &state->b); break; // POP B
  case 0xc2: // JNZ addr
    /*
      Conditional jump
      If (CCC)
      (PC) <- (byte 3)(byte 2)
    */
    if (state->cc.z == 0) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xc3: // JMP addr
    // (PC) <- (byte 3)(byte 2)
    state->pc = (opcode[2] << 8) | opcode[1];
    return 0;
    break;
  case 0xc4: // CNZ addr
    // Condition call
    {
      if (state->cc.z == 0) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xc5: opcode_push(state, &state->b); break; // PUSH B
  case 0xc6: // ADI data 8
    // (A) <- (A) + (byte 2)
    {
      uint16_t res = state->a + opcode[1];
      state->a = res & 0xff;
      set_zsp_flags(state, state->a);
      state->cc.cy = (res > 0xff);
      state->pc++;
    }
    break;
  case 0xc7: unimplemented_instruction(state); break; // TODO: RST 0 (CALL $0)
  case 0xc8: // RZ
    /*
      Return if Z
      if Z == 1
      (PCL) <- ((SP))
      (PCH) <- ((SP) + 1)
      (SP) <- (SP) + 2
    */
    if (state->cc.z == 1) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xc9: // RET
    /*
      Return
      (PCL) <- ((SP))
      (PCH) <- ((SP) + 1)
      (SP) <- (SP) + 2
      The content of the memory location whose address is specified
      in register SP is moved to the low-order eight bits of register PC.
      The content of the memory location whose address is one more than
      the content of register SP is moved to the high-order eight bits
      of register PC. The content of register SP is incremented by 2.
    */
    {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xca: // JZ addr
    // if Z, PC <- adr
    if (state->cc.z == 1) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xcb: unimplemented_instruction(state); break; //  No instruction
  case 0xcc: // CZ addr
    {
      // Condition call
      if (state->cc.z == 1) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xcd: // CALL addr
    /*
      Call
      ((SP) - 1) <- (PCH)
      ((SP) - 2) <- (PCL)
      (SP) <- (SP) - 2
      (PC) <- (byte 3)(byte 2)
      The high-order eight bits of the next instruction addess
      are moved to the memory location whose
      address is one less than the content of register SP.
      The low-order eight bits of the next instruction whose
      address is two less than the content of register SP.
      The content of register SP is decremented by 2. Control
      is trasferred to the instruction whose address is
      specified in byte 3 and byte 2 of the current instruction
      Cycles: 5
      States: 17
      Addressing: immediate/reg. indirect
      Flags: none
    */
#ifdef CPUDIAG_CALL5
    if (5 == ((opcode[2] << 8) | opcode[1])) {
      if (state->c == 9) {
        unsigned char *str = &state->memory[state->de + 3]; // skip the prefix bytes
        //unsigned char *str = &state->memory[state->de];
        printf("'");
        while (*str != '$')
          printf("%c", *str++);
        printf("'\n");
      } else if (state->c == 2) {
        printf("printed char: %c\n", state->e);
      }
      state->pc += 2;
    } else if (0 == ((opcode[2] << 8) | opcode[1])) {
      exit(EXIT_SUCCESS);
    } else
#endif
      {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      }
    break;
  case 0xce: // ACI data
    // (A) <- (A) + (byte 2) + (CY)
    {
      uint16_t res = state->a + opcode[1] + state->cc.cy;
      state->a = res & 0xff;
      set_zsp_flags(state, state->a);
      state->cc.cy = (res > 0xff);
      state->pc += 1;
    }
    break;
  case 0xcf: unimplemented_instruction(state); break; // TODO: RST 1 (CALL $8)
  case 0xd0: // RNC
    // Conditional return
    if (state->cc.cy == 0) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xd1: opcode_pop(state, &state->d); break; // POP D
  case 0xd2: // JNC addr
    // Jump if CY == 0
    if (state->cc.cy == 0) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xd3: // OUT data 8
    /*
      Output
      (data) <- (A)
      The content of register A is placed on the eight bit
      bi-directional data bus for transmission to the specified port
    */
    // TODO
    printf("---- CALLED OUT - DON'T KNOW WHAT TO DO\n");
    state->pc += 1;
    break;
  case 0xd4: // CNC addr
    {
      // Condition call
      if (state->cc.cy == 0) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xd5: opcode_push(state, &state->d); break; // PUSH D
  case 0xd6: // SUI data
    // (A) <- (A) - (byte 2)
    {
      state->cc.cy = (state->a < opcode[1]);
      state->a = state->a - opcode[1];
      set_zsp_flags(state, state->a);
      state->pc++;
    }
    break;
  case 0xd7: unimplemented_instruction(state); break; // TODO: RST 2 (CALL $10)
  case 0xd8: // RC
    // Conditional return
    if (state->cc.cy == 1) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xd9: unimplemented_instruction(state); break; // No instruction
  case 0xda: // JC addr
    // Jump if CY == 1    - checked
    if (state->cc.cy == 1) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xdb: unimplemented_instruction(state); break; // TODO: IN data
  case 0xdc: // CC addr
    /*
      if CY = 1
      ((SP) - 1) <- (PCH)
      ((SP) - 2) <- (PCL)
      (SP) <- (SP) - 2
      (PC) <- (byte3)(byte2)
    */
    if (state->cc.cy == 1) {
      stack_put_addr16(state, state->pc + 3);
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;      
    } else {
      state->pc += 2;
    }
    break;
  case 0xdd: unimplemented_instruction(state); break; // No instruction
  case 0xde: // SBI data
    // (A) <- (A) - (byte2) - (CY)
    {
      state->cc.cy = (state->a < opcode[1]);
      state->a = state->a - opcode[1] - state->cc.cy;
      set_zsp_flags(state, state->a);
      state->pc++;
    }
    break;
  case 0xdf: unimplemented_instruction(state); break; // TODO: RST 3 (CALL $18)
  case 0xe0: // RPO
    // Conditional return
    if (state->cc.p == 0) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xe1: opcode_pop(state, &state->h); break; // POP H
  case 0xe2: // JPO addr
    // Jump if P == 0
    if (state->cc.p == 0) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xe3: // XTHL
    /*
      Exchange stack top with H and L
      (L) <-> ((SP))
      (H) <-> ((SP) + 1)
      The content of the L register is exchanged with the content of the memory location
      whose address is specified by the content of register SP.
      The content of the H register is exchanged with the content of the memory location
      whose address is one more than the content of register SP
    */
    {
      uint8_t l = state->l;
      uint8_t h = state->h;
      state->l = state->memory[state->sp];
      state->memory[state->sp] = l;
      state->h = state->memory[state->sp + 1];
      state->memory[state->sp + 1] = h;
    }
    break;
  case 0xe4: // CPO addr
    {
      // Condition call
      if (state->cc.p == 0) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xe5: opcode_push(state, &state->h); break; // PUSH H
  case 0xe6: // ANI data 8
    // (A) <- (A) & (byte2)
    {
      state->a = state->a & opcode[1];
      set_zsp_flags(state, state->a);
      state->cc.cy = state->cc.ac = 0;            
      state->pc++;
    }
    break;
  case 0xe7: unimplemented_instruction(state); break; // TODO: RST 4 (CALL $20)
  case 0xe8: // RPE
    // Conditional return
    if (state->cc.p == 1) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xe9: // PCHL
    state->pc = state->hl;
    return 0;
    break;
  case 0xea: // JPE addr
    // Jumf if PE == 1
    if (state->cc.p == 1) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xeb: // XCHG
    /* Exchange H and L with D and E
       (H) <-> (D)
       (L) <-> (E) */
    {
      uint16_t temp = state->hl;
      state->hl = state->de;
      state->de = temp;
    }
    break;
  case 0xec: // CPE addr
    // Condition call
    {
      if (state->cc.p == 1) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
    
  case 0xed: // Z80
    z80_opcode_ed(state, opcode[1]);
    break; // No instruction

  case 0xee: // XRI data
    // (A) <- (A) XOR (byte2)
    {
      state->a = state->a ^ opcode[1];
      set_zsp_flags(state, state->a);
      state->cc.cy = state->cc.ac = 0;
      state->pc++;
    }
    break;
  case 0xef: unimplemented_instruction(state); break; // TODO: RST 5 (CALL $28)
  case 0xf0: // RP
    // Conditional return
    if (state->cc.s == 0) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xf1: // POP PSW
    {
      state->psw = state->memory[state->sp++];
      state->a = state->memory[state->sp++];
    }
    break;
  case 0xf2: // JP addr
    // Jump if S == 0
    if (state->cc.s == 0) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xf3: // DI
    // Disable interrupts
    state->int_enable = 0;
    printf("DI called. Dont know what to do now. Skipping\n");
    break;
  case 0xf4: // CP addr
    // Condition call
    {
      if (state->cc.s == 0) {
        stack_put_addr16(state, state->pc + 3);
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xf5: // PUSH PSW
    // Push processor status word
    {
      state->memory[--state->sp] = state->a;
      state->memory[--state->sp] = state->psw;
    }
    break;
  case 0xf6: // ORI data
    // (OR Immediate) (A) <- (A) V (byte 2)
    {
      state->a = state->a | opcode[1];
      set_zsp_flags(state, state->a);
      state->cc.cy = state->cc.ac = 0;
      state->pc++;
    }
    break;
  case 0xf7: unimplemented_instruction(state); break; // TODO: RST 6 (CALL $30)
  case 0xf8: // RM
    // Conditional return
    if (state->cc.s == 1) {
      state->pc = stack_take_addr16(state);
      return 0;
    }
    break;
  case 0xf9: // SPHL
    // (SP) <- (H)(L)
    state->sp = state->hl;
    break;
  case 0xfa: // JM addr
    // Jump if S == 1
    if (state->cc.s == 1) {
      state->pc = (opcode[2] << 8) | opcode[1];
      return 0;
    } else {
      state->pc += 2;
    }
    break;
  case 0xfb: state->int_enable = 1; break; // EI
  case 0xfc: // CM addr
    {
      // Condition call
      if (state->cc.s == 1) {
        stack_put_addr16(state, state->pc + 3); // 3 because we return 0 here
        state->pc = (opcode[2] << 8) | opcode[1];
        return 0;
      } else {
        state->pc += 2;
      }
    }
    break;
  case 0xfd: unimplemented_instruction(state); break; // No instruction
  case 0xfe: // CPI data 8   - checked
    /* Compare immediate
       (A) - (byte 2)
       The content of the second byte of the instruction is substracted
       from accumulator. The condition flags are set by the result of the substraction.
       The Z flag is set to 1 if (A) = (byte 2). The CY flag is set to 1 if (A) < (byte 2). */
    {
      uint8_t res = state->a - opcode[1];
      set_zsp_flags(state, res);
      state->cc.cy = (state->a < opcode[1]);
      state->pc++;
    }
    break;
  case 0xff: unimplemented_instruction(state); break; // RST 7

  default:
    unimplemented_instruction(state);
    break;
  }
  state->pc++;

  return 0;
}
