/*
 * game.h
 *
 *  Created on: Jan 20, 2025
 *      Author: nikutsuki
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <stdbool.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "piece.h"
#include "main_menu.h"
#include "authors.h"
#include "end_screen.h"
#include "save_score_screen.h"
#include "leaderboard.h"

#define PLAYING_FIELD_HEIGHT 20
#define PLAYING_FIELD_WIDTH 10

#define SCORE_OFFSET 0
#define SCORE_HEIGHT 20

#define FALL_SPEED 6
#define SIMULATION_SPEED 500

// New Flash Definitions
#define FLASH_USER_START_ADDR   0x08080000U
#define FLASH_USER_END_ADDR     0x08083FFFU
#define FLASH_PAGE_SIZE         2048U

typedef enum {
    MAIN_MENU = 0,
    PLAYING = 1,
    GAME_OVER = 2,
    AUTHORS = 3,
    SAVE_SCORE = 4,
    HIGH_SCORES = 5
} GAME_STATE;

typedef struct game {
    piece_t* piece;
    int blocks[PLAYING_FIELD_WIDTH][PLAYING_FIELD_HEIGHT];
    RNG_HandleTypeDef hrng;
    uint32_t last_time;
    uint32_t last_fall_time;
    bool should_update;
    bool should_fall;
    int score;
    main_menu_t* main_menu;
    end_screen_t* end_screen;
    authors_menu_t* authors_menu;
    save_scores_screen_t* save_score_screen;
    leaderboard_t* leaderboard;
    high_score_t high_scores[MAX_SCORES];
    GAME_STATE state;
    char name[NAME_LENGTH + 1];
} game_t;

void render_border(game_t* game);
piece_t* generate_random_piece(game_t* game);
void game_init(game_t* game, RNG_HandleTypeDef hrng);
void game_tick(game_t* game);
void check_collision(game_t* game);
void convert_piece_to_blocks(game_t* game);
void render_blocks(game_t* game);
void delete_game_piece(game_t* game);
void keyboard_input(game_t* game, char input);
void next_tick(game_t* game, uint32_t current_time);
void break_lines(game_t* game);
bool is_valid_move(game_t* game, piece_t* piece, int x, int y);
bool should_game_end(game_t* game);
void game_reset(game_t* game);
void game_loop(game_t* game);
HAL_StatusTypeDef Flash_Write_Data(uint32_t StartPageAddress, uint64_t *Data, uint16_t numberofwords);
void Flash_Read_Data(uint32_t StartPageAddress, uint64_t *Data, uint16_t numberofwords);
uint32_t GetPage(uint32_t Addr);
void save_high_score(const char* name, uint32_t score);
void read_high_scores(high_score_t* scores);
HAL_StatusTypeDef clear_flash_memory(void);

// New Flash Function Prototypes
void Flash_ErasePage(uint32_t address);
void Flash_Write(uint32_t address, uint64_t data);
uint32_t Flash_Read(uint32_t address);

#endif /* INC_GAME_H_ */
