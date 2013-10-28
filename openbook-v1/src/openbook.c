/*
 * OpenBook: Frotz + eye candy
 *
 * openbook.c: initialisation
 *
 */

#include "vis.h"

void vis_init_debug() {
/*     if (debugging > 0) */
          debug = fopen("openbook.debug", "wt");
};

void vis_debug(char key, char* message) {
/*     if (debugging == 0)
        return;*/
        
     char mp1[8];
     mp1[0] = '[';
     mp1[1] = key;
     mp1[2] = ']';
     mp1[3] = ' ';
     mp1[4] = '%';
     mp1[5] = 's';
     mp1[6] = '\n';
     mp1[7] = '\0';
     fprintf(debug, mp1, message);
};

void vis_debug_print_c(char k) {
/*     if (debugging > 0) { */
          char pc[2];
          pc[0] = k; pc[1]='\0';
          fprintf(debug, pc);
/*          }*/
     };
     
void vis_debug_print_s(char* m) {
/*     if (debugging > 0)*/
          fprintf(debug, m);
     };

void vis_close_debug() {
/*     if (debugging > 0) */
          fclose(debug);
};

int main(int argc, char* argv[]) {

    vis_init_debug();
    vis_debug(' ', "OpenBook initialisation, part 1");

    vis_init();
    
    vis_debug('v', "Vis initialised.");
    vis_debug(' ', "Initialising Frotz...");
    
    argc=2;
    argv[1] = "shade.z5";
    argv[1] = "House.z5";
    argv[1] = "termite.z5";
    df_main(argc, argv);

    vis_debug('f', "Frotz finished.");

    vis_shutdown();
    
    vis_debug('v', "Vis shutdown completed.");

    vis_close_debug();
    
};
