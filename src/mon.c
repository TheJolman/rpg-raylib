#include "mon.h"
#include "debug.h"
#include "raylib.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void loadOneTexture(char *imagePath, Texture2D *texture)
{
    Image image = LoadImage(imagePath);
    *texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void load_mon_texture(Mon *mon, MonTextureType textureType)
{
    char imagePath[256];
    Image image = {};
    if (!mon->name)
    {
        error_exit(1, "attempted to load texture of mon with no name");
    }

    switch (textureType)
    {
    case FRONT:
        snprintf(imagePath, sizeof(imagePath), "assets/%s-front.png", mon->name);
        break;
    case BACK:
        snprintf(imagePath, sizeof(imagePath), "assets/%s-back.png", mon->name);
        break;
    }

    image = LoadImage(imagePath);
    *mon->texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Result create_mon(char *name)
{
    Mon *mon = malloc(sizeof(Mon));
    if (!mon)
    {
        return (Result){.value = nullptr, .err = "Out of memory "};
    }

    mon->name = malloc(strlen(name) + 1);
    if (!mon->name)
    {
        return (Result){.value = nullptr, .err = "Out of memory "};
    }
    strcpy((char *)mon->name, name);
    mon->texture = malloc(sizeof(Texture2D));
    if (!mon->texture)
    {
        return (Result){.value = nullptr, .err = "Out of memory "};
    }
    mon->hp = 100;
    // TODO: Initialize other values

    return (Result){.value = mon, .err = nullptr};
}

void destroy_mon(Mon *mon)
{
    if (mon)
    {
        if (mon->texture)
        {
            UnloadTexture(*mon->texture);
            free(mon->texture);
        }
        if (mon->name)
        {
            free((char *)mon->name);
        }
        free(mon);
    }
}
