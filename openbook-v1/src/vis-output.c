/*
 * Vis - FreeGLUT library and interface for Frotz
 *
 * vis-output.c: whole-body for dumb-output.c
 *               massive rewrite. very traumatic.
 *
 */

#include "vis.h"

struct position_detail { float x, float y, float z,            /* x, y, z */
                         float angle,                          /* angle */
                         float ax, float ay, float az          /* rotation-axis */
};

struct char_detail {     char c, float l,                      /* char, line-width */
                         position_detail cp                    /* current position */
                         position_detail tp                    /* target position */                        
                         float r, float g, float b, float a    /* r, g, b, a */
};
                        
struct row_detail {      float width,                          /* cumulative width */
                         float x, float y, float z,            /* initial x,y,z */
                         int chars,                            /* characters stored */
                         char_detail* s                        /* string of content */
};

struct window_detail {   float left, float top,                /* position */
                         float z,                              /* initial z */
                         float width, float height,            /* w, h */
                         int rows,                             /* rows stored */
                         row_detail* row                       /* row contents */
} v_wp[6];

char latin1_to_ascii[] =
  "    !   c   L   >o< Y   |   S   ''  C   a   <<  not -   R   _   "
  "^0  +/- ^2  ^3  '   my  P   .   ,   ^1  o   >>  1/4 1/2 3/4 ?   "
  "A   A   A   A   Ae  A   AE  C   E   E   E   E   I   I   I   I   "
  "Th  N   O   O   O   O   Oe  *   O   U   U   U   Ue  Y   Th  ss  "
  "a   a   a   a   ae  a   ae  c   e   e   e   e   i   i   i   i   "
  "th  n   o   o   o   o   oe  :   o   u   u   u   ue  y   th  y   "
;

#define PICTURE_STYLE 16

/* current editing position detail */
int current_style = 0;
int current_window = 0;
int cursor_row = 1;
int cursor_col = 1;

/* Reverse-video display styles.  */
static enum { RV_NONE, RV_DOUBLESTRIKE, RV_UNDERLINE, RV_CAPS } rv_mode = RV_NONE;
static char *rv_names[] = {"NONE", "DOUBLESTRIKE", "UNDERLINE", "CAPS"};
static char rv_blank_char = ' ';

int os_char_width (zchar z) {
    return (int)vis_get_char_width(z);
};

int os_string_width (const zchar *s) {
    int width = 0;
    zchar c;
    while ((c = *s++) != 0)
        if (c == ZC_NEW_STYLE || c == ZC_NEW_FONT)
        s++;
    else
        width += os_char_width(c);
    return width;
};

void vis_set_cursor(int window, int row, int col) {
    /* cursor_row = row - 1; cursor_col = col - 1;
    if (cursor_row >= h_screen_rows)
        cursor_row = h_screen_rows - 1;
    */
    current_window = window;
    cursor_row = row;
    cursor_col = col;
};

void os_set_text_style(int x) { current_style = x & REVERSE_STYLE; };

void os_display_char (zchar c)
{
   /* if (c >= ZC_LATIN1_MIN && c <= ZC_LATIN1_MAX) { */
         
    if (plain_ascii) {
      char *ptr = latin1_to_ascii + 4 * (c - ZC_LATIN1_MIN);
      do
	    vis_assign_char(*ptr++);
      while (*ptr != ' ');
      
    } else {
      vis_assign_char(c);
    } 
    
    /*
    else if (c >= 32 && c <= 126) {
      dumb_display_char(c);
      
    } else if (c == ZC_GAP) {
      dumb_display_char(' '); dumb_display_char(' ');
      
    } else if (c == ZC_INDENT) {
      dumb_display_char(' '); dumb_display_char(' '); dumb_display_char(' ');
      
    } */
  /* }  */
}

void os_display_string (const zchar *s)
{
  zchar c;
  while ((c = *s++) != 0)
    if (c == ZC_NEW_FONT)
      s++;
    else if (c == ZC_NEW_STYLE)
      os_set_text_style(*s++);
    else
      os_display_char (c);
}

/* os_erase_area -- what calls this? */
/* os_scroll_area -- what calls this? */

void os_erase_area (int top, int left, int bottom, int right)
{
  int row, col;
  top--; left--; bottom--; right--;
  for (row = top; row <= bottom; row++)
    for (col = left; col <= right; col++)
      dumb_set_cell(row, col, make_cell(current_style, ' '));
}

void os_scroll_area (int top, int left, int bottom, int right, int units) {
  int row, col;
  top--; left--; bottom--; right--;
  if (units > 0) {
    for (row = top; row <= bottom - units; row++)
      for (col = left; col <= right; col++)
	dumb_copy_cell(row, col, row + units, col);
    os_erase_area(bottom - units + 2, left + 1, bottom + 1, right + 1);
  } else if (units < 0) {
    for (row = bottom; row >= top - units; row--)
      for (col = left; col <= right; col++)
	dumb_copy_cell(row, col, row + units, col);
    os_erase_area(top + 1, left + 1, top - units, right + 1);
  }
};

int os_font_data(int font, int *height, int *width) {
    if (font == TEXT_FONT) {
      *height = 1; *width = 1; return 1;
    }
    return 0;
};


void os_set_colour (int x, int y) {};
void os_set_font (int x) {};


void os_reset_screen(void) { dumb_show_screen(FALSE); };

void os_beep (int volume)
{
  if (visual_bell)
    printf("[%s-PITCHED BEEP]\n", (volume == 1) ? "HIGH" : "LOW");
  else
    putchar('\a'); /* so much for dumb.  */
}

void os_prepare_sample (int x) {}
void os_finish_with_sample (void) {}
void os_start_sample (int x, int y, int z) {}
void os_stop_sample (void) {}
