#include <stdbool.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"

typedef struct {
    bool should_update;
} authors_menu_t;

void render_authors(authors_menu_t* authors_menu);
void init_authors(authors_menu_t* authors_menu);