/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-main.c: setup, main callbacks
 *
 */

#include "vis.h"

char vis_input_buffer[INPUT_BUFFER_SIZE];
int vis_next_input_char;

/* GLUT callback Handlers */

static void vis_resize(int width, int height) {
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void vis_display(void) {
    /* const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0; */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    book_render();
    
    glutSwapBuffers();
}

static void vis_key(unsigned char key, int x, int y) {
    /* set character, increment current position */
    if (vis_next_input_char < INPUT_BUFFER_SIZE)
        vis_input_buffer[vis_next_input_char++] = key;

    /* glutPostRedisplay(); */
}

void vis_post_redisplay() {
     glutPostRedisplay();     
};

/* get character from buffer */
int vis_get_char_blocking() {
      
       /* extern float page_angle; */
      
       /* allow GLUT to continue, animate, collect characters... */
       while (vis_next_input_char == 0) {
            glutMainLoopEvent();

            /* page_angle += 1.0;
            glutPostRedisplay(); */
       }
       
       int i; int o;
       o = vis_input_buffer[0];
 
       for (i=0; i < vis_next_input_char; i++) {
               vis_input_buffer[i] = vis_input_buffer[i+1];
       }
       vis_input_buffer[vis_next_input_char--] = '\0';

       return o;
}

static void vis_idle(void) { /* nothing much for now */ }

const GLfloat light_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
const GLfloat light_position[] = { 8.0f, 8.0f, -2.0f, 1.0f }; /* positive Z is above page */

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

static void vis_render_settings() {
    /* render settings */
    glClearColor(0,0,0,1);
    /*
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* lighting */
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
    /* glEnable (GL_POLYGON_SMOOTH);  /* Enable Antialiased polygon edges */
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);
             
    /* glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); */

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* material settings */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);       

};

int vis_init() {
    
    /* initialise glut with defaults */
    int argc = 0;
    char* argv[1];
    argv[0]="openbook";
    glutInit(&argc, argv);

    /*establish window */
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenBook");

    /* set callback functions */
    glutReshapeFunc(vis_resize);
    glutDisplayFunc(vis_display);
    glutKeyboardFunc(vis_key);
    glutIdleFunc(vis_idle);

    vis_render_settings();

    /* other setup */
    vis_next_input_char=1;
    
    book_init();
    vis_load_images();
    return 1;
}

int vis_get_char_width(char z) {
  extern void* play_font;
  int w = glutStrokeWidth(play_font, z);
  return w;
};


void vis_shutdown() {
    glutLeaveMainLoop();
};
