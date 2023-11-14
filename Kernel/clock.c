#include <clock.h>
#include <lib.h>

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAYS 0x07
#define MONTH 0x08
#define YEAR 0x09

extern unsigned char clock(unsigned char mode); // assembly function

static unsigned int decode(unsigned char time) {
    return (time >> 4) * 10 + (time & 0x0F); //Guardo 4 bits mas significativos y guardo 4 bits menos significativos
    //Luego multiplico por 10 pues el tiempo va de 0 a 9 y sumo
}

unsigned int seconds() {
    return decode(clock(SECONDS));
}

unsigned int minutes() {
    return decode(clock(MINUTES));
}

unsigned int hours() {
    return decode(clock(HOURS));
}

unsigned int day() {
    return decode(clock(DAYS));
}

unsigned int month() {
    return decode(clock(MONTH));
}

unsigned int year() {
    return decode(clock(YEAR));
}
//Las dos funciones reciben el string y por consecuente lo modifican
void timeToStr(char* dest) {
    dest[2] = dest[5] = ':'; //Divido para minutos y segundos
    uint8_t s, m, h = hours(); //Obtengo la hora
    dest[0] = (h/10) % 10 + '0';
    dest[1] = h % 10 + '0';
    m = minutes(); //Obtengo los minutos
    dest[3] = (m/10) % 10 + '0';
    dest[4] = m % 10 + '0';
    s = seconds(); //Obtengo los segundos
    dest[6] = (s/10) % 10 + '0';
    dest[7] = s % 10 + '0';
}

void dateToStr(char* dest) {
    dest[2] = dest [5] = '/'; //Separo los dias, meses y años
    uint8_t d = day(), m, y; //Obtengo el dia
    dest[0] = (d/10) % 10 + '0';
    dest[1] = d % 10 + '0';
    m = month(); //Obtengo el mes
    dest[3] = (m/10) % 10 + '0';
    dest[4] = m % 10 + '0';
    y = year(); //Obtengo el año
    dest[6] = (y/10) % 10 + '0';
    dest[7] = y % 10 + '0';
}