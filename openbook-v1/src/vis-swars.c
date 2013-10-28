/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-swars.c: functions for starwars-style text rendering
 *
 */

#include <GL/glut.h>
#include "vis.h"

float play_left, play_far, play_char_scale, play_line_height;
void* play_font = GLUT_STROKE_ROMAN;
extern float play_width, play_height;

static void render_rendering_space();

typedef unsigned short cell; 

void init_render_swars(float swars_w, float swars_h) {

  gluLookAt(0.0, 0.0, 2.0,       /* standing at 0,0,4 */
            0.0, 0.0, 0.0,       /* looking at 0,0,0 */
            0.0, 1.0, 0.0);      /* up is 0,1,0 */

  glTranslated (0.0,0.0,-6.0); /* back */
  glTranslated (0.0,-3.9, 0.0); /* down a bit */
  glRotated (-30.0,1.0,0.0,0.0); /* turn */

  glTranslated(-swars_w/2,swars_h,0.0);

}

int swars_charwidth(char c) {
   return (int)glutStrokeWidth(play_font, c);
}

void render_swars() {
  glPushMatrix(); /* as received */

  render_rendering_space();
  
  zword r,c; cell a;
    
  for (r = 0; r < h_screen_rows; r++) {

      glPopMatrix();                  /* restore previous position */
      glTranslated(0.0,-play_line_height,0.0);     /* shift down 1 line */
      glPushMatrix();                 /* pre-line push */
      glScalef(play_char_scale, play_char_scale, play_char_scale);  /* drastically scale down text */

      for (c = 0; c < h_screen_cols; c++) {
          /* code to display text for each entry */
          /* vis_debug_print_c(dumb_row(r)[c]); */

          a = cell_char(dumb_cell_val(r, c));
          vis_debug_print_c(a);
          glColor3f(1.0,1.0,1.0);
          glLineWidth(1.0);

          glutStrokeCharacter(play_font, a);
            
      }
      vis_debug_print_c('\n');
   
  }

  glPopMatrix(); /* as received */       

}

static void render_rendering_space() {
     vis_debug('i', "Rendering render space\n");
     
     glPushMatrix(); /* as received */
                     /* we are in TOP, LEFT corner, just above the page */

          glLineWidth(1.0);

          glTranslatef(0.0, 0.0, -0.004); /* negative-z is BEHIND the page */
                                          /* ie. step back to render book itself */

         /* glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); */
         glScalef(play_width * 1.02, play_height * 1.02, 1.0); /* scale up box */

         glPushMatrix(); /* at point of page top-left corner */
         glBegin(GL_LINES);
           glColor3f(0.8,0.8,0.8);
           
           glNormal3f(0.0, 0.0, 1.0);       /* normal is UP out of page */
           
           /* glTexCoord2f (0.0, 1.0);        /* left, far (texture) */
           glVertex3f   (0.0, 0.0, 0.0);   /* left, far (3d) */
           glVertex3f   (1.0, 0.0, 0.0);   /* right, far (3d) */
                      
           /* glTexCoord2f (1.0, 1.0);        /* right, far (texture) */
           glVertex3f   (1.0, 0.0, 0.0);   /* right, far (3d) */
           glVertex3f   (1.0, -1.0, 0.0);   /* right, near (3d) */           

           /* glTexCoord2f (1.0, 0.0);        /* right, near (texture) */
           glVertex3f   (1.0, -1.0, 0.0);   /* right, near (3d) */
           glVertex3f   (0.0, -1.0, 0.0);   /* left, near (3d) */
                      
           /* glTexCoord2f (0.0, 0.0);        /* left, near (texture) */
           glVertex3f   (0.0, -1.0, 0.0);   /* left, near (3d) */
           glVertex3f   (0.0, 0.0, 0.0);   /* left, far (3d) */
           
         glEnd();
         
         glPopMatrix(); /* return to point of page top-left corner */
         
         glTranslatef(0.01, -0.01, 0.004); /* IN to page by margin and up a little */
         
         glBegin(GL_QUADS);
           glColor3f(0.5,0.5,0.5);
           glVertex3f   (0.0, 0.0, 0.0);   /* left, far (3d) */
           glVertex3f   (1.0, 0.0, 0.0);   /* right, far (3d) */
           glVertex3f   (1.0, -1.0, 0.0);   /* right, near (3d) */           
           glVertex3f   (0.0, -1.0, 0.0);   /* left, near (3d) */
         glEnd();           
 
     glPopMatrix(); /* as received */
}
