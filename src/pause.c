#include "pause.h"
#include "assert.h"
#include "menu.h"
#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>

constexpr uint32_t NUM_ITEMS = 3;

static void options_select(void) { return; }
static void exit_select(void)
{
    pause_menu_end();
    CloseWindow();
    exit(0);
}
static void credits_select(void) { return; }

// NOTE: maybe move these into the create function?

// clang-format off
static const MenuItem options_item = {"OPTIONS", 50, 100, 20, DARKGRAY, options_select};
static const MenuItem exit_item =    {"EXIT",    50, 130, 20, DARKGRAY, exit_select};
static const MenuItem credits_item = {"CREDITS", 50, 160, 20, DARKGRAY, credits_select};
// clang-format on

static const MenuItem pause_items[NUM_ITEMS] = {options_item, exit_item, credits_item};

VerticalMenu *pause_menu = nullptr;

static VerticalMenu *pause_menu_create()
{
    VerticalMenu *menu = vertical_menu_create(NUM_ITEMS);
    if (menu)
    {
        for (uint32_t i = 0; i < menu->num_items; i++)
            menu->items[i] = pause_items[i];
    }

    return menu;
}

void pause_menu_display(void)
{
    if (!pause_menu)
    {
        pause_menu = pause_menu_create();
        if (!pause_menu)
            return;
    }

    DrawText("PAUSE MENU", 50, 50, 40, DARKGRAY);

    for (uint32_t i = 0; i < pause_menu->num_items; i++)
    {
        MenuItem item = pause_items[i];
        DrawText(item.text, item.pos_x, item.pos_y, item.font_size, item.color);
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        assert(pause_menu->next_item);
        pause_menu->next_item(pause_menu);
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        assert(pause_menu->prev_item);
        pause_menu->prev_item(pause_menu);
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        pause_menu->items[pause_menu->selected_item].select();
        return;
    }

    MenuItem currentItem = pause_items[pause_menu->selected_item];
    DrawRectangleLines(currentItem.pos_x - 10, currentItem.pos_y - 5, 300, 30, DARKGRAY);
}

void pause_menu_end()
{
    if (pause_menu)
        vertical_menu_destroy(pause_menu);

    pause_menu = nullptr;
}
