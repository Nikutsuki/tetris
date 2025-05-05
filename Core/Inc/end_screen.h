#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <stdbool.h>

typedef struct {
    int menu_selection;
    bool should_update;
    int score;
} end_screen_t;

void render_end_screen(end_screen_t* end_screen);
void init_end_screen(end_screen_t* end_screen);