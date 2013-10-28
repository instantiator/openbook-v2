/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis.h: vis library interface and internal shared functions
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/freeglut.h>

#include "dumb-frotz.h"
#include "vis-textures.h"

#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE 200
#endif

extern FILE* debug;

int vis_init();
void vis_shutdown();

FILE* debug;

void vis_init_debug();
void vis_debug(char, char*);
void vis_debug_print_c(char);
void vis_debug_print_s(char*);
void vis_close_debug();

int vis_get_char_blocking();
void vis_post_redisplay();

/* void render_swars();
void init_render_swars(float swars_w, float swars_h); */

void book_init();
void book_render();
