#include <stdio.h>
#include "cpm.h"

void cpm_bdos(state_t *state) {
    if (state->register_c == 9) {
        //printf("> ");
        unsigned char *str = &state->memory[state->register_de];
        while (*str != '$')
            printf("%c", *str++);
        //printf("\n");
    } else if (state->register_c == 2) {
        /* printf("printed char: %c\n", state->register_e); */
        printf("%c", state->register_e);
    }
}
