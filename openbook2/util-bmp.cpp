/** util-bmp.cpp - BMP utilities **/

#include "openbook2.h"

// Further redeclarations mysteriously required by C++.

void BMP::ImageLoad(char* xiFilename, Texture* xoTexture)
{
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */

    /*  make sure the file is there. */
    if ((file = fopen(xiFilename, "rb"))==NULL) {
      throw "Could not open BMP file. File not found.";
    }

    /*  seek through the bmp header, up to the width height: */
    fseek(file, 18, SEEK_CUR);

    /*  No 100% errorchecking anymore!!! */

    /*  read the width */
    xoTexture->sizeX = getint (file);

    /*  read the height */
    xoTexture->sizeY = getint (file);

    /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
    size = xoTexture->sizeX * xoTexture->sizeY * 3;

    /*  read the planes */
    planes = getshort(file);
    if (planes != 1) {
        throw "Planes from BMP is not 1.";
    }

    /*  read the bpp */
    bpp = getshort(file);
    if (bpp != 24) {
      throw "BPP from BMP is not 24.";
    }

    /*  seek past the rest of the bitmap header. */
    fseek(file, 24, SEEK_CUR);

    /*  read the data.  */
    xoTexture->data = (char *) malloc(size);
    if (xoTexture->data == NULL) {
        throw "Error allocating memory for color-corrected image data.";
    }

    if ((i = fread(xoTexture->data, size, 1, file)) != 1) {
        throw "Error reading image data from BMP.";
    }

    for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
      temp = xoTexture->data[i];
      xoTexture->data[i] = xoTexture->data[i+2];
      xoTexture->data[i+2] = temp;
    }

    fclose(file); /* Close the file and release the filedes */
}

unsigned int BMP::getint(FILE *fp) {
  int c, c1, c2, c3;

  /*  get 4 bytes */
  c = getc(fp);
  c1 = getc(fp);
  c2 = getc(fp);
  c3 = getc(fp);

  return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

unsigned int BMP::getshort(FILE* fp) {
  int c, c1;

  /* get 2 bytes*/
  c = getc(fp);
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}
