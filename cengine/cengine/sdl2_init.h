//
//  sdl2_init.h
//  cengine
//
//  Created by Alexander on 8/30/15.
//  Copyright (c) 2015 Alexander. All rights reserved.
//

#ifndef __cengine__sdl2_init__
#define __cengine__sdl2_init__

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#include <time.h>

//int frames = 0;
//time_t last_time = 0;

void sdl_setup();
void sdl_destroy();
void sdl_swap_window();
void draw_hud();

#endif /* defined(__cengine__sdl2_init__) */
