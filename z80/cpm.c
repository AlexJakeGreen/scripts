#include <stdio.h>
#include "cpm.h"

void cpm_bdos(state_t *state) {
    if (state->r_c == 9) {
        //printf("> ");
        unsigned char *str = &state->memory[state->r_de];
        while (*str != '$')
            printf("%c", *str++);
        //printf("\n");
    } else if (state->r_c == 2) {
        /* printf("printed char: %c\n", state->r_e); */
        printf("%c", state->r_e);
    }
}
