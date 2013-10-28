/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-book.c: functions for text and book rendering
 *
 */

#include "vis.h"

typedef unsigned short cell; 

/* state variables describing the book */

/* location of the 'page spine', far point */
float page_left, page_far, page_width, page_height;
float page_angle, other_page_angle;

/* information about the text */
float play_char_scale, play_line_height, play_lines;
void* play_font = GLUT_STROKE_ROMAN;

/* page size */
float play_width, play_height, play_margin;
float bind_margin;

/* book size */
float book_width, book_height, book_depth;

int page_segments_x = 12;
int page_segments_y = 18;

/* Establish variables and sizes of parts of play */
void book_init() {

     page_angle = 5.0;
     other_page_angle = 15.0;

     play_char_scale = 0.0025;
     play_line_height = 0.45;
     play_lines = 24;
     play_margin = 0.2;
     bind_margin = 0.1;

     play_width = 10.0;
     play_height = play_line_height * play_lines;

     page_width = play_width + (play_margin*2);
     page_height = play_height + (play_margin*2);

     book_width = page_width + (play_margin*2);
     book_height = page_height + (play_margin*2);
     book_depth = play_margin * 2;
       
     /* scale up user_screen_width (character widths are like 80!) */
     extern int user_screen_width, user_screen_height;
     user_screen_width = (int) (play_width) / (play_char_scale);
     /*      user_screen_width = (int) (play_width) / play_char_scale); */
     /* user_screen_width = (int) (play_width / (vis_get_char_width(' ')*play_char_scale)); */

     /*
     extern zword h_screen_cols;
     h_screen_cols = (int) (play_width / (vis_get_char_width(' ')*play_char_scale));
     */ /* doesn't work because h_screen_cols reset elsewhere */
     
     user_screen_height = (int) (play_height / play_line_height);

     page_left = -play_width/2;
     page_far = play_height;
     
}

void book_render_text() {
  glPushMatrix(); /* as received */

  zword r,c,s; cell a; /* r,c,a: rows and colums, single char buffer */
  glColor4f (0.0, 0.0, 0.2, 1.0);
  /*glLineWidth (1.0); */
  
  glTranslatef(play_margin, -(play_line_height+play_margin), 0.05); /* raise, centre */
  glPushMatrix();                                 /* and push */

  fprintf(debug, "DISPLAY:\n");      
  for (r = 0; r < h_screen_rows; r++) {
      glPopMatrix();                                 /* restore corner */
      glPushMatrix();                                /* whack it back in */
      glTranslated(0.0, -play_line_height * r, 0.0); /* shift down r lines */

      glScalef(play_char_scale, play_char_scale, play_char_scale);  /* scale down text */
      for (c = 0; c < h_screen_cols; c++) {
          a = cell_char(dumb_cell_val(r, c));
          s = cell_style(dumb_cell_val(r,c));
          if (s == REVERSE_STYLE)
              glLineWidth (2.5);
          else
              glLineWidth (1.5);
                                 
          glutStrokeCharacter(play_font, a);
          fprintf(debug, "%c", a);
      } /* column */
      fprintf(debug, "\n");
  } /* row */
  
  glPopMatrix(); /* that corner again */
  glPopMatrix(); /* as received */       
}

void book_render_page() {
     glPushMatrix(); /* as received, far spine */

         /* page itself */
         glColor4f(1.0, 1.0, 1.0, 1.0); /* page is white */
         pattern_rectangle (page_width, page_segments_x,
                           page_height, page_segments_y,
                           0 /*texture*/);

         /* binding */
         glColor4f(0.4, 0.0, 0.1, 1.0);
         glPushMatrix(); /* before binding */
         glTranslatef (-bind_margin, bind_margin, -book_depth);
         pattern_rectangle (page_width + (bind_margin*2),
                            page_segments_x,
                            page_height + (bind_margin*2),
                            page_segments_y,
                            0);
         glPopMatrix(); /* before binding */                            
         
         /* bars */         
         glColor4f(0.5,0.5,0.5, 1.0);
         glRotatef(90.0, 1.0, 0.0, 0.0);

         /* rear bar */
         pattern_rectangle (page_width, page_segments_x, book_depth, 1, 0);

         /* front bar */
         glTranslatef(0.0, 0.0, page_height);
         pattern_rectangle (page_width, page_segments_x, book_depth, 1, 0);

         /* left bar */
         glRotatef(90.0, 0.0, 1.0, 0.0);
         pattern_rectangle (page_height, page_segments_y, book_depth, 1, 0);
         
         /* right bar */
         glTranslatef(0.0, 0.0, page_width);
         pattern_rectangle (page_height, page_segments_y, book_depth, 1, 0);         
       
     glPopMatrix(); /* as received */
} /* book_render_page */

void book_std_position() {
     glLoadIdentity();                              /* ORIGIN */
     glTranslatef (0.0, 0.0,-8.0);                  /* back */
     glTranslatef (0.0,-3.9, 0.0);                  /* down a bit */
     glRotatef  (-30.0, 1.0, 0.0, 0.0);             /* rotate plane */
     glTranslatef(-book_width/2, book_height, 0.0); /* move to far spine */    
} /* book_std_position */

void book_render() {
     book_std_position(); /* set origin to far spine */
     glRotatef(-page_angle, 0.0, 1.0, 0.0);
     book_render_page();

     book_std_position(); /* set origin to far spine */
     glRotatef(-page_angle, 0.0, 1.0, 0.0);
     book_render_text();

     book_std_position(); /* set origin to far spine */
     glRotatef (other_page_angle, 0.0, 1.0, 0.0);
     glScalef( -1.0, 1.0, 1.0);
     book_render_page();

     book_std_position(); /* set origin to far spine */
     glTranslatef (0.0, 0.0, 0.05);
     glColor4f (0.5, 0.5, 0.5, 1.0);
     glLineStipple (1, ((85<<8)+85));
     glEnable (GL_LINE_STIPPLE);
     glBegin (GL_LINES);
       glVertex3f (0.0, 0.0, 0.0);
       glVertex3f (0.0, -page_height, 0.0);
     glEnd();
     glDisable (GL_LINE_STIPPLE);

}
