#include <colors.h>

#define WIDTH 40
#define HEIGHT 40
#define MAXDIM 200

#define PIXELWIDTH (get_scrWidht() / WIDTH)
#define PIXELHEIGHT (get_scrHeight() / HEIGHT)
#define MAX_BUFFER 254

#define PLAYER1_UP 'w'
#define PLAYER1_DOWN 's'
#define PLAYER1_LEFT 'a'
#define PLAYER1_RIGHT 'd'

#define PLAYER2_UP 'i'
#define PLAYER2_DOWN 'k'
#define PLAYER2_LEFT 'j'
#define PLAYER2_RIGHT 'l'


static unsigned long int next = 1;

int rand() {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

int finish;
int foodPosX, foodPosY;

struct Position{
    int x;
    int y;
};

struct Player{
    int currentX;
    int currentY;
    int alive;
    int body;
    int direction;
    int length;
    Color color;
    struct Position position[MAXDIM];
};

void generateFood(char board[HEIGHT][WIDTH], int *foodX, int *foodY) {
    do {
        *foodX = rand() % (WIDTH - 2) + 1;
        *foodY = rand() % (HEIGHT - 2) + 1;
    } while (board[*foodY][*foodX] != ' ');    

    board[*foodY][*foodX] = '*';
}

void eraseTail(char board[HEIGHT][WIDTH], int i, int j) {
    board[i][j] = ' ';
}

void drawBoard(char board[HEIGHT][WIDTH], struct Player *player) {
    Color currentColor;
    int i, j;
    
    // Recorre las filas y columnas del tablero
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Si la celda está vacía, establece el color de fondo en blanco
            if (board[i][j] == ' ') {
                currentColor = WHITE;
            }
            // Si la celda contiene al jugador, usa su color
            else if (i == player-> currentY && j == player-> currentX) {
                currentColor = player-> color;
            }
            // Si la celda contiene comida, usa un color específico
            else if (board[i][j] == '*') {
                currentColor = RED;
            }
            // Dibuja un rectángulo en la posición actual con el color correspondiente
            drawRectangle(j * PIXELWIDTH, i * PIXELHEIGHT, PIXELWIDTH - 1, PIXELHEIGHT - 1, currentColor);
        }
    }
}