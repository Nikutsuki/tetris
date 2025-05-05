#include "end_screen.h"

void render_end_screen(end_screen_t *end_screen)
{
    if (end_screen->should_update == false)
    {
        return;
    }

    // For end_screen->menu_selection values:
    // 0: "Menu" highlighted, 1: "Restart" highlighted, 2: "zapisz" highlighted

    switch (end_screen->menu_selection)
    {
    case 0:
    {
        ssd1306_Fill(Black);
        // Score display
        ssd1306_SetCursor(1, 1);
        ssd1306_WriteString("Game over", Font_6x8, White);
        char score_str[16];
        sprintf(score_str, "Score: %lu", end_screen->score);
        ssd1306_SetCursor(1, 12);
        ssd1306_WriteString(score_str, Font_6x8, White);

        // "Menu" - Highlighted
        ssd1306_FillRectangle(1, 23, 63, 38, White);
        ssd1306_SetCursor(20, 27);
        ssd1306_WriteString("Menu", Font_6x8, Black);

        // "Restart" - Normal
        ssd1306_DrawRectangle(1, 41, 63, 56, White);
        ssd1306_SetCursor(13, 45);
        ssd1306_WriteString("Restart", Font_6x8, White);

        // "zapisz" - Normal
        ssd1306_DrawRectangle(1, 59, 63, 74, White);
        ssd1306_SetCursor(16, 63);
        ssd1306_WriteString("zapisz", Font_6x8, White);
        break;
    }
    case 1:
    {
        ssd1306_Fill(Black);
        // Score display
        ssd1306_SetCursor(1, 1);
        ssd1306_WriteString("Game over", Font_6x8, White);
        char score_str[16];
        sprintf(score_str, "Score: %lu", end_screen->score);
        ssd1306_SetCursor(1, 12);
        ssd1306_WriteString(score_str, Font_6x8, White);

        // "Menu" - Normal
        ssd1306_DrawRectangle(1, 23, 63, 38, White);
        ssd1306_SetCursor(20, 27);
        ssd1306_WriteString("Menu", Font_6x8, White);

        // "Restart" - Highlighted
        ssd1306_FillRectangle(1, 41, 63, 56, White);
        ssd1306_SetCursor(13, 45);
        ssd1306_WriteString("Restart", Font_6x8, Black);

        // "zapisz" - Normal
        ssd1306_DrawRectangle(1, 59, 63, 74, White);
        ssd1306_SetCursor(16, 63);
        ssd1306_WriteString("zapisz", Font_6x8, White);
        break;
    }
    case 2:
    {
        ssd1306_Fill(Black);
        // Score display
        ssd1306_SetCursor(1, 1);
        ssd1306_WriteString("Game over", Font_6x8, White);
        char score_str[16];
        sprintf(score_str, "Score: %lu", end_screen->score);
        ssd1306_SetCursor(1, 12);
        ssd1306_WriteString(score_str, Font_6x8, White);

        // "Menu" - Normal
        ssd1306_DrawRectangle(1, 23, 63, 38, White);
        ssd1306_SetCursor(20, 27);
        ssd1306_WriteString("Menu", Font_6x8, White);

        // "Restart" - Normal
        ssd1306_DrawRectangle(1, 41, 63, 56, White);
        ssd1306_SetCursor(13, 45);
        ssd1306_WriteString("Restart", Font_6x8, White);

        // "zapisz" - Highlighted
        ssd1306_FillRectangle(1, 59, 63, 74, White);
        ssd1306_SetCursor(16, 63);
        ssd1306_WriteString("zapisz", Font_6x8, Black);
        break;
    }
    }

    ssd1306_UpdateScreen();
    end_screen->should_update = false;
}

void init_end_screen(end_screen_t *end_screen)
{
    end_screen->menu_selection = 0;
    end_screen->should_update = true;
}