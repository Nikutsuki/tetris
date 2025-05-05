/*
 * game.c
 *
 *  Created on: Jan 20, 2025
 *      Author: nikutsuki
 */

#include "game.h"

void render_border(game_t *game)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0, 0);

	char scoreStr[5];

	sprintf(scoreStr, "%lu", (void *)game->score);
	ssd1306_WriteString(scoreStr, Font_7x10, White);
	ssd1306_FillRectangle(0, 10, 64, 11, White);
	ssd1306_FillRectangle(0, 10, 1, 127, White);
	ssd1306_FillRectangle(62, 10, 64, 127, White);
	ssd1306_FillRectangle(0, 126, 64, 127, White);
}

piece_t *generate_random_piece(game_t *game)
{
	uint32_t type = HAL_RNG_GetRandomNumber(&game->hrng) % 7;
	return construct_piece(type, 26, 11);
}

void game_init(game_t *game, RNG_HandleTypeDef hrng)
{

	game->hrng = hrng;
	game->piece = generate_random_piece(game);
	game->last_time = HAL_GetTick();
	game->last_fall_time = HAL_GetTick();
	game->should_update = false;
	game->should_fall = false;
	game->score = 0;
	game->main_menu = malloc(sizeof(main_menu_t));
	game->state = MAIN_MENU;
	game->end_screen = malloc(sizeof(end_screen_t));
	game->authors_menu = malloc(sizeof(authors_menu_t));
	game->save_score_screen = malloc(sizeof(save_scores_screen_t));
    game->leaderboard = malloc(sizeof(leaderboard_t));

	init_main_menu(game->main_menu);
	init_end_screen(game->end_screen);
	init_authors(game->authors_menu);
	init_save_score_screen(game->save_score_screen);
    init_leaderboard(game->leaderboard);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			game->blocks[i][j] = 0;
		}
	}
}

void game_tick(game_t *game)
{
	render_border(game);
	if (should_game_end(game))
	{
		game->end_screen->score = game->score;
		game->state = GAME_OVER;
		return;
	}
	next_tick(game, HAL_GetTick());
	break_lines(game);
	if (!game->should_update)
	{
		return;
	}
	check_collision(game);
	render_piece(game->piece);
	render_blocks(game);

	if (game->should_fall)
	{
		move_piece(game->piece, 0, FALL_SPEED);
		game->should_fall = false;
	}

	game->should_update = false;

	ssd1306_UpdateScreen();
	HAL_Delay(50);
}

void check_collision(game_t *game)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (game->piece->blocks[i][j].x != -1)
			{
				if (game->piece->blocks[i][j].y + FALL_SPEED >= 122 || game->blocks[(game->piece->blocks[i][j].x) / 6][(game->piece->blocks[i][j].y + FALL_SPEED) / 6] == 1)
				{
					convert_piece_to_blocks(game);
					delete_game_piece(game);
					game->piece = generate_random_piece(game);
				}
			}
		}
	}
}

void convert_piece_to_blocks(game_t *game)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (game->piece->blocks[i][j].x != -1)
			{
				// Round positions to grid
				int gridX = (game->piece->blocks[i][j].x) / 6;
				int gridY = (game->piece->blocks[i][j].y) / 6;

				// Clamp positions to 10x27
				if (gridX < 0)
					gridX = 0;
				if (gridX > 9)
					gridX = 9;
				if (gridY < 0)
					gridY = 0;
				if (gridY > 19)
					gridY = 19;

				// Assign the block to the grid
				game->blocks[gridX][gridY] = 1;
			}
		}
	}
}

void render_blocks(game_t *game)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (game->blocks[i][j] == 1)
			{
				int x = i * 6 + 2;
				int y = j * 6 + 5;
				ssd1306_DrawRectangle(x, y, x + 6, y + 6, White);
				ssd1306_DrawRectangle(x + 2, y + 2, x + 4, y + 4, White);
			}
		}
	}
}

