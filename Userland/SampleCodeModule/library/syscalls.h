#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
uint64_t sys_print(unsigned int fd,char* buffer,unsigned int size);
uint64_t sys_print_color(unsigned int fd,char* buffer,unsigned int size,uint64_t color);
uint64_t sys_clean_screen();

#endif