#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <stdbool.h>

typedef struct {
    char* name;
    void (*function)();
    int menu_selection;
    bool should_update;
} main_menu_t;

void render_menu_main(main_menu_t* menu);
void init_main_menu(main_menu_t* menu);