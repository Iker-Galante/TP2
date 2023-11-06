#include <colors.h>
#include <time.h>

#define WIDTH 70
#define HEIGHT 70
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


void singlePlayerSnake();
void mpSnake();

int rand() {
    time_t tiempo;
    time(&tiempo);
    unsigned int semilla = (unsigned int)tiempo;
    
    // Usa la semilla para generar un número pseudoaleatorio en el rango [0, 70).
    unsigned int numeroAleatorio = (semilla % 68) + 1;

}

int finish;
int foodPosX, foodPosY;

struct Position{
    int x;
    int y;
};

typedef struct Player{
    int currentX;
    int currentY;
    int alive;
    int body;
    int direction;
    int length;
    //Revisar Color color;
    struct Position position[MAXDIM];
}Player;




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

void createFood(char snake[HEIGHT][WIDTH], int *foodPosX, int *foodPosY){
    do{
        *foodPosX = rand();
        *foodPosY = rand(); 
    } while (snake[*foodPosY][*foodPosX] != '0');

    game[*foodPosY][*foodPosX] = "#";
}

void initializeGame(char snake[HEIGHT][WIDTH], Player *player){
    player->currentX = WIDTH/2;
    player->currentY = HEIGHT/2;
    player->direction = PLAYER1_LEFT;
    player->alive = 1;
    player->symbol = "*";
   //revisar color player->playerColor
    player->length = 3;

    snake[player->actualY][player->actualX] = player->symbol;

    int i,j;

    for(i = 0; i < HEIGHT ; i++{
        for (j = 0 ; j < WIDTH; j++){
            snake[i][j] = '0'; //revisar falopeada de iker 
        }
    }

    createFood(snake,&foodPosX,&foodPosY);
}


void playerInput(struct Player *player,char up, char down, char left, char right) {
    char key;
    key = getChar();

    if (key == up && player->direction != down) {
        player->direction = up;
    } else if (key == down && player->direction != up) {
        player->direction = down;
    } else if (key == left && player->direction != right) {
        player->direction = left;
    } else if (key == right && player->direction != left) {
        player->direction = right;
    }
}

void eraseTail(char board[HEIGHT][WIDTH], int i, int j) {
    board[i][j] = '0';
}

//revisar eraseTail call
void snakeMovement(char snake[HEIGHT][WIDTH], Player *player) {
    if (player->length >= 1) {
        eraseTail(snake, player->position[player->length - 1].x, player->position[player->length - 1].y);
        for (int i = player->length - 1; i > 0; i--) {
            player->position[i] = player->position[i - 1];
        }
    }
}


////////// 1 player startup //////////
int snake(){
    sys_clean_screen();
    singlePlayerSnake();
}


void singlePlayerSnake(){
    char snake[HEIGHT][WIDTH];

    Player player;

    initializeGame(snake,&player);

    int flag = 0;

    while(!flag){
        playerInput(&player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        snakeMovement(snake,&player);
    }
}
