#include "main_menu.h"

void render_menu_main(main_menu_t *menu)
{
	if(menu->should_update == false)
	{
		return;
	}
	switch (menu->menu_selection)
	{
	case 0:
	{
		ssd1306_Fill(Black);

		ssd1306_FillRectangle(1, 1, 63, 20, White);
		ssd1306_SetCursor(15, 7);
		ssd1306_WriteString("Start", Font_7x10, Black);

		ssd1306_DrawRectangle(1, 23, 63, 43, White);
		ssd1306_SetCursor(13, 29);
		ssd1306_WriteString("Wyniki", Font_7x10, White);

		ssd1306_DrawRectangle(1, 46, 63, 64, White);
		ssd1306_SetCursor(8, 51);
		ssd1306_WriteString("Autorzy", Font_7x10, White);

		ssd1306_UpdateScreen();
		break;
	}
	case 1:
	{
		ssd1306_Fill(Black);

		ssd1306_DrawRectangle(1, 1, 63, 20, White);
		ssd1306_SetCursor(15, 7);
		ssd1306_WriteString("Start", Font_7x10, White);

		ssd1306_FillRectangle(1, 23, 63, 43, White);
		ssd1306_SetCursor(13, 29);
		ssd1306_WriteString("Wyniki", Font_7x10, Black);

		ssd1306_DrawRectangle(1, 46, 63, 64, White);
		ssd1306_SetCursor(8, 51);
		ssd1306_WriteString("Autorzy", Font_7x10, White);


		ssd1306_UpdateScreen();
		break;
	}
	case 2:
	{
		ssd1306_Fill(Black);

		ssd1306_DrawRectangle(1, 1, 63, 20, White);
		ssd1306_SetCursor(15, 7);
		ssd1306_WriteString("Start", Font_7x10, White);

		ssd1306_DrawRectangle(1, 23, 63, 43, White);
		ssd1306_SetCursor(13, 29);
		ssd1306_WriteString("Wyniki", Font_7x10, White);

		ssd1306_FillRectangle(1, 46, 63, 64, White);
		ssd1306_SetCursor(8, 51);
		ssd1306_WriteString("Autorzy", Font_7x10, Black);

		ssd1306_UpdateScreen();
		break;
	}
	default:
		break;
	}

	menu->should_update = false;
}

void init_main_menu(main_menu_t* menu)
{
	menu->menu_selection = 0;
	menu->name = "Main Menu";
	menu->function = NULL;
	menu->should_update = true;
}