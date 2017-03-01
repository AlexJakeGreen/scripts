//
//  main.c
//  cengine
//
//  Created by Alexander on 8/30/15.
//  Copyright (c) 2015 Alexander. All rights reserved.
//

#include <stdio.h>
#include "obj_loader.h"

#include "sdl2_init.h"

void draw_cube(float xrf, float yrf, float zrf) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);    // Сдвинуть вглубь экрана
    
    glRotatef(xrf, 1.0f, 0.0f, 0.0f);   // Вращение куба по X, Y, Z
    glRotatef(yrf, 0.0f, 1.0f, 0.0f);   // Вращение куба по X, Y, Z
    glRotatef(zrf, 0.0f, 0.0f, 1.0f);   // Вращение куба по X, Y, Z
    
    glBegin(GL_QUADS);                  // Рисуем куб
    
    glColor3f(0.0f, 1.0f, 0.0f);        // Синяя сторона (Верхняя)
    glVertex3f( 1.0f, 1.0f, -1.0f);     // Верхний правый угол квадрата
    glVertex3f(-1.0f, 1.0f, -1.0f);     // Верхний левый
    glVertex3f(-1.0f, 1.0f,  1.0f);     // Нижний левый
    glVertex3f( 1.0f, 1.0f,  1.0f);     // Нижний правый
    
    glColor3f(1.0f, 0.5f, 0.0f);        // Оранжевая сторона (Нижняя)
    glVertex3f( 1.0f, -1.0f,  1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f, -1.0f,  1.0f);    // Верхний левый
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
    
    glColor3f(1.0f, 0.0f, 0.0f);        // Красная сторона (Передняя)
    glVertex3f( 1.0f,  1.0f, 1.0f);     // Верхний правый угол квадрата
    glVertex3f(-1.0f,  1.0f, 1.0f);     // Верхний левый
    glVertex3f(-1.0f, -1.0f, 1.0f);     // Нижний левый
    glVertex3f( 1.0f, -1.0f, 1.0f);     // Нижний правый
    
    glColor3f(1.0f,1.0f,0.0f);          // Желтая сторона (Задняя)
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Верхний левый
    glVertex3f(-1.0f,  1.0f, -1.0f);    // Нижний левый
    glVertex3f( 1.0f,  1.0f, -1.0f);    // Нижний правый
    
    glColor3f(0.0f,0.0f,1.0f);          // Синяя сторона (Левая)
    glVertex3f(-1.0f,  1.0f,  1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f,  1.0f, -1.0f);    // Верхний левый
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
    glVertex3f(-1.0f, -1.0f,  1.0f);    // Нижний правый
    
    glColor3f(1.0f,0.0f,1.0f);          // Фиолетовая сторона (Правая)
    glVertex3f( 1.0f,  1.0f, -1.0f);    // Верхний правый угол квадрата
    glVertex3f( 1.0f,  1.0f,  1.0f);    // Верхний левый
    glVertex3f( 1.0f, -1.0f,  1.0f);    // Нижний левый
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
    
    glEnd();                            // Закончили квадраты
    
}

void draw_obj(obj_t *obj) {



}

int main(int argc, const char * argv[]) {
    /*
    const char *model_file = "/Users/alex/Documents/data/t34.obj";
    obj_t *ret = obj_load(model_file);
    if (ret == NULL) {
        puts("Model load failed\n");
        exit(EXIT_FAILURE);
    };
    free(ret);
    */
    sdl_setup();
    int running = 1;
    float xrf = 0, yrf = 0, zrf = 0;

    float ax = 0;
    float ay = 0;
    

    while (running == 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            
            switch (event.type) {

                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                            
                        case SDLK_LEFT:
                            ay = - 0.5;
                            break;
                            
                        case SDLK_RIGHT:
                            ay =  0.5;
                            break;
                            
                        case SDLK_UP:
                            ax = - 0.5;
                            break;
                        
                        case SDLK_DOWN:
                            ax = 0.5;
                            break;
                            
                        default:
                            break;
                    };
                    break;

                case SDL_KEYUP:
                    
                    switch (event.key.keysym.sym) {
                            
                        case SDLK_LEFT:
                            ay = 0;
                            break;
                            
                        case SDLK_RIGHT:
                            ay = 0;
                            break;
                            
                        case SDLK_UP:
                            ax = 0;
                            break;
                            
                        case SDLK_DOWN:
                            ax = 0;
                            break;
                            
                        default:
                            break;
                    }


                default:
                    break;
            }
        }
        
        xrf = xrf + ax;
        yrf = yrf + ay;
        //zrf = zrf + 0.5;
        draw_cube(xrf, yrf, zrf);

        draw_hud();
        
        sdl_swap_window();
    }
    
    puts("Exit clean.");
    return 0;
}

