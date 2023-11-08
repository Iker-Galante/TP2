#include <stdint.h>
#include "include/usyscalls.h"
#include "include/drawings.h"

void drawBox(int x, int x2, int y, int y2, uint64_t color){
    sys_draw_rectangle(x, y, x2, y2, color);
}