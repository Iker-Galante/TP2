#include <sounds.h>
#include <usyscalls.h.>
#include <calls.h>

void playNote(int freq, int duration, int wait){
    sys_play_sound(freq,duration,wait);
}

void snakeDeadSound(){
    playNote(310,6,30);
}