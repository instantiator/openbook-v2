/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-textures.h: some definitions for handling textures
 *
 */

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;

struct ImageSet {
	unsigned long sizeX[21];
	unsigned long sizeY[21];
	char *data[21];
};

typedef struct ImageSet ImageSet;

#define NUMTEXTURES 1
GLuint	texture[NUMTEXTURES];

void vis_load_images();
void pattern_rectangle(float, int, float, int, int);
