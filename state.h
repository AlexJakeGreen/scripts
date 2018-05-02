#ifndef H_STATE
#define H_STATE

#include "stdlib.h"

typedef struct {
    uint8_t c:1; // carry bit0
    uint8_t n:1; // add/substract
    uint8_t pv:1; // parity/overflow
    uint8_t f3:1; // the 3rd bit of the last 8-bit instruction that altered flags
    uint8_t h:1; // half carry
    uint8_t f5:1; // the 5th bit of the last 8-bit instruction that altered flags
    uint8_t z:1; // zero
    uint8_t s:1; // sign
} condition_codes_t;

typedef struct  __attribute__((__packed__)) {
    union {
        uint16_t r_bc;
        struct {
            uint8_t r_c;
            uint8_t r_b;
        };
    };
    
    union {
        uint16_t _r_bc;
        struct {
            uint8_t _r_c;
            uint8_t _r_b;
        };
    };

    union {
        uint16_t r_de;
        struct {
            uint8_t r_e;
            uint8_t r_d;
        };
    };

    union {
        uint16_t _r_de;
        struct {
            uint8_t _r_e;
            uint8_t _r_d;
        };
    };

    
    union {
        uint16_t r_hl;
        struct {
            uint8_t r_l;
            uint8_t r_h;
        };
    };

    union {
        uint16_t _r_hl;
        struct {
            uint8_t _r_l;
            uint8_t _r_h;
        };
    };
    
    union {
        uint16_t r_af;
        struct {
            union {
                uint8_t r_f;
                condition_codes_t flags;
            };
            uint8_t r_a;
        };
    };

    union {
      uint16_t _r_af;
      struct {
        union {
          uint8_t _r_f;
          condition_codes_t _flags;
        };
        uint8_t _r_a;
      };
    };

    
    union {
        uint16_t r_ix;
        struct {
            uint8_t r_ixl;
            uint8_t r_ixh;
        };
    };

    union {
        uint16_t r_iy;
        struct {
            uint8_t r_iyl;
            uint8_t r_iyh;
        };
    };

    union {
        uint16_t r_sp;
        struct {
            uint8_t r_spl;
            uint8_t r_sph;
        };
    };

    union {
        uint16_t r_pc;
        struct {
            uint8_t r_pcl;
            uint8_t r_pch;
        };
    };

    uint8_t *memory;
    
} state_t;

#endif
