
#include "debug.h"

void print_state(state_t *state) {
  printf("\t==AX:0x%04x BX:0x%04x CX:0x%04x DX:0x%04x\n", state->ax, state->bx, state->cx, state->dx);
  printf("\t==IP:0x%04x SP:0x%04x BP:0x%04x SI:0x%04x DI:0x%04x\n", state->ip, state->sp, state->bp, state->si, state->di);
  /* printf("\t==CS:0x%04x DS:0x%04x SS:0x%04x ES:0x%04x\n", state->cs, state->ds, state->ss, state->es); */
  /* printf("\t==cf:%01x pf:%01x af:%01x zf:%01x sf:%01x of:%01x\n", */
  /*        state->flags.cf, state->flags.pf, state->flags.af, state->flags.zf, state->flags.sf, state->flags.of); */
  /* printf("\t==if:%01x df:%01x tf:%01x\n", state->flags._if, state->flags.df, state->flags.tf); */
}

void print_memory(state_t *state) {
  printf("\nRAM dump\n");
  for(int i = 0; i < state->ip; i++) {
    printf("%02x ", state->memory[i]);
    if ((i & 0x0f) == 0x0f)
      printf("\n");
  }
  printf("\n");
}

void unimplemented_instruction(state_t *state) {
  unsigned char opcode = state->memory[state->ip];
  printf("\nError: Unimplemented instruction: %02x\n", opcode);
  printf("--------------------------------------\n");
  print_state(state);
  // print_memory(state);
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
