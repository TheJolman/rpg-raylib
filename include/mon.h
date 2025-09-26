/**
 * @file mon.h
 * @brief Monster/Pokemon system definitions and functions
 */

#pragma once

#include "components.h"
#include "utils.h"

/**
 * @brief Texture type for monster sprites
 */
typedef enum
{
    FRONT, ///< Front-facing sprite (opponent's view)
    BACK   ///< Back-facing sprite (player's view)
} MonSpriteView;

/**
 * @brief The "type" of a monster or move. Currenly one of 'rock', 'paper', or 'scissors'
 */
typedef enum
{
    TYPE_ROCK,
    TYPE_PAPER,
    TYPE_SCISSORS,
} MonType;

/**
 * @brief A move/attack that a monster can perform
 */
typedef struct
{
    const char *name;       ///< Name of the move
    const HitPoints damage; ///< Damage dealt by this move
    MonType type;
} Move;

/**
 * @brief Monster/Pokemon data structure
 */
typedef struct
{
    const char *name;          ///< Monster's name
    Sprite sprite;             ///< Monster's sprite
    Position position;         ///< Position to render sprite
    MonSpriteView sprite_view; ///< Determines if we get the front or back view
    Move moves[4];             ///< Array of up to 4 moves
    Health health;             ///< Max and current HP
    MonType type;
} Mon;

/**
 * @brief Creates a new monster with the given name
 *
 * @param name The name of the monster to create
 * @param level The level of the monster to create
 * @return Result containing the created monster or error information
 */
Result create_mon(char *name, unsigned level);

/**
 * @brief Destroys a monster and frees its resources
 *
 * @param mon Pointer to the monster to destroy
 */
void destroy_mon(Mon *mon);

/**
 * @brief Loads the appropriate texture for a monster
 *
 * @param mon Pointer to the monster
 * @param textureType Type of texture to load (FRONT or BACK)
 */
void load_mon_texture(Mon *mon, MonSpriteView textureType);
