#include "mon.h"
#include "raylib.h"
// #include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void loadOneTexture(char *imagePath, Texture2D *texture)
{
    Image image = LoadImage(imagePath);
    *texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void loadMonTexture(Mon *mon, MonTextureType textureType)
{
    char imagePath[256];
    Image image = {};
    auto basePath = "assets/";
    strcpy(imagePath, basePath);
    if (!mon->name)
    {
        fprintf(stderr, "ERROR: Can not load texture of mon with no name");
        exit(1);
    }
    strcat(imagePath, mon->name);

    switch (textureType)
    {
    case FRONT:
        strcat(imagePath, "-front.png");
        break;
    case BACK:
        strcat(imagePath, "-back.png");
        break;
    }

    image = LoadImage(imagePath);
    *mon->texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Mon *createMon(char *name)
{
    Mon *mon = malloc(sizeof(Mon));
    mon->name = malloc(strlen(name) + 1);
    strcpy((char*)mon->name, name);
    mon->texture = malloc(sizeof(Texture2D));
    mon->hp = 100;
    // TODO: Initialize other values
    return mon;
}

void destroyMon(Mon *mon) {
    if (mon) {
        if (mon->texture) {
            UnloadTexture(*mon->texture);
            free(mon->texture);
        }
        if (mon->name) {
            free((char*)mon->name);
        }
        free(mon);
    }
}
