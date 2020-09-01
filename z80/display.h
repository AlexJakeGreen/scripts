#include <SDL2/SDL.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192

// cc -lSDL2 display.c && ./a.out

SDL_Window *win;
SDL_Renderer* renderer;
SDL_Texture* texture;

int display_init();
void display_draw(uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]);
void display_destroy();
