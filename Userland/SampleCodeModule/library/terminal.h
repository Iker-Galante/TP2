#ifndef TERMINAL_H
#define TERMINAL_H

extern void invalidOperationCode();
extern void divbyZero();

void terminal();
void checkBuffer(char *buffer,int count);
int commandMatch(char * str1, char * command, int count);
void analizeBuffer(char *buffer, int count);
#endif