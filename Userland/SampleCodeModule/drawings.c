#include <stdint.h>
#include "include/usyscalls.h"
#include "include/drawings.h"

void drawBox(int x, int y,uint32_t color){
    sys_draw_rectangle(x, y,color);
}