void delete_game_piece(game_t *game)
{
	delete_piece(game->piece);
}

void keyboard_input(game_t *game, char input)
{
    switch(game->state)
    {
        case PLAYING:
        {
            switch(input)
            {
                case 'a':
                    if (is_valid_move(game, game->piece, -6, 0))
                    {
                        move_piece(game->piece, -6, 0);
                    }
                    break;
                case 'd':
                    if (is_valid_move(game, game->piece, 6, 0))
                    {
                        move_piece(game->piece, 6, 0);
                    }
                    break;
                case 's':
                    if (is_valid_move(game, game->piece, 0, 6))
                    {
                        move_piece(game->piece, 0, 6);
                    }
                    else
                    {
                        convert_piece_to_blocks(game);
                        delete_game_piece(game);
                        game->piece = generate_random_piece(game);
                    }
                    break;
                case 'r':
                    rotate_piece(game->piece, game->blocks);
                    break;
                default:
                    break;
            }
            game->should_update = true;
            game->last_fall_time = HAL_GetTick();
            break;
        }
        case MAIN_MENU:
        {
            switch(input)
            {
                case 'w':
                    if (game->main_menu->menu_selection != 0)
                    {
                        game->main_menu->menu_selection -= 1;
                        game->main_menu->should_update = true;
                    }
                    break;
                case 's':
                    if (game->main_menu->menu_selection != 2)
                    {
                        game->main_menu->menu_selection += 1;
                        game->main_menu->should_update = true;
                    }
                    break;
                case '\r':
                    if (game->main_menu->menu_selection == 0)
                    {
                        game_reset(game);
                        game->state = PLAYING;
                    }
                    if(game->main_menu->menu_selection == 1)
                    {
                        read_high_scores(game->high_scores);
                        game->state = HIGH_SCORES;
                        game->should_update = true;
                        game->leaderboard->should_update = true;
                    }
                    else if (game->main_menu->menu_selection == 2)
                    {
                        game->state = AUTHORS;
                        game->main_menu->should_update = true;
                        game->authors_menu->should_update = true;
                    }
                    game->should_update = true;
                    break;
				case 'p':
					clear_flash_memory();
                    break;
				case 't':
					high_score_t scores[MAX_SCORES];
					read_high_scores(scores);
                    break;
                case 'y':
                    save_high_score("test2", 1000);
                    break;
                default:
                    break;
            }
            break;
        }
        case AUTHORS:
        {
            if(input == 27)  // Escape key
            {
                game->state = MAIN_MENU;
                game->should_update = true;
                game->main_menu->should_update = true;
            }
            break;
        }
        case GAME_OVER:
        {
            switch(input)
            {
                case 'w':
                    if(game->end_screen->menu_selection != 0)
                    {
                        game->end_screen->menu_selection -= 1;
                        game->end_screen->should_update = true;
                    }
                    break;
                case 's':
                    if(game->end_screen->menu_selection != 2)
                    {
                        game->end_screen->menu_selection += 1;
                        game->end_screen->should_update = true;
                    }
                    break;
                case '\r':
                    if(game->end_screen->menu_selection == 0)
                    {
                        game->state = MAIN_MENU;
                        game->main_menu->should_update = true;
                    }
                    else if(game->end_screen->menu_selection == 1)
                    {
                        game_reset(game);
                        game->state = PLAYING;
                    }
                    else if(game->end_screen->menu_selection == 2)
                    {
                        game->state = SAVE_SCORE;
                        game->save_score_screen->should_update = true;
                    }
                    game->should_update = true;
                    break;
                default:
                    break;
            }
            break;
        }
        case SAVE_SCORE:
        {
            switch(input)
            {
                case 27:  // Escape key
                    game->state = GAME_OVER;
                    game->end_screen->should_update = true;
                    break;
                case '\r':
                    save_high_score(game->save_score_screen->name, game->score);
					game->save_score_screen->name[0] = '\0';
                    game->state = MAIN_MENU;
                    game->main_menu->should_update = true;
                    break;
                case 8:  // Backspace
                {
                    size_t len = strlen(game->save_score_screen->name);
                    if(len > 0)
                    {
                        game->save_score_screen->name[len - 1] = '\0';
                        game->save_score_screen->should_update = true;
                    }
                    break;
                }
                default:
                {
                    if(strlen(game->save_score_screen->name) < 8 && input >= 'a' && input <= 'z')
                    {
                        size_t len = strlen(game->save_score_screen->name);
                        game->save_score_screen->name[len] = input;
                        game->save_score_screen->name[len + 1] = '\0';
                        game->save_score_screen->should_update = true;
                    }
                    break;
                }
            }
            break;
        }
        case HIGH_SCORES:
        {
            if(input == 27)  // Escape key
            {
                game->state = MAIN_MENU;
                game->main_menu->should_update = true;
            }
            break;
        }
        default:
            break;
    }
}

