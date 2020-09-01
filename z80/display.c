#include <stdio.h>
#include "display.h"

int display_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL){
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to init renderer: %s\n", SDL_GetError());
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        printf("Failed to init texture: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

void display_draw(uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]) {
    SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void display_destroy() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();
}
