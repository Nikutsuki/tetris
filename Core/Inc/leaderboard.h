#include <stdbool.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define NAME_LENGTH 7
#define MAX_SCORES 5

typedef struct {
    char name[NAME_LENGTH + 1];  // +1 for null terminator
    uint64_t score;
} high_score_t;

typedef struct {
    bool should_update;
} leaderboard_t;

void render_leaderboard(leaderboard_t* leaderboard, high_score_t* scores);
void init_leaderboard(leaderboard_t* leaderboard);