void next_tick(game_t *game, uint32_t current_time)
{
	if (current_time - game->last_fall_time > SIMULATION_SPEED)
	{
		game->should_fall = true;
		game->should_update = true;
		game->last_fall_time = current_time;
	}
}

void break_lines(game_t *game)
{
	bool should_fall = false;
	int broken_lines = 0;
	int how_many_broken = 0;
	int add_score = 0;
	for (int i = 0; i < 20; i++)
	{
		int count = 0;
		for (int j = 0; j < 10; j++)
		{
			if (game->blocks[j][i] == 1)
			{
				count++;
			}
		}
		if (count == 10)
		{
			how_many_broken++;
			add_score += 128;
			// Shift rows above down
			for (int row = i; row > 0; row--)
			{
				for (int col = 0; col < 10; col++)
				{
					game->blocks[col][row] = game->blocks[col][row - 1];
				}
			}
			// Clear the top row
			for (int col = 0; col < 10; col++)
			{
				game->blocks[col][0] = 0;
			}
			// Re-check this row after shifting
			i--;
		}
	}
	if (how_many_broken > 1)
	{
		game->score += add_score*how_many_broken;
		game->should_update = true;
	}
	else
	{
		game->score += add_score;
		game->should_update = true;
	}
}

bool is_valid_move(game_t *game, piece_t *piece, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (piece->blocks[i][j].x != -1)
			{
				int gridX = (piece->blocks[i][j].x + x) / 6;
				int gridY = (piece->blocks[i][j].y + y) / 6;

				if (gridX < 0 || gridX > 9 || gridY < 0 || gridY > 19)
				{
					return false;
				}

				if (game->blocks[gridX][gridY] == 1)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool should_game_end(game_t *game)
{
	for (int i = 0; i < 10; i++)
	{
		if (game->blocks[i][1] != 0)
		{
			game->end_screen->score = game->score;
			game->state = GAME_OVER;
			game->should_update = true;
			game->end_screen->should_update = true;
			return true;
		}
	}
	return false;
}

void game_reset(game_t *game)
{
	game->piece = generate_random_piece(game);
	game->last_time = HAL_GetTick();
	game->last_fall_time = HAL_GetTick();
	game->should_update = false;
	game->should_fall = false;
	game->score = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			game->blocks[i][j] = 0;
		}
	}
}

void game_loop(game_t *game)
{
	switch (game->state)
	{
	case MAIN_MENU:
		render_menu_main(game->main_menu);
		break;
	case PLAYING:
		game_tick(game);
		break;
	case GAME_OVER:
		render_end_screen(game->end_screen);
		break;
	case AUTHORS:
		render_authors(game->authors_menu);
		break;
	case SAVE_SCORE:
		render_save_score_screen(game->save_score_screen, game->score);
		break;
    case HIGH_SCORES:
        render_leaderboard(game->leaderboard, game->high_scores);
        break;
	default:
		break;
	}
}


