
#include "state.h"

void em_init(uint8_t *data, int size, uint16_t offset);
int emulate_op();
void em_setmem(uint16_t addr, uint8_t val);
state_t *em_get_state();
