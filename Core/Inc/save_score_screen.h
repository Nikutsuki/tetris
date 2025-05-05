#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <stdbool.h>

typedef struct {
    bool should_update;
    char name[10];
} save_scores_screen_t;

void render_save_score_screen(save_scores_screen_t* authors_menu, int score);
void init_save_score_screen(save_scores_screen_t* authors_menu);