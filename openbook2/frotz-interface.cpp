/** interface-frotz.cpp - C/C++ crossover episode for frotz. **/

#include "openbook2.h"
#include <sys/stat.h>

// os_path_open
//  takes a story name, some arguments, and a pointer with size details
extern FILE* os_path_open (const char *xiStoryName, const char* xiArgs, long* xiSizePtr)
{
    Logger::Log("Entering os_path_open...", LOG_DEBUG);

    // qq: hardcoded for now
    //xiStoryName = "stories\\makegood.z8";
    xiStoryName = "stories\\planetfall.z5";
    //xiStoryName = "..\\stories\\quake.z5";
    //xiStoryName = "stories\\milliways_release184.z6";
    //xiStoryName = "stories\\Conference.z5";

    struct stat results;
    if (stat(xiStoryName, &results) == 0)
    {
        FILE* lFile = fopen(xiStoryName, xiArgs);
        *xiSizePtr = results.st_size;
        return lFile;
    }
    else
    {
        throw("Error occurred reading file.");
    }
}

// os_display_char
//  Takes a single zword, displays a single char.
void os_display_char (zword c)
{
    putchar((char)c);
    PrintLogic::AddChar(c);
}

// os_display_string
// Takes a pointer to a zword array.
void os_display_string (const zword* s)
{
    Logger::Log("Entering frotz-interface::os_display_string...", LOG_DEBUG);

    // establish trace text
    char lDisplayMessage[80];
    memset(lDisplayMessage, 0, sizeof(lDisplayMessage));
    strcat(lDisplayMessage, "Displaying string: '");
    int strpos = strlen(lDisplayMessage);

    // step through input, adding each character to the PrintLogic buffer
    zword c;
    while ((c = *s++) != 0)
    {
        if (c == ZC_NEW_FONT)
            s++;
        else if (c == ZC_NEW_STYLE)
            os_set_text_style(*s++);
        else
        {
            // add to the PrintLogic buffer
            PrintLogic::BufferAddChar(c);

            // dump it to the stdout for now
            std::cout << (char)c;

            // and add to trace
            lDisplayMessage[strpos++] = c;
        }
    }

    // record trace
    lDisplayMessage[strpos++] = '\'';
    lDisplayMessage[strpos] = 0;
    Logger::Log(lDisplayMessage, LOG_DEBUG);

    // trigger printlogic
    PrintLogic::AddBuffer();
}

/* get a character.  Exit with no fuss on EOF.  */
// this will not be an interface function.
static int local_getchar_from_console(void)
{
    int c = getchar();
    if (c == EOF)
    {
        if (feof(stdin))
        {
            fprintf(stderr, "\nEOT\n");
            exit(0);
        }
        os_fatal(strerror(errno));
    }
    return c;
}

// this will not be an interface function
zword os_read_key (int xiTimeout, int xiShowCursor)
{
    return (zword)ZC_RETURN;
}

// this will not be an interface function. Now unused
static void local_getline_from_console(char *s)
{
    Logger::Log("Entering frotz-interface::getline...", LOG_DEBUG);
    int c;
    char *p = s;

    memset(s, '0', INPUT_BUFFER_SIZE);

    while (p < s + INPUT_BUFFER_SIZE - 1)
        if ((*p++ = local_getchar_from_console()) == '\n')
        {
            p[-1] = ' ';
            *p = '\0';
            return;
        }

    p[-1] = 13;
    p[0] = '\0';
    while ((c = local_getchar_from_console()) != '\n')
        ;

    Logger::Log("Input line truncated.", LOG_WARN);
}

// os_read_line
//  reads a line of length xiMax into xoBuffer before xiTimeout.
//  Ignores xiWidth and xiContinued. Returns the terminator.
zword os_read_line (int xiMax, zword* xoBuffer, int xiTimeout, int xiWidth, int xiContinued)
{
    Logger::Log("Entering frotz-interface::os_read_line...", LOG_DEBUG);

    Logger::Log("Fetching input string...", LOG_DEBUG);
    const char* lCharBuffer = Frotzer::GetInputString(xiTimeout);
    Logger::Log((string("Input string returned: ").append(lCharBuffer)).c_str(), LOG_DEBUG);

    if (strlen(lCharBuffer) == 0)
    {
        // timeout
        return 0;
    }

    // copy results from the local_getline (chars) into the buffer (zwords).
    int lRead; int lWrite = 0;
    for (lRead = 0; lRead < INPUT_BUFFER_SIZE; lRead++)
    {
        xoBuffer[lWrite++] = (zword)lCharBuffer[lRead];
    }

    return (zword)13;
}

/// os_fatal
//  For now, print message to stdout, exit.
void os_fatal (const char* s)
{
    Logger::Log("Entering frotz-interface::os_fatal...", LOG_DEBUG);
    Logger::Log(s, LOG_ERROR);
    exit(0);
}


/// STUBS
// functions to implement

void os_beep (int x)
{
    Logger::Log("Entering frotz-interface::os_beep...", LOG_DEBUG);
    os_display_string((zword*)"BEEP!");
}

int os_buffer_screen (int x)
{
    Logger::Log("Entering frotz-interface::os_buffer_screen...", LOG_DEBUG);
    return NULL;
}
int os_char_width (zword x)
{
    Logger::Log("Entering frotz-interface::os_char_width...", LOG_DEBUG);
    return 1;
}

