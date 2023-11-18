#include <time.h>
#include <stdint.h>
#include <usyscalls.h>
#include <colors.h>
#include <calls.h>
#include "include/drawings.h"
#include "include/sounds.h"
#define WIDTH 32
#define HEIGHT 32
#define MAXDIM 1024 //REVISAR

#define PIXELWIDTH 32
#define PIXELHEIGHT 24
#define MAX_BUFFER 254

#define PLAYER1_UP 'w'
#define PLAYER1_DOWN 's'
#define PLAYER1_LEFT 'a'
#define PLAYER1_RIGHT 'd'

#define PLAYER2_UP 'i'
#define PLAYER2_DOWN 'k'
#define PLAYER2_LEFT 'j'
#define PLAYER2_RIGHT 'l'




void singlePlayerSnake();
void mpSnake();

unsigned int t1 = 0x12345678, t2 = 0x87654321;
//utiliza una combinación de operaciones de desplazamiento y XOR para generar un número aleatorio de 32 bits.
unsigned int random_1_to_31() {
    unsigned int b;
    b = ((t1 << 13) ^ t1) >> 19;
    t1 = ((t1 & 4294967294U) << 12) ^ b;
    b = ((t2 << 2) ^ t2) >> 25;
    t2 = ((t2 & 4294967288U) << 4) ^ b;
    return (t1 ^ t2) % 31 + 1;
}

int finish;
int foodPosX, foodPosY;

struct Position{
    int i;
    int j;
};

typedef struct Player{
    int currentX;
    int currentY;
    int alive;
    char body;
    int direction;
    int length;
    uint64_t playerColor;
    struct Position position[MAXDIM];
}Player;


//checkear color y tema del drawrectangle

void drawBoard(char board[HEIGHT][WIDTH], Player *player) {
    int i, j;
    uint32_t currentColor;
    // Recorre las filas y columnas del tablero
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Si la celda está vacía, establece el color de fondo en blanco
            if (board[i][j] == '0') {
                currentColor = 0x00FF00; // Verde
            }
            // Si la celda contiene al jugador, usa su color
            else if (board[i][j] == '*') {
                currentColor = player->playerColor;
            }
            // Si la celda contiene comida, usa un color específico
            else if (board[i][j] == '#') {
                currentColor = 0xFF0000; // Rojo para comida
            }
            // Dibuja en la posición actual con el color correspondiente
            drawBox(j * PIXELWIDTH, i * PIXELHEIGHT, currentColor);
        }
    }
}


void createFood(char snake[HEIGHT][WIDTH], int *foodPosX, int *foodPosY){
    do{
        *foodPosX = random_1_to_31();
        *foodPosY = random_1_to_31(); 
    } while (snake[*foodPosY][*foodPosX] != '0');

    snake[*foodPosY][*foodPosX] = '#';
}

void initializeGame(char snake[HEIGHT][WIDTH], Player *player){ //_hlt para dormir 
    player->currentX = WIDTH/2; //Empiezo en el medio
    player->currentY = HEIGHT/2;
    player->direction = PLAYER1_UP;
    player->alive = 1;
    player->body = '*';
    player->playerColor=0xAAAFFF; //Color blanco
    player->length = 3;
    int i,j;
    snake[player->currentY][player->currentX] = player->body;
    for(i = 0; i < HEIGHT ; i++){
        for (j = 0 ; j < WIDTH; j++){
            snake[i][j] = '0'; 
        }
    }
    
    //snake[player->currentY][player->currentX + 1] = player->body; // REVISAR
    //snake[player->currentY][player->currentX + 2] = player->body;


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
    // Guarda la posición actual de la cabeza de la serpiente
    int headI = player->currentY;
    int headJ = player->currentX;

    // Borra la antigua posición de la cola
    eraseTail(snake, player->position[player->length - 1].i, player->position[player->length - 1].j);

    // Actualiza las posiciones de la cola
    for (int i = player->length - 1; i > 0; i--) {
        player->position[i] = player->position[i - 1];
    }

    // Actualiza la posición de la cabeza
    player->position[0].i = headI;
    player->position[0].j = headJ;
}

void gameLogic(char snake[HEIGHT][WIDTH], Player * player, char up, char down, char left, char right){

    if (player->direction == up) {
        player->currentY--;
    } else if (player->direction == down) {
        player->currentY++;
    } else if (player->direction == left) {
        player->currentX--;
    } else if (player->direction == right) {
        player->currentX++;
    }

    // check self collision
    for (int i = 0; (i < player->length) && player->alive; i++) {
        if (player->currentX == player->position[i].j && player->currentY == player->position[i].i ) {
            player->alive = 0;
        }
    }
    

    // check snake border collision
    if (player-> currentX < 0 || player-> currentX >= WIDTH  || player-> currentY < 0 || player-> currentY >= HEIGHT ) {
        player->alive = 0;
    }
    

    if (!player->alive) {
        finish = 1;
        player->playerColor = BLACK; 
    }

    if (player-> currentX == foodPosX && player->currentY == foodPosY) {
        player->length++;
        createFood(snake, &foodPosX, &foodPosY);
        //sys_playSound(1500);
        //sys_mute(); VER SCRIPT WPP
    }

    if (player->alive) {
        snake[player->currentY][player->currentX] = player->body;

    }
}