void Flash_Write(uint32_t address, uint64_t data) {
    HAL_FLASH_Unlock();

    uint64_t doubleWordData = ((uint64_t)data);

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, doubleWordData) != HAL_OK) {
        printf("Błąd podczas zapisu do Flash!\r\n");
    }

    HAL_FLASH_Lock();
}

uint32_t Flash_Read(uint32_t address) {
    uint64_t storedValue = *(volatile uint64_t*)address;

    if (storedValue == 0xFFFFFFFFFFFFFFFF) {
        return 0;
    }

    return (uint32_t)storedValue;
}

uint32_t GetPage(uint32_t Addr)
{
    return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}

void save_high_score(const char* name, uint32_t score) 
{
    high_score_t scores[MAX_SCORES];

    // Read existing scores from flash memory.
    memcpy(scores, (const void*)FLASH_USER_START_ADDR, sizeof(scores));

    // Replace erased entries (filled with 0xFF values) with default values.
    for (int i = 0; i < MAX_SCORES; i++) {
        if ((scores[i].score == 0xFFFFFFFF) || ((unsigned char)scores[i].name[0] == 0xFF)) {
            scores[i].score = 0;
            scores[i].name[0] = '\0';
        }
    }

    // Create new entry.
    high_score_t new_score;
    strncpy(new_score.name, name, NAME_LENGTH);
    new_score.name[NAME_LENGTH] = '\0';
    new_score.score = score;

    // Find insertion point.
    int insert_at = MAX_SCORES;
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i].score) {
            insert_at = i;
            break;
        }
    }



    // Shift lower scores down and insert new score if applicable.
    if (insert_at < MAX_SCORES) {
        for (int i = MAX_SCORES - 1; i > insert_at; i--) {
            scores[i] = scores[i - 1];
        }
        scores[insert_at] = new_score;
    }

    // Erase the flash page before writing new data.
    Flash_ErasePage(FLASH_USER_START_ADDR);

    // Write the scores data to flash memory using 64-bit (double word) writes.
    uint64_t *pScores = (uint64_t *)scores;
    uint32_t numDoubleWords = sizeof(scores) / 8;
    for (uint32_t i = 0; i <= numDoubleWords; i++) {
        Flash_Write(FLASH_USER_START_ADDR + i * 8, pScores[i]);
    }
}

void Flash_ErasePage(uint32_t address) {
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;
    EraseInitStruct.NbPages = 1;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
        printf("Błąd podczas kasowania Flash!\r\n");
    }
    HAL_FLASH_Lock();
}

void read_high_scores(high_score_t* scores)
{
    // Read the entire high scores block from flash memory.
    memcpy(scores, (const void*)FLASH_USER_START_ADDR, sizeof(high_score_t) * MAX_SCORES);

    // Replace any erased entries (filled with 0xFF) with default values.
    for (int i = 0; i < MAX_SCORES; i++) {
        if ((scores[i].score == 0xFFFFFFFF) || ((unsigned char)scores[i].name[0] == 0xFF)) {
            scores[i].score = 0;
            scores[i].name[0] = '\0';
        }
    }
}

HAL_StatusTypeDef clear_flash_memory(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    
    HAL_FLASH_Unlock();
    
    // Calculate pages to erase between FLASH_USER_START_ADDR and FLASH_USER_END_ADDR
    uint32_t start_page = GetPage(FLASH_USER_START_ADDR);
    uint32_t end_page = GetPage(FLASH_USER_END_ADDR - 1);
    uint32_t nb_pages = end_page - start_page + 1;
    
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks     = FLASH_BANK_2;
    EraseInitStruct.Page      = start_page;
    EraseInitStruct.NbPages   = nb_pages;
    
    status = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    
    HAL_FLASH_Lock();
    
    return status;
}