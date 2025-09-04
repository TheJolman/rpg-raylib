/**
 * @file battle.c
 * @brief Battle system implementation
 */

#include "battle.h"
#include "map.h"
#include "menu.h"
#include "mon.h"
#include "raylib.h"
#include <assert.h>
#include <stdint.h>

// Should these be moved into initBattleUI ?
constexpr uint32_t WINDOW_MARGIN = 50;
constexpr uint32_t TEXT_HEIGHT = 150;
constexpr Color TINT = WHITE;
constexpr float ROTATION = 0.0;
constexpr float SCALE = 0.6f;
constexpr uint32_t NUM_ITEMS = 4;
constexpr uint32_t NUM_ROWS = 2;
constexpr uint32_t NUM_COLS = 2;

typedef struct
{
    Rectangle text_box;
    Vector2 player_mon_pos;
    Vector2 enemy_mon_pos;
    Vector2 action_menu_pos;
    Vector2 status_bar_pos;
} BattleUI;

typedef enum
{
    BATTLE_MENU,
    BATTLE_ATTACK,
    BATTLE_ITEMS,
    BATTLE_RUN,
    BATTLE_SWITCH,
} BattleState;

static BattleState battle_state = BATTLE_MENU;

// clang-format off
static void attack_select() { battle_state = BATTLE_ATTACK; }
static void items_select()  { battle_state = BATTLE_ITEMS; }
static void run_select()    { battle_state = BATTLE_RUN; }
static void switch_select() { battle_state = BATTLE_SWITCH; }

static MenuItem attack_item = {"ATTACK", 0, 0, 20, DARKGRAY, attack_select};
static MenuItem items_item =  {"ITEMS",  0, 0, 20, DARKGRAY, items_select};
static MenuItem run_item =    {"RUN",    0, 0, 20, DARKGRAY, run_select};
static MenuItem switch_item = {"SWITCH", 0, 0, 20, DARKGRAY, switch_select};
// clang-format on

Mon *player_mon = nullptr;
Mon *enemy_mon = nullptr;
BattleUI ui = {0};
static bool battle_initialized = false;

static MenuItem action_items[NUM_ROWS][NUM_COLS];

static void update_menu_item_positions()
{
    float menu_start_x = ui.text_box.x + ui.text_box.width * 0.5f + 10;
    float menu_start_y = ui.text_box.y + 30;
    float item_spacing = 35;

    attack_item.pos_x = menu_start_x;
    attack_item.pos_y = menu_start_y;

    items_item.pos_x = menu_start_x + 120;
    items_item.pos_y = menu_start_y;

    run_item.pos_x = menu_start_x;
    run_item.pos_y = menu_start_y + item_spacing;

    switch_item.pos_x = menu_start_x + 120;
    switch_item.pos_y = menu_start_y + item_spacing;

    action_items[0][0] = attack_item;
    action_items[0][1] = items_item;
    action_items[1][0] = run_item;
    action_items[1][1] = switch_item;
}

GridMenu *action_menu = nullptr;
GridMenu *attack_menu = nullptr;
GridMenu *items_menu = nullptr;
GridMenu *switch_menu = nullptr;

static GridMenu *action_menu_create()
{
    GridMenu *menu = grid_menu_create(NUM_ITEMS, NUM_ROWS, NUM_COLS);
    if (!menu)
    {
        return nullptr;
    }

    for (uint32_t row = 0; row < NUM_ROWS; row++)
    {
        for (uint32_t col = 0; col < NUM_COLS; col++)
        {
            uint32_t index = row * NUM_COLS + col;
            if (index >= menu->num_items)
            {
                grid_menu_destroy(menu);
                return nullptr;
            }
            menu->items[index] = &action_items[row][col];
        }
    }
    return menu;
}

static void action_menu_end()
{
    if (action_menu)
        grid_menu_destroy(action_menu);

    action_menu = nullptr;
}

