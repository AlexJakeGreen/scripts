#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emulator.h"

#define RAM_SIZE 0xffff // 64k RAM

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("Usage: ./test <hex_code>\n");
        exit(EXIT_FAILURE);
    }

    printf("===================\n");
    
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    char filename[255];
    strcpy(filename, "tests/opcodes/");
    strcat(filename, argv[1]);
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    state_t *state = calloc(1, sizeof(state_t));
    state->memory = calloc(1, RAM_SIZE);
    
    // op code
    char opcode[8];
    getline(&line, &len, fp);
    strcpy(opcode, line);
    /* printf("op code: %s", opcode); */

    // main registers
    getline(&line, &len, fp);

    unsigned int af, bc, de, hl, af_, bc_, de_, hl_, ix, iy, sp, pc;
    sscanf(line, "%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
           &af, &bc, &de, &hl, &af_, &bc_, &de_, &hl_, &ix, &iy, &sp, &pc);

    // set registers
    state->r_af = af; state->r_bc = bc; state->r_de = de; state->r_hl = hl;
    state->_r_af = af_; state->_r_bc = bc_; state->_r_de = de_; state->_r_hl = hl_;
    state->r_ix = ix; state->r_iy = iy; state->r_sp = sp; state->r_pc = pc;

    printf("%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
           state->r_af, state->r_bc, state->r_de, state->r_hl,
           state->_r_af, state->_r_bc, state->_r_de, state->_r_hl,
           state->r_ix, state->r_iy, state->r_sp, state->r_pc);

    
    // other registers - skip them
    getline(&line, &len, fp);

    // memory
    uint16_t addr;
    getline(&line, &len, fp);
    sscanf(line, "%4hx ", &addr);
    /* printf("start addr: %04x\n", addr); */

    int count = 0;
    for (int i=5; i<(int)strlen(line)-4; i+=3) {
        
        uint8_t code = (((line[i]<97)?line[i]-48:line[i]-87)<<4) + ((line[i+1]<97)?line[i+1]-48:line[i+1]-87);

        /* printf("mem[%04x + %02x] = %02x (%c%c)\n", addr, count, code, line[i], line[i+1]); */
        state->memory[addr + count] = code;
        count++;
    }

    getline(&line, &len, fp);
    uint16_t addr2 = 0;
    int count2 = 0;
    /* printf("LINE:%s", line); */
    if (line[0] != '-') {
        sscanf(line, "%4hx ", &addr2);
        /* printf("start addr2: %04x\n", addr2); */

        for (int i=5; i<(int)strlen(line)-4; i+=3) {
            uint8_t code = (((line[i]<97)?line[i]-48:line[i]-87)<<4) + ((line[i+1]<97)?line[i+1]-48:line[i+1]-87);

            /* printf("mem[%04x + %02x] = %02x (%c%c)\n", addr2, count2, code, line[i], line[i+1]); */
            state->memory[addr2 + count2] = code;
            count2++;
        }
    }
    
    while (state->r_pc < (addr + count)) {
        emulate_op(state);
    }
    
    printf("===================\n");
    printf("%s", opcode);

    for (int i=0; i<count; i++) {
        printf("%04x %02x\n", i, state->memory[i]);
    }

    for (int i=count; i<0xffff; i++) {
        if (state->memory[i])
            printf("%04x %02x\n", i, state->memory[i]);
    }
    
    printf("%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
           state->r_af, state->r_bc, state->r_de, state->r_hl,
           state->_r_af, state->_r_bc, state->_r_de, state->_r_hl,
           state->r_ix, state->r_iy, state->r_sp, state->r_pc);

    uint8_t n = state->r_f;
    char str[9] = "00000000";

    for (int i=0; i<8; i++) {
        if (n & 0x01)
            // printf("1");
            str[7-i] = '1';
        else
            //printf("0");
            str[7-i] = '0';
        n >>=1;
    }
    printf("Flags: %s\n       sz h pnc\n", str);
    
    fclose(fp);
    if (line)
        free(line);

    free(state->memory);
    free(state);

  return 0;
}
