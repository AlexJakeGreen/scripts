//
//  obj_loader.c
//  cengine
//
//  Created by Alexander on 8/30/15.
//  Copyright (c) 2015 Alexander. All rights reserved.
//

#include "obj_loader.h"

obj_t *obj_load(const char *file_name) {

    int fc1, fc2, fc3, ft1, ft2, ft3, fn1, fn2, fn3;
    
    printf("Loading: %s\n", file_name);
    
    obj_t *obj;
    obj = malloc(sizeof(obj_t));
    obj->coords = malloc(sizeof(obj->coords));
    obj->normal = malloc(sizeof(obj->normal));
    obj->faces = malloc(sizeof(obj->faces));
    obj->face_normal = malloc(sizeof(obj->face_normal));
    
    obj->vertex_count = 0;
    obj->normal_count = 0;
    obj->face_normal = 0;
    
        FILE *f;
    char line[2048];
    double x, y, z;
    
    f = fopen(file_name, "r");
    if (f == NULL) {
        printf("Cannot open file %s\n", file_name);
        return NULL;
    }
    
    while (!feof(f)) {
        fgets(line, 2048, f);
        if (strlen(line) < 1)
            continue;
        
        switch (line[0]) {
            case '#': // Comment
                continue;
                break;
            case '\n': // Empty line
                continue;
                break;
            case 'g': // Group
                break;
            case 'v': // Vertex
                if (strncmp(line, "vn ", 3) == 0) {
                    // Vertex normal
                    if (sscanf(line, "vn %lf %lf %lf", &x, &y, &z) == 3) {
                        obj->normal_count++;
                        obj->normal = realloc(obj->normal, sizeof(double) * 3 * (obj->normal_count + 1));
                        obj->normal[obj->normal_count * 3 + 0] = x;
                        obj->normal[obj->normal_count * 3 + 1] = y;
                        obj->normal[obj->normal_count * 3 + 2] = z;

                    } else {
                        puts("Cannot parse normal\n");
                    }
                    
                } else if (strncmp(line, "vt ", 3) == 0) {
                    // Vertex texture

                } else if (sscanf(line, "v %lf %lf %lf", &x, &y, &z) == 3) {
                    // Our data
                    obj->vertex_count++;
                    obj->coords = realloc(obj->coords, sizeof(double) * 3 * (obj->vertex_count + 1));
                    obj->coords[obj->vertex_count * 3 + 0] = x;
                    obj->coords[obj->vertex_count * 3 + 1] = y;
                    obj->coords[obj->vertex_count * 3 + 2] = z;
                    
                } else {
                    printf("Cannot parse line %s\n", line);
                }
                break;
            case 'f': // Face
                if (strncmp(line, "f ", 2) == 0) {
                    if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i",
                               &fc1, &ft1, &fn1, &fc2, &ft2, &fn2, &fc3, &ft3, &fn3) == 9) {
                        
                        obj->face_count++;
                        obj->faces = realloc(obj->faces, sizeof(int) * 3 * (obj->face_count + 1));
                        obj->faces[obj->face_count * 3 + 0 - 3] = fc1;
                        obj->faces[obj->face_count * 3 + 1 - 3] = fc2;
                        obj->faces[obj->face_count * 3 + 2 - 3] = fc3;
                        
                        obj->face_normal = realloc(obj->face_normal, sizeof(int) * 3 * obj->face_count);
                        obj->face_normal[obj->face_count * 3 + 0 - 3] = fn1;
                        obj->face_normal[obj->face_count * 3 + 1 - 3] = fn2;
                        obj->face_normal[obj->face_count * 3 + 2 - 3] = fn3;
                    } else {
                        puts("Cannot parse a face\n");
                    };
                } else {
                    printf("Cannot parse line %s\n", line);
                    continue;
                }
                break;
            case 's': // Don't know
                break;
            case 'u': // Don't know
                break;
            case 'm': // mtllib?
                break;
            default:
                printf("Unknown line %s\n", line);
                break;
        }
    }
    
    printf("Statistics:\nVertex: %i\nNormals: %i\nFaces: %i\n",
           obj->vertex_count, obj->normal_count, obj->face_count);
    
    return obj;
};
