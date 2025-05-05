#include "save_score_screen.h"

void render_save_score_screen(save_scores_screen_t* authors_menu, int score)
{
    if(!authors_menu->should_update)
    {
        return;
    }
    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Punkty:", Font_6x8, White);
    char score_str[10];
    sprintf(score_str, "%d", score);
    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString(score_str, Font_6x8, White);
    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString("Podaj imie:", Font_6x8, White);
    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString(authors_menu->name, Font_6x8, White);

    authors_menu->should_update = false;
    ssd1306_UpdateScreen();
}

void init_save_score_screen(save_scores_screen_t* authors_menu)
{
    authors_menu->should_update = true;
    authors_menu->name[0] = '\0';
}