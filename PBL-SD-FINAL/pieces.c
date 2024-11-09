#include "pieces.h"


int piece_I[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int piece_square[4][4] = {
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int piece_T[4][4] = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int piece_Z[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0}
};

int piece_J[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0}
};

int piece_L[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0}
};

int piece_S[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 0}
};

Piece getPiece (char type) {
    Piece piece;
    switch (type) {
       case 'I':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_I[i][j];
                }
            }
            break;
        
        case 'O':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_square[i][j];
                }
            }
            break;

        case 'T':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_T[i][j];
                }
            }
            break;
        
        case 'Z':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_Z[i][j];
                }
            }
            break;
        
        case 'J':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_J[i][j];
                }
            }
            break;
        
        case 'L':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_L[i][j];
                }
            }
            break;

        case 'S':
            for (int i = 0; i < PIECES_SIZE; i++) {
                for (int j = 0; j < PIECES_SIZE; j++) {
                    piece.shape[i][j] = piece_S[i][j];
                }
            }
            break;
    }
    return piece;
}

Piece getRandomPiece() {
    int randomIndex = rand() % 7;  // Gera um número entre 0 e 6
    char pieceTypes[7] = {'O','I', 'T', 'S', 'Z', 'J', 'L'};
    
    return getPiece(pieceTypes[randomIndex]);
}
