#include <stdio.h>
#include <keyboard.h>
#include <naiveConsole.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE] = {0};
static int writeIndex = 0;
static int readIndex = 0; // indices para mi buffer circular

static char shiftPressed = 0;
static char capsLocked = 0;

static const char charHexMap[256] = {
    0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\', '+', '\n',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, 0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char isLetter(unsigned char key) {
    char c = charHexMap[key];
    return (c >= 'a' && c <= 'z');
}

void addToBuffer(char character) {
    if (writeIndex >= BUFFER_SIZE) {
        writeIndex = 0; // Wrap around the circular buffer
    }

    buffer[writeIndex++] = character;
}

char getFromBuffer() {
    if (readIndex >= BUFFER_SIZE) {
        readIndex = 0; // Wrap around the circular buffer
    }
    return buffer[readIndex++];
}

void keyboard_handler() {
    unsigned char key = getKey();
    if (key < 83 || key == 0xAA /* Release SHIFT */ || key == 0x3A /* CAPS Lock */) {
        if (writeIndex >= BUFFER_SIZE) return; // Buffer is full

        if (charHexMap[key] == 5 && !shiftPressed) { // Shift key
            shiftPressed = 1;
        } else if (key == 0xAA) { // Shift released
            shiftPressed = 0;
        } else if (key == 0x3A) { // Caps Lock
            capsLocked = !capsLocked;
        } else {
            char character = charHexMap[key];
            if (isLetter(key)) {
                if ((shiftPressed && !capsLocked) || (!shiftPressed && capsLocked)) {
                    addToBuffer(character - 'a' + 'A');
                } else {
                    addToBuffer(character);
                }
            } else {
                if (shiftPressed) {
                    addToBuffer(charHexMap[key]);
                } else {
                    addToBuffer(character);
                }
            }
        }
    }
}

char getChar() {
    if (readIndex == writeIndex) {
        return 0; // Buffer is empty
    }
    return getFromBuffer();
}
