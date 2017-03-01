//
//  obj_loader.h
//  cengine
//
//  Created by Alexander on 8/30/15.
//  Copyright (c) 2015 Alexander. All rights reserved.
//

#ifndef __cengine__obj_loader__
#define __cengine__obj_loader__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int vertex_count;
    int normal_count;
    int face_count;
    double *coords;
    double *normal;
    int *faces;
    int *face_normal;
} obj_t;

obj_t *obj_load(const char *file_name);

#endif /* defined(__cengine__obj_loader__) */
