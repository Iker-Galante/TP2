#ifndef COLORS_H
#define COLORS_H

#define RED ((Color){0xFF, 00, 00})
#define GREEN ((Color){0x00, FF, 00})
#define BLUE ((Color){0x00, 00, FF})
#define WHITE ((Color){0xFF, FF, FF})
#define BLACK ((Color){0x00, 00, 00})
#define YELLOW ((Color){0xFF, FF, 00})
#define ORANGE ((Color){0xFF, A5, 00})

typedef struct colors
{
    char r, g, b;
}Color;


#endif
