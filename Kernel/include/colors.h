#ifndef COLORS_H
#define COLORS_H

#define RED ((Color){0xFF, 00, 00})
#define GREEN ((Color){0x00, 0xFF, 00})
#define BLUE ((Color){0x00, 00, 0xFF})
#define WHITE ((Color){0xFF, 0xFF, 0xFF})
#define BLACK ((Color){0x00, 00, 00})
#define YELLOW ((Color){0xFF, 0xFF, 00})
#define ORANGE ((Color){0xFF, 0xA5, 00})

typedef struct colors
{
    char r, g, b;
}Color;


#endif
