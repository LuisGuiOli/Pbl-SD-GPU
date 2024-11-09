#ifndef PICES_H
#define PICES_H


#define PIECES_SIZE 4
#include <time.h>
#include <stdlib.h>

typedef struct {
    int shape[PIECES_SIZE][PIECES_SIZE];
    short color;
} Piece;

Piece getPiece (char type);

Piece getRandomPiece();

#endif
