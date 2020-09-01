
#include "debug.h"

void print_state(state_t *state) {
  printf("\t AX:0x%04x BX:0x%04x CX:0x%04x DX:0x%04x\n", state->ax, state->bx, state->cx, state->dx);
  printf("\t SP:0x%04x BP:0x%04x SI:0x%04x DI:0x%04x\n", state->sp, state->bp, state->si, state->di);
  printf("\t CS:0x%04x DS:0x%04x SS:0x%04x ES:0x%04x\n", state->cs, state->ds, state->ss, state->es);

  // printf("\t IP: %04x\n", state->ip);

  printf("\tFLAGS: ____odit sz_a_p_c\n");
  printf("\tFLAGS: ____%x%x%x%x %x%x_%x_%x_%x %04x\n", state->flag.o, state->flag.d, state->flag.i,
         state->flag.t, state->flag.s, state->flag.z, state->flag.a, state->flag.p, state->flag.c, state->flags);
}

void print_memory(state_t *state) {
  int x, y;
  int video_ram = 0x8000;
  printf("\nRAM dump\n");
  for(y = 0; y < 25; y++) {
    for(x = 0; x < 80; x++) {
      if(state->memory[video_ram + x + y * 80] == 0) {
        printf(" ");
      } else {
        printf("%c", (char)state->memory[video_ram + x + y * 80]);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void unimplemented_instruction(state_t *state) {
  unsigned char opcode = state->memory[state->ip];
  printf("\nError: Unimplemented instruction: %02x\n", opcode);
  printf("--------------------------------------\n");
  print_state(state);
  print_memory(state);
  // disassemble_opcode_8086(state, state->ip);
  exit(EXIT_FAILURE);
}

void unimplemented_instruction_detailed(state_t *state, int line, const char *func) {
  unsigned char opcode = state->memory[state->ip];
  printf("\nError: Unimplemented instruction: %02x Line: %d, function: %s\n", opcode, line, func);
  printf("--------------------------------------\n");
  print_state(state);
  // print_memory(state);
  // disassemble_opcode_8086(state, state->ip);
  exit(EXIT_FAILURE);
}
