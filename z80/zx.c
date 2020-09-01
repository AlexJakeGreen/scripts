#include <stdio.h>
#include "display.h"
#include "emulator.h"
#include "utils.h"

#define DISPLAY_PIXELS_START 0x4000
#define DISPLAY_PIXELS_END 0x57FF
#define DISPLAY_ATTRS_START 0x5800
#define DISPLAY_ATTRS_END 0x5AFF

uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
uint8_t buf[DISPLAY_ATTRS_END - DISPLAY_PIXELS_START + 1];

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./test <hex_code>\n");
    exit(EXIT_FAILURE);
  }


  uint8_t data[0xffff];
    int fsize = read_rom_file(data, argv[1]);

    uint16_t rom_offset = 0x0000;
    em_init(data, fsize, rom_offset);

    display_init();

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    /* for(uint16_t i=DISPLAY_PIXELS_START; i<DISPLAY_PIXELS_END; i++ ) { */
    /*     em_setmem(i, 0xaa); */
    /* } */

    /* int j = DISPLAY_PIXELS_START; */
    /* for(int i=0;i<12;i++) { */
    /*     for (int k=0; k<256; k++) { */
    /*         em_setmem(j+k, 0xaa); */
    /*     } */

    /*     for (int k=0; k<256; k++) { */
    /*         em_setmem(j+k+256,0x55); */
    /*     } */
    /*     j+=512; */
    /* } */
    /* for (int i=0;i<768;i++) { */
    /*     zxmem[j+i]=i&127; */
    /* } */

    int s = 0;
    while(running) {
        if (emulate_op() != 0)
            running = SDL_FALSE;
        s++;

        if (s > 10000) {
            s = 0;
            em_get_mem(buf, DISPLAY_PIXELS_START, DISPLAY_ATTRS_END);

            /* for (int k=0; k < DISPLAY_PIXELS_END - DISPLAY_PIXELS_START; k++) { */
            /*     if(buf[k]) { */
            /*         printf("not zero!\n"); */
            /*         break; */
            /*     }     */
            /* } */

            uint8_t yp, pix, attr;
            uint8_t ink = 0;
            uint8_t paper = 0;
            uint8_t __attribute((unused)) flash = 0;
            uint8_t bright = 0;
            uint16_t pixel_addr, buf_addr, attr_addr;
            attr_addr = 6144;
            // Draw all rows 0 - 23
            for(int y=0; y<24;y++) {

                // yp is y in pixels
                yp=y*8;

                // Calculate pixel address offset from the x/y 8x8 block we want to draw.
                pixel_addr = ((yp>>3)<<5)&0xFF;
                pixel_addr += ((yp&0x7)+(((yp>>6)&0x3)<<3))<<8;

                // Draw each 8x8 block 0 - 31
                for (int x=0;x<32;x++ ) {

                    // Buf Addr is the first pixel we want to draw on
                    buf_addr = (y*8)*256+x*8;
                    attr = buf[attr_addr];
                    ink = attr & 0x7;
                    paper = (attr >> 3) &0x7;
                    bright = (attr >>6) & 0x1;
                    if(bright) {
                        ink+=8;
                        paper+=8;
                    }

                    flash = (attr & 0x80);
                    
                    for(int j=0;j<8;j++) {
                            // Read spectrum ram where pixel data lives
                            pix = buf[pixel_addr+j*256];

                            // plot each pixel as ink or paper
                            for(int i=7;i>=0;i--) {
                                pixels[buf_addr]=(pix&1<<i)?ink*0xffffff:paper*0xffffff;
                                buf_addr++;
                            }
                            // move buffer back 
                            buf_addr-=8;
                            
                            // move to next line
                            buf_addr+=256;
                    }
                    
                    // move pixel byte along
                    pixel_addr++;
                    attr_addr++;
                }
                
            }
            /* int p = 0; */
            /* for (int y=0; y<8; y++) { */
            /*     for(int x=0; x<32; x++) { */
            /*         pixels[p]   = (buf[x + y*32*8*8] & 0b00000001) != 0 ? 0xffffff : 0; */
            /*         pixels[p+1] = (buf[x + y*32*8*8] & 0b00000010) != 0 ? 0xffffff : 0; */
            /*         pixels[p+2] = (buf[x + y*32*8*8] & 0b00000100) != 0 ? 0xffffff : 0; */
            /*         pixels[p+3] = (buf[x + y*32*8*8] & 0b00001000) != 0 ? 0xffffff : 0; */
            /*         pixels[p+4] = (buf[x + y*32*8*8] & 0b00010000) != 0 ? 0xffffff : 0; */
            /*         pixels[p+5] = (buf[x + y*32*8*8] & 0b00100000) != 0 ? 0xffffff : 0; */
            /*         pixels[p+6] = (buf[x + y*32*8*8] & 0b01000000) != 0 ? 0xffffff : 0; */
            /*         pixels[p+7] = (buf[x + y*32*8*8] & 0b10000000) != 0 ? 0xffffff : 0; */
            /*         p += 8; */
                
            /*     } */
            /* } */
            display_draw(pixels);
        }
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                {
                    running = SDL_FALSE;
                }
                break;
            }
        }
    }

    display_destroy();
    return 0;
}
