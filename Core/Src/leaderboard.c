#include "leaderboard.h"

void render_leaderboard(leaderboard_t *leaderboard, high_score_t *scores)
{
    if (!leaderboard->should_update)
    {
        return;
    }

    ssd1306_Fill(Black);
    ssd1306_SetCursor(8, 0);
    ssd1306_WriteString("Rekordy", Font_7x10, White);
    ssd1306_SetCursor(0, 10);

    for (int i = 0; i < MAX_SCORES; i++)
    {
        if(scores[i].name[0] == '\0')
        {
            break;
        }
        char scoreStr[5];
        sprintf(scoreStr, "%lu", scores[i].score);
        ssd1306_SetCursor(0, 10 + (2 * i) * 12);
        ssd1306_WriteString(scores[i].name, Font_7x10, White);
        ssd1306_WriteString(":", Font_7x10, White);
        ssd1306_SetCursor(0, 10 + (2*i + 1) * 12);
        ssd1306_WriteString(scoreStr, Font_7x10, White);
    }

    leaderboard->should_update = false;
    ssd1306_UpdateScreen();
}

void init_leaderboard(leaderboard_t *leaderboard)
{
    leaderboard->should_update = true;
}