/*******************************************************************************************
 *
 *   VGDC Game Project
 *
 *   <Game title>
 *   <Game description>
 *
 *   This game has been created using raylib (www.raylib.com)
 *
 *   Copyright (c) 2025 Joshua Holman
 *
 ********************************************************************************************/

/**
 * @file main.c
 * @brief Main entry point and window initialization
 */

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "game.h"
#include "map.h"
#include "utils.h"

#ifndef GIT_VERSION
#define GIT_VERSION "unknown"
#endif

uint32_t screen_width = 1200;
uint32_t screen_height = 900;

Screen screen = {0};

/**
 * @returns true if target matches short_arg or long_arg.
 * @returns false otherwise
 * If only short_arg or long_arg is needed, pass nullptr in place of the other.
 */
bool match_arg(const char *target, const char *short_arg, const char *long_arg)
{
    return (short_arg && strcmp(target, short_arg) == 0 ||
            long_arg && strcmp(target, long_arg) == 0);
}

int main(int argc, const char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (match_arg(argv[i], "-v", "--version"))
        {
            printf("%s %s\n", argv[0], GIT_VERSION);
            exit(0);
        }
        else if (match_arg(argv[i], "-h", "--help"))
        {
            printf("Usage: %s [-h | --help] [-v | --version]\n"
                   "Have fun!\n",
                   argv[0]);
            exit(0);
        }
        else
        {
            error_exit(1,
                       "Unrecognized argument. "
                       "Try '%s --help' for usage information.",
                       argv[0]);
        }
    }

    screen.width = screen_width;
    screen.height = screen_height;

    debug_log("Game initiated with screen dimensions %dx%d", screen.width, screen.height);

    InitWindow(screen.width, screen.height, "Game!");
    if (!IsWindowReady())
    {
        error_exit(1, "failed to initialize window");
    }
    SetTargetFPS(60);
    init_game();
    SetExitKey(KEY_NULL);

    const char *file_path = "assets/map.csv";
    Result res = map_load_from_csv(file_path);
    if (res.err)
    {
        error_exit(1, "%s", res.err);
    }
    Map *map = (Map *)res.value;
    debug_log("Map loaded with %u rows and %u cols", map->height, map->width);

    // Main game loop
    while (!WindowShouldClose())
    {
        update_game();
        draw_game(map);
    }

    free(map->data);
    free(map);

    CloseWindow();
    return 0;
}
