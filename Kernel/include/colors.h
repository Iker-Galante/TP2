#ifndef COLORS_H
#define COLORS_H

#define RED ((Color){255, 0, 0})
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define YELLOW 0xFFFF00
#define ORANGE 0xFFA500

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

#endif
