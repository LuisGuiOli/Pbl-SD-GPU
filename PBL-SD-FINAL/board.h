#ifndef BOARD_H
#define BORAD_H

#include "pieces.h"


#define ROWS 24
#define VISIBLE_ROWS 20
#define COLS 10
#define PIXEL_ON 1
#define PIXEL_OFF 0

// cores

#define BLUE 500
#define BLACK 0
#define VERDE 170
#define ROXO 900
#define RED 006
#define YELLOW 255

extern void background_block(int , int );
extern void background_block(int , int );
extern void display_7seg(int cent_milhar,int dez_milhar,int milhar,int centenas,int dezenas,int unidade);

void initBoard(int board[ROWS][COLS]);

void displayBoard(int board[ROWS][COLS]);

void clearLines(int board[ROWS][COLS], int* pontos);

int canPlacePiece(int board[ROWS][COLS], Piece *piece, int x, int y);

void placePiece(int board[ROWS][COLS], Piece *piece, int x, int y);

void renderWithPiece(int board[ROWS][COLS], Piece *piece, int x, int y);

int moveLeft(int board[ROWS][COLS], Piece *piece, int x, int *y);

int moveRight(int board[ROWS][COLS], Piece *piece, int x, int *y);

int gameOver(int board[ROWS][COLS], Piece *Piece);

int number_trans(int num);

void score(int); 

int adress(int colum, int line);

void draw_square(int colum, int line, int BGR);

void clear_screen(); 




#endif