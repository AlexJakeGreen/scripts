#ifndef H_STATE
#define H_STATE

#include "stdlib.h"

typedef struct {
    uint8_t s:1; // sign
    uint8_t z:1; // zero
    uint8_t pad5:1; // the 5th bit of the last 8-bit instruction that altered flags
    uint8_t h:1; // half carry
    uint8_t pad3:1; // the 3rd bit of the last 8-bit instruction that altered flags
    uint8_t pv:1; // parity/overflow
    uint8_t n:1; // add/substract
    uint8_t c:1; // carry
} condition_codes_t;

typedef struct  __attribute__((__packed__)) {
    union {
        uint16_t register_bc;
        struct {
            uint8_t register_c;
            uint8_t register_b;
        };
    };
    
    union {
        uint16_t register_de;
        struct {
            uint8_t register_e;
            uint8_t register_d;
        };
    };

    union {
        uint16_t register_hl;
        struct {
            uint8_t register_l;
            uint8_t register_h;
        };
    };

    union {
        uint16_t register_af;
        struct {
            union {
                uint8_t register_f;
                condition_codes_t flags;
            };
            uint8_t register_a;
        };
    };

    union {
        uint16_t register_ix;
        struct {
            uint8_t register_ixl;
            uint8_t register_ixh;
        };
    };

    union {
        uint16_t register_iy;
        struct {
            uint8_t register_iyl;
            uint8_t register_iyh;
        };
    };

    union {
        uint16_t register_sp;
        struct {
            uint8_t register_spl;
            uint8_t register_sph;
        };
    };

    union {
        uint16_t register_pc;
        struct {
            uint8_t register_pcl;
            uint8_t register_pch;
        };
    };

    uint8_t *memory;
    
} state_t;

#endif
