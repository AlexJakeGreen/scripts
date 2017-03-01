//
//  sdl2_init.c
//  cengine
//
//  Created by Alexander on 8/30/15.
//  Copyright (c) 2015 Alexander. All rights reserved.
//
#include "sdl2_init.h"
#include "chars.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int frames = 0;
static int fps = 0;
time_t last_time = 0;

SDL_Window* window = NULL;

void sdl_setup() {
    if(SDL_Init(SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    };
    
    atexit(sdl_destroy);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);

    window = SDL_CreateWindow( "SDL Tutorial",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN );
    if( window == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    };

    SDL_GLContext glcontext __attribute__((unused)) = SDL_GL_CreateContext(window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // устанавливаем фоновый цвет на черный
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST); // включаем тест глубины
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    gluPerspective(45.0f,
                   (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
                   0.1f, 100.0f); // настраиваем трехмерную перспективу
#pragma GCC diagnostic pop
    
    glMatrixMode(GL_MODELVIEW); // переходим в трехмерный режим
    
};


void sdl_destroy() {
    SDL_DestroyWindow(window);
    SDL_Quit();
};

void sdl_swap_window() {
    glFlush();
    SDL_GL_SwapWindow(window);
}

int get_fps() {
    frames++;
    time_t current = time(0);
    int delta = (int)(current - last_time);
    if (delta >= 3) {
        fps = (int)(frames/delta);
        last_time = current;
        frames = 0;
    }
    return fps;
    
}


void draw_hud() {
    char str[3];
    sprintf(str, "%d", get_fps());
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 300, 300, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
/*    glColor3f(1, 0, 0);
    glVertex2f(10, 10);
    glVertex2f(100, 10);
    glVertex2f(100, 30);
    glVertex2f(10, 30);  */
    glEnd();
    
    // Text goes here
    glPixelTransferi(GL_UNPACK_ALIGNMENT, 4);
    glPixelTransferi(GL_INDEX_OFFSET, 1);
    glColor3f(0.0, 1.0, 1.0);
    for (int i = 0; i < strlen(str); i++) {
        glRasterPos2i(15 + 4 * i, 10);
        glBitmap(8, 8, 8.0, 8.0, 0.0, 0.0, chars[(int)str[i] - 48]);
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
}