int os_string_width (const zword *s)
{
    Logger::Log("Entering frotz-interface::os_string_width...", LOG_DEBUG);
    int lCount = 0;
    zword c;
    while ((c = *s++) != 0)
        lCount += os_char_width(c);

    return lCount;
}

int os_check_unicode (int x, zword y)
{
    Logger::Log("Entering frotz-interface::os_check_unicode...", LOG_DEBUG);
    return NULL;
}
void os_draw_picture (int x, int y, int z)
{
    Logger::Log("Entering frotz-interface::os_draw_picture...", LOG_DEBUG);
}
void os_erase_area (int v, int w, int x, int y, int z)
{
    Logger::Log("Entering frotz-interface::os_erase_area...", LOG_DEBUG);
}
void os_finish_with_sample (int x)
{
    Logger::Log("Entering frotz-interface::os_finish_with_sample...", LOG_DEBUG);
}

// returns true if font is available (TEXT_FONT, FIXED_WIDTH_FONT, GRAPHICS_FONT)
int os_font_data (int font, int *height, int *width)
{
    Logger::Log("Entering frotz-interface::os_font_data...", LOG_DEBUG);
    *height = 1;
    *width = 1;
    return 1;
}
int os_from_true_colour (zword c)
{
    Logger::Log("Entering frotz-interface::os_from_true_colour...", LOG_DEBUG);
    return NULL;
}
void os_more_prompt ()
{
    Logger::Log("Entering frotz-interface::os_more_prompt...", LOG_DEBUG);
}
int os_picture_data (int x, int *y, int *z)
{
    Logger::Log("Entering frotz-interface::os_picture_data...", LOG_DEBUG);
    return NULL;
}
void os_prepare_sample (int s)
{
    Logger::Log("Entering frotz-interface::os_prepare_sample...", LOG_DEBUG);
}
void os_process_arguments (int a, char *v[])
{
    Logger::Log("Entering frotz-interface::os_process_arguments...", LOG_DEBUG);
    story_name = "planetfall.z5";
}
int	os_random_seed ()
{
    Logger::Log("Entering frotz-interface::os_random_seed...", LOG_DEBUG);
    return 6;
}
int os_read_file_name (char *f, const char *g, int h)
{
    Logger::Log("Entering frotz-interface::os_read_file_name...", LOG_DEBUG);
    return NULL;
}
zword os_read_mouse ()
{
    Logger::Log("Entering frotz-interface::os_read_mouse...", LOG_DEBUG);
    return NULL;
}
void os_restart_game (int r)
{
    Logger::Log("Entering frotz-interface::os_restart_game...", LOG_DEBUG);
}
void os_set_colour (int a, int c)
{
    Logger::Log("Entering frotz-interface::os_set_colour...", LOG_DEBUG);
}
void os_set_font (int f)
{
    Logger::Log("Entering frotz-interface::os_set_font...", LOG_DEBUG);
}
void os_set_text_style (int s)
{
    Logger::Log("Entering frotz-interface::os_set_text_style...", LOG_DEBUG);
}
void os_start_sample (int s, int a, int b, zword c)
{
    Logger::Log("Entering frotz-interface::os_start_sample...", LOG_DEBUG);
}
void os_stop_sample (int s)
{
    Logger::Log("Entering frotz-interface::os_stop_sample...", LOG_DEBUG);
}
void os_tick ()
{
    /* Logger::Log("Entering frotz-interface::os_tick...", LOG_DEBUG); */
}
zword os_to_true_colour (int t)
{
    Logger::Log("Entering frotz-interface::os_to_true_colour...", LOG_DEBUG);
    return NULL;
}

// functions to leave as stubs

void os_init_screen ()
{
    Logger::Log("Entering frotz-interface::os_init_screen...", LOG_DEBUG);
    h_screen_rows = H_SCREEN_ROWS;
    h_screen_cols = H_SCREEN_COLS;
    h_font_width = 1;
    h_font_height = 1;
    h_screen_width = h_screen_rows * h_font_width;
    h_screen_height = h_screen_cols * h_font_height;
    h_default_foreground = 1;
    h_default_background = 1;
}

void os_menu(int m, int n, const zword *o)
{
    Logger::Log("Entering frotz-interface::os_menu...", LOG_DEBUG);
}
int os_peek_colour ()
{
    Logger::Log("Entering frotz-interface::os_peek_colour...", LOG_DEBUG);
    return 1;
}
void os_reset_screen ()
{
    Logger::Log("Entering frotz-interface::os_reset_screen...", LOG_DEBUG);
}
void os_scroll_area (int v, int w, int x, int y, int z)
{
    Logger::Log("Entering frotz-interface::os_scroll_area...", LOG_DEBUG);
}
void os_scrollback_char (zword s)
{
    Logger::Log("Entering frotz-interface::os_scrollback_char...", LOG_DEBUG);
}
void os_scrollback_erase (int m)
{
    Logger::Log("Entering frotz-interface::os_scrollback_erase...", LOG_DEBUG);
}
void os_set_cursor (int x, int y)
{
    Logger::Log("Entering frotz-interface::os_set_cursor...", LOG_DEBUG);
}

// returns non-zero if the window should wrap
int os_wrap_window (int w)
{
    Logger::Log("Entering frotz-interface::os_wrap_window...", LOG_DEBUG);
    //cout << endl;
    return 0;
}
void os_window_height (int w, int h)
{
    Logger::Log("Entering frotz-interface::os_window_height...", LOG_DEBUG);
}
