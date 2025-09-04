/**
 * @file battle.h
 * @brief Battle system management and rendering
 */

#pragma once

// TODO: Use linked list for managing assets

/**
 * @brief Renders the battle scene interface
 *
 * Draws the battle UI, monsters, and handles battle-specific rendering
 */
void battle_scene_render(void);

/**
 * @brief Cleans up battle scene resources
 *
 * Called when exiting the battle scene to free allocated memory
 */
void battle_scene_end(void);
