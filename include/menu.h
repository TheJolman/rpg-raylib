/**
 * @file menu.h
 * @brief Menu system structures and functions
 */

#pragma once

#include "raylib.h"
#include <stdint.h>

/**
 * @brief Individual menu item structure
 */
typedef struct
{
    char *text;           ///< Text to display for this menu item
    int pos_x;            ///< X position for rendering
    int pos_y;            ///< Y position for rendering
    uint16_t font_size;   ///< Font size for text rendering
    Color color;          ///< Text color
    void (*select)(void); ///< Function to call when item is selected
} MenuItem;

/**
 * @brief Grid navigation helper structure
 */
typedef struct
{
    uint16_t num_rows;    ///< Number of rows in the grid
    uint16_t num_cols;    ///< Number of columns in the grid
    uint16_t current_row; ///< Currently selected row
    uint16_t current_col; ///< Currently selected column
} Grid;

/**
 * @brief Menu arranged in a grid layout
 */
typedef struct
{
    void (*move_left)(const void *);  ///< Function to handle left movement
    void (*move_right)(const void *); ///< Function to handle right movement
    void (*move_down)(const void *);  ///< Function to handle down movement
    void (*move_up)(const void *);    ///< Function to handle up movement
    Grid grid;                        ///< Grid navigation state
    uint16_t num_items;               ///< Total number of menu items
    MenuItem *items[];                ///< Array of menu item pointers
} GridMenu;

/**
 * @brief Menu arranged in a vertical list
 */
typedef struct
{
    void (*next_item)(const void *); ///< Function to move to next item
    void (*prev_item)(const void *); ///< Function to move to previous item
    void (*select)(const void *);    ///< Function to select current item
    uint16_t selected_item;          ///< Index of currently selected item
    uint16_t num_items;              ///< Total number of menu items
    MenuItem items[];                ///< Array of menu items
} VerticalMenu;

/**
 * @brief Creates a new grid menu
 *
 * @param num_items Total number of menu items
 * @param num_rows Number of rows in the grid
 * @param num_cols Number of columns in the grid
 * @return Pointer to the created grid menu
 */
GridMenu *grid_menu_create(const uint16_t num_items, const uint16_t num_rows,
                           const uint16_t num_cols);

/**
 * @brief Destroys a grid menu and frees its memory
 *
 * @param menu Pointer to the grid menu to destroy
 */
void grid_menu_destroy(GridMenu *menu);

/**
 * @brief Creates a new vertical menu
 *
 * @param num_items Number of menu items
 * @return Pointer to the created vertical menu
 */
VerticalMenu *vertical_menu_create(const uint16_t num_items);

/**
 * @brief Destroys a vertical menu and frees its memory
 *
 * @param menu Pointer to the vertical menu to destroy
 */
void vertical_menu_destroy(VerticalMenu *menu);
