/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-textures.c: vis library functions for handling textures
 *
 */

#include "vis.h"

void pattern_rectangle (float pw, int psx, float ph, int psy, int txt) {
     glPushMatrix(); /* as received */

     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, texture[txt]);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

     glScalef(pw, ph, 0.0);
     glBegin(GL_QUADS);
       glNormal3f   (0.0, 1.0, 0.0);
           
       glTexCoord2f (0.0, (float)psy);
       glVertex3f   (0.0, 0.0, 0.0);   
       glTexCoord2f ((float)psx, (float)psy);
       glVertex3f   (1.0, 0.0, 0.0);
       glTexCoord2f ((float)psx, 0.0);
       glVertex3f   (1.0, -1.0, 0.0);
       glTexCoord2f (0.0, 0.0);
       glVertex3f   (0.0, -1.0, 0.0);
     glEnd();           
    
     glDisable(GL_TEXTURE_2D);
     glPopMatrix(); /* as received */
} /* pattern_rectangle */

static unsigned int getint(FILE *fp) {
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

static unsigned int getshort(FILE* fp) {
  int c, c1;
  
  /* get 2 bytes*/
  c = getc(fp);  
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */

    /*  make sure the file is there. */
    if ((file = fopen(filename, "rb"))==NULL) {
      fprintf(debug, "File Not Found : %s\n",filename);
      return 0;
    }
    
    /*  seek through the bmp header, up to the width height: */
    fseek(file, 18, SEEK_CUR);

    /*  No 100% errorchecking anymore!!! */

    /*  read the width */    image->sizeX = getint (file);
    
    /*  read the height */ 
    image->sizeY = getint (file);
    
    /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
    size = image->sizeX * image->sizeY * 3;

    /*  read the planes */
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    /*  read the bpp */ 
    bpp = getshort(file);
    if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
    }
	
    /*  seek past the rest of the bitmap header. */
    fseek(file, 24, SEEK_CUR);

    /*  read the data.  */
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
      temp = image->data[i];
      image->data[i] = image->data[i+2];
      image->data[i+2] = temp;
    }

    fclose(file); /* Close the file and release the filedes */

    /*  we're done. */
    return 1;
}

void vis_load_images() {

     Image *TextureImage;
     ImageSet *TextureImageSet;
     int count;
    
     TextureImage = (Image *) malloc(sizeof(Image));
	 if (TextureImage == NULL) {
        printf("Error allocating space for image");
        exit(1);
     } 
     
     TextureImageSet = (ImageSet *) malloc(sizeof(ImageSet));
	 if (TextureImageSet == NULL) {
        printf("Error allocating space for image");
        exit(1);
     }
     
	 ImageLoad("images/page_texture.bmp", TextureImage);
	 TextureImageSet->data[0] = TextureImage->data;
	 TextureImageSet->sizeX[0] = TextureImage->sizeX;
	 TextureImageSet->sizeY[0] = TextureImage->sizeY;
	 
     for ( count=0;count<NUMTEXTURES;count++) {
		TextureImage->data = TextureImageSet->data[count];
		TextureImage->sizeX = TextureImageSet->sizeX[count];
		TextureImage->sizeY = TextureImageSet->sizeY[count];

		glGenTextures (1, &texture[count]);	
		glBindTexture (GL_TEXTURE_2D, texture[count]);
		glTexImage2D  (GL_TEXTURE_2D, 0, 3,
                      TextureImage->sizeX,
                      TextureImage->sizeY,
                      0, GL_RGB, GL_UNSIGNED_BYTE,
                      TextureImage->data);
                      
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* Create The Texture */
	 }
	 
     /* clean up */
     free (TextureImage->data);
     free( TextureImage );
}