static void action_menu_display()
{
    if (!action_menu)
    {
        action_menu = action_menu_create();
        if (!action_menu)
            return;
    }

    float menu_title_x = ui.text_box.x + ui.text_box.width * 0.5f + 10;
    float menu_title_y = ui.text_box.y + 5;
    DrawText("BATTLE MENU", menu_title_x, menu_title_y, 18, WHITE);

    DrawLine(ui.text_box.x + ui.text_box.width * 0.5f, ui.text_box.y,
             ui.text_box.x + ui.text_box.width * 0.5f, ui.text_box.y + ui.text_box.height, GRAY);
    for (uint32_t i = 0; i < action_menu->grid.num_rows; i++)
    {
        for (uint32_t j = 0; j < action_menu->grid.num_cols; j++)
        {
            MenuItem item = action_items[i][j];
            if (item.text)
            {
                DrawText(item.text, item.pos_x, item.pos_y, item.font_size, item.color);
            }
        }
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        if (action_menu->move_down)
            action_menu->move_down(action_menu);
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        if (action_menu->move_up)
            action_menu->move_up(action_menu);
    }
    else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        if (action_menu->move_left)
            action_menu->move_left(action_menu);
    }
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        if (action_menu->move_right)
            action_menu->move_right(action_menu);
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        if (action_menu->grid.current_row < action_menu->grid.num_rows &&
            action_menu->grid.current_col < action_menu->grid.num_cols)
        {
            uint32_t index = action_menu->grid.current_row * action_menu->grid.num_cols +
                             action_menu->grid.current_col;
            if (index < action_menu->num_items && action_menu->items[index]->select)
            {
                action_menu->items[index]->select();
                action_menu_end();
                return;
            }
        }
    }

    if (action_menu->grid.current_row < NUM_ROWS && action_menu->grid.current_col < NUM_COLS)
    {
        MenuItem current_item =
            action_items[action_menu->grid.current_row][action_menu->grid.current_col];
        constexpr int WIDTH = 120;
        constexpr int HEIGHT = 30;
        DrawRectangleLines(current_item.pos_x - 10, current_item.pos_y - 5, WIDTH, HEIGHT,
                           DARKGRAY);
    }
}

static void init_battle_ui(void)
{
    ui.text_box = (Rectangle){WINDOW_MARGIN, screen.height - (WINDOW_MARGIN + TEXT_HEIGHT),
                              screen.width - WINDOW_MARGIN * 2, TEXT_HEIGHT};

    // TODO: Get consistent asset sizes
    ui.player_mon_pos = (Vector2){screen.width * 0.6f, screen.height * 0.35f};
    ui.enemy_mon_pos = (Vector2){screen.width * 0.05f, screen.height * 0.1f};
    ui.action_menu_pos = (Vector2){ui.text_box.x + 20, ui.text_box.y + 20};
    ui.status_bar_pos = (Vector2){ui.text_box.x + 20, ui.text_box.y + 80};

    update_menu_item_positions();
    battle_state = BATTLE_MENU;

    if (!player_mon)
    {
        Result res = create_mon("froge");
        if (res.err)
        {
            error_exit(1, "%s", res.err);
        }
        else
        {
            player_mon = (Mon *)res.value;
        }
        player_mon->hp = 100;
        load_mon_texture(player_mon, BACK);
    }

    if (!enemy_mon)
    {
        Result res = create_mon("froge");
        if (res.err)
        {
            error_exit(1, "%s", res.err);
        }
        else
        {
            enemy_mon = (Mon *)res.value;
        }
        enemy_mon->hp = 80;
        load_mon_texture(enemy_mon, FRONT);
    }
}

static void render_mon(Mon *mon, Vector2 position)
{
    if (!mon)
        return;

    if (!mon->texture)
        return;

    if (!IsTextureValid(*mon->texture))
        return;

    DrawTextureEx(*mon->texture, position, ROTATION, SCALE, TINT);
}

static void render_text_box(void)
{
    DrawRectangleLines(ui.text_box.x, ui.text_box.y, ui.text_box.width, ui.text_box.height, WHITE);
}

static void render_action_menu(void)
{
    switch (battle_state)
    {
    case BATTLE_MENU:
        action_menu_display();
        break;
    case BATTLE_ATTACK:
        // attack_menuDisplay();
        break;
    case BATTLE_ITEMS:
        // items_menuDisplay();
        break;
    case BATTLE_RUN:
        // runMenuDisplay();
        break;
    case BATTLE_SWITCH:
        // switch_menuDisplay();
        break;
    }
}

static void render_battle_ui(void)
{
    render_text_box();
    render_action_menu();

    if (player_mon)
        render_mon(player_mon, ui.player_mon_pos);

    if (enemy_mon)
        render_mon(enemy_mon, ui.enemy_mon_pos);
}

void battle_scene_render(void)
{
    if (!battle_initialized)
    {
        init_battle_ui();
        battle_initialized = true;
    }

    DrawText("Battle scene is active!\nPress B to go back!", 50, 50, 20, DARKGRAY);
    render_battle_ui();
}

void battle_scene_end(void)
{
    if (player_mon)
    {
        destroy_mon(player_mon);
        player_mon = nullptr;
    }

    if (enemy_mon)
    {
        destroy_mon(enemy_mon);
        enemy_mon = nullptr;
    }

    action_menu_end();
    battle_state = BATTLE_MENU;
    battle_initialized = false;
}
