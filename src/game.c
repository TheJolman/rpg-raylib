/**
 * @file game.c
 * @brief Core game logic implementation
 */

#include <raylib.h>
#include <raymath.h>

#include "battle.h"
#include "game.h"
#include "map.h"
#include "pause.h"
// #include "utils.h"

Player player = {0};
Camera2D camera = {0};
GameState state = {0};

static void move_player(void)
{
    // Movement
    float moveSpeedModifier = 1.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        moveSpeedModifier = 2.0f;
    }

    player.speed = player.base_speed * moveSpeedModifier;
    Vector2 moveDirection = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W))
        moveDirection.y -= 1.0f;
    if (IsKeyDown(KEY_S))
        moveDirection.y += 1.0f;
    if (IsKeyDown(KEY_A))
        moveDirection.x -= 1.0f;
    if (IsKeyDown(KEY_D))
        moveDirection.x += 1.0f;

    if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
    {
        moveDirection = Vector2Normalize(moveDirection);
    }

    // Calculate new position
    Vector2 newPosition = {player.position.x + moveDirection.x * player.speed,
                           player.position.y + moveDirection.y * player.speed};

    player.position = newPosition;

    // Camera
    camera.target = player.position;
}

static void handle_input(void)
{
    switch (state)
    {
    case FREE_ROAM:
        move_player();
        if (IsKeyPressed(KEY_B))
            state = BATTLE_SCENE;
        if (IsKeyPressed(KEY_ESCAPE))
            state = PAUSED;
        break;

    case BATTLE_SCENE:
        if (IsKeyPressed(KEY_B))
        {
            // NOTE: If concurencey is ever added should these be switched?
            battle_scene_end();
            state = FREE_ROAM;
        }
        break;

    case PAUSED:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            pause_menu_end();
            state = FREE_ROAM;
        }
        break;

    case TITLE_SCREEN:
        break;
    }

    // Unconditional inputs
    if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))
    {
        if (IsKeyPressed(KEY_F4))
            CloseWindow();
    }
}

void init_game(void)
{
    state = FREE_ROAM;

    player.position = (Vector2){(float)screen.width / 2, (float)screen.height / 2};
    player.base_speed = 5.0f;
    player.speed = player.base_speed;
    player.size = 30;

    camera.target = player.position;
    camera.offset = (Vector2){screen.width / 2.0f, screen.height / 2.0f};
    camera.zoom = 1.0f;
}

void update_game(void) { handle_input(); }

void draw_game(Map *map)
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (state)
    {
    case FREE_ROAM:
        BeginMode2D(camera);

        map_draw(map);
        // Draw player
        DrawCircleV(player.position, player.size / 2, RED);

        EndMode2D();
        DrawText("Press B to enter the Battle Scene!", 50, 50, 20, DARKGRAY);
        break;
    case BATTLE_SCENE:
        battle_scene_render();
        break;
    case PAUSED:
        pause_menu_display();
        break;
    case TITLE_SCREEN:
        break;
    }

    EndDrawing();
}

void cleanup_game(void) {}