void snakeFunctionality(char snake[HEIGHT][WIDTH], Player *player, char up, char down, char left, char right) {
    gameLogic(snake,player,up,down,left,right);

    drawBoard(snake, player);
}

///// IMPLEMENTATION 2 PLAYERS ////

void initializeGameMP(char snake[HEIGHT][WIDTH], Player *player1, Player *player2){

    int i, j;
    for(i = 0; i < HEIGHT ; i++){
        for (j = 0 ; j < WIDTH; j++){
            snake[i][j] = '0';  
        }
    }
    
    player1->currentX = WIDTH/4;
    player1->currentY = HEIGHT/2;
    player1->direction = PLAYER1_LEFT;
    player1->alive = 1;
    player1->body = '*';
    player1->playerColor= BLUE;
    player1->length = 3;

    player2->currentX = 2 * WIDTH/4;
    player2->currentY = HEIGHT/2;
    player2->direction = PLAYER2_UP;
    player2->alive = 1;
    player2->body = '+';
    player2->playerColor=YELLOW;
    player2->length = 3;

    snake[player1->currentY][player1->currentX] = player1->body;
    snake[player2->currentY][player2->currentX] = player2->body;

    

    createFood(snake,&foodPosX,&foodPosY);

}

void snakeFunctionality2(char snake[HEIGHT][WIDTH], Player *player, char up, char down, char left, char right) {
    gameLogic(snake,player,up,down,left,right);
    drawBoard(snake, player);
}

void drawBoard2(char board[HEIGHT][WIDTH], Player *player1, Player *player2) {
    uint64_t currentColor;
    int i, j;
    
    // Recorre las filas y columnas del tablero
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Si la celda está vacía, establece el color de fondo en blanco
            if (board[i][j] == '0') {
                currentColor = WHITE;
            }
            // Si la celda contiene al jugador, usa su color
            else if (i == player1-> currentY && j == player1-> currentX) {
                currentColor = player1->playerColor;
            }else if(i == player2 -> currentY && j == player2 -> currentX){
                currentColor = player2 ->playerColor;
            }
            
            // Si la celda contiene comida, usa un color específico
            else if (board[i][j] == '*') {
              currentColor = RED;
            }
            // Dibuja un rectángulo en la posición actual con el color correspondiente
            //drawBox(j*PIXELWIDTH , i * PIXELHEIGHT,(j*PIXELWIDTH)+PIXELWIDTH , (i*PIXELHEIGHT)+PIXELHEIGHT, currentColor);       
               }
    }

}

////////// 1 player startup //////////
void snake(){
    sys_clean_screen();
    sys_change_cursor();
    singlePlayerSnake();
}


void singlePlayerSnake(){
    char snake[HEIGHT][WIDTH];
    Player player;
    sys_draw_board(0,0,0xFFFFFF); //Dibujo el tablero de blanco
    initializeGame(snake,&player);

    finish = 0;

    while(!finish){
        playerInput(&player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        snakeMovement(snake,&player);
        snakeFunctionality(snake, &player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        ////// sys_wait(100);// verificar si se mueve muy rapido la snake, en ese caso agregar un wait(100)
    }
    //Revisar 
   //snakeDeadSound();
   sys_clean_screen();
   print("GAME OVER,Press \"e\" to exit");
   while(getChar() != 'e');
   sys_clean_screen(); //Revisar por si hay que llamar a terminal*/
}

////////// 2 players startup //////////

void snake2(){
    sys_clean_screen();
    mpSnake();
}

void mpSnake(){
    char snake[HEIGHT][WIDTH];

    Player player1, player2;

    initializeGameMP(snake, &player1, &player2);

    finish = 0;

    while(!finish){
        /*
        playerInput(&player1,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        snakeMovement(snake,&player1);
        snakeFunctionality2(snake, &player1,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
       */
        playerInput(&player2,PLAYER2_UP,PLAYER2_DOWN,PLAYER2_LEFT,PLAYER2_RIGHT);
        snakeMovement(snake,&player2);
        snakeFunctionality2(snake, &player2,PLAYER2_UP,PLAYER2_DOWN,PLAYER2_LEFT,PLAYER2_RIGHT);
        
        drawBoard2(snake, &player1, &player2);
        ///////////////////sys_wait(100);
        // verificar si se mueve muy rapido la snake, en ese caso agregar un wait()    
    }
   // snakeDeadSound();
    sys_clean_screen();
   print("GAME OVER,Press \"e\" to exit");
   while(getChar() != 'e');
   sys_clean_screen();
}