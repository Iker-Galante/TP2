#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
uint64_t sys_print(unsigned int fd,char* buffer,unsigned int size);
uint64_t sys_print_color(unsigned int fd,char* buffer,unsigned int size,uint64_t color);
uint64_t sys_clean_screen();
uint64_t sys_read(unsigned int fd, char* buffer, unsigned int size);
uint64_t sys_time(char* buffer);
uint64_t sys_date(char* buffer);
uint64_t sys_registers(uint64_t* registerBuffer);
uint64_t sys_IncreasePixel();
uint64_t sys_DecreasePixel();
uint64_t sys_draw_rectangle(int x, int y, int width, int height, int color);

#endif