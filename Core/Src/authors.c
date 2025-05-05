#include "authors.h"

void render_authors(authors_menu_t* authors_menu)
{
    if(!authors_menu->should_update)
    {
        return;
    }
    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Autorzy:", Font_7x10, White);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString("Mateusz K.", Font_6x8, White);

    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString("Jakub S.", Font_6x8, White);

    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString("Michal D.", Font_6x8, White);

    ssd1306_SetCursor(0, 40);
    ssd1306_WriteString("Kacper D.", Font_6x8, White);

    authors_menu->should_update = false;
    ssd1306_UpdateScreen();
}

void init_authors(authors_menu_t* authors_menu)
{
    authors_menu->should_update = true;
}