#include <stdio.h>
#include "board.h"


// função que inicializa o tabuleiro
void initBoard(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = 0;
        }
    }
}

// função que calcula o endereço
int adress(int colum, int line){
    int adress = (line * 80) + colum;
    return adress;
};

// função que faz um bloco a cada elemento da matriz
void draw_square(int colum, int line, int BGR){
    int adress1 = adress(colum,line);
    int adress2 = adress(colum + 1, line);
    int adress3 = adress(colum, line + 1);
    int adress4 = adress(colum + 1, line + 1);
    
    background_block(adress1,BGR);
    background_block(adress2,BGR);
    background_block(adress3,BGR);
    background_block(adress4,BGR);

}
// função que desenha o tabuleiro na tela vga
void displayBoard(int board[ROWS][COLS]) {
    int cellSize = 8;
    int borderSize = 1; // Tamanho da borda

    int boardWidth = COLS * cellSize;
    int boardHeight = ROWS * cellSize;

    int marginX = (75 - boardWidth) / 2;
    int marginY = (95 - boardHeight) / 2;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            short cellColor = (board[i][j] == 0) ? 511 : (BLUE );

            int x1 = j  + marginX + borderSize;
            int y1 = i  + marginY + borderSize;
            
            draw_square(x1*2,y1*2,cellColor);
            
        }
    }
}

// função que limpa as linhas
void clearLines(int board[ROWS][COLS], int* pontuacao) { 
    for (int i = 0; i < ROWS; i++) {
        int fullLine = 1;
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 0) {
                fullLine = 0;
                break;
            }
        }

        if (fullLine) {
            *pontuacao +=  10;
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < COLS; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }

            for (int j = 0; j < COLS; j++) {
                board[0][j] = 0;
            }
            
            i--;
        }
    }
}

// função que verifica se pode colocar a peça
int canPlacePiece (int board[ROWS][COLS], Piece *piece, int x, int y) {
    for (int i = 0; i < PIECES_SIZE; i++) {
        for (int j = 0; j < PIECES_SIZE; j++) {
            if (piece->shape[i][j] == 1) {
                int boardX = x + i;
                int boardY = y + j;
                if (boardX < 0 || boardX >= ROWS || boardY < 0 || boardY >= COLS || board[boardX][boardY] != 0) {
                    return 0; // não consegue colocar a peça
                }
            }
        }
    }

    return 1; // poe colocar a peça
}

// função que coloca a peça na matriz
void placePiece(int board[ROWS][COLS], Piece *piece, int x, int y) {
    for (int i = 0; i < PIECES_SIZE; i++) {
        for (int j = 0; j < PIECES_SIZE; j++){
            if (piece->shape[i][j] == 1) {
                board[x + i][y + j] = 1; // insere na matriz
            }
        }
    }
}

// função que renderiza a peça
void renderWithPiece(int board[ROWS][COLS], Piece *piece, int x, int y) {
    //  Cria um tabuleiro temporário
    int tempBoard[ROWS][COLS];
    
    // Copia o tabuleiro original para o temporário
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }
    
    // Adiciona a peça temporária no tabuleiro temporário
    for (int i = 0; i < PIECES_SIZE; i++) {
        for (int j = 0; j < PIECES_SIZE; j++) {
            if (piece->shape[i][j] == 1) {
                int drawX = y + j; // Coluna
                int drawY = x + i; // Linha
                
                // Verifica se a posição está dentro dos limites do tabuleiro
                if (drawX >= 0 && drawX < COLS && drawY >= 0 && drawY < ROWS) {
                    tempBoard[drawY][drawX] = 2; // Para identificar que é a peça
                }
                
            }
        }
    }
    // Exibe o tabuleiro com a peça temporária
    displayBoard(tempBoard);
}


// função para mover a peça para a esquerda 
int moveLeft(int board[ROWS][COLS], Piece *piece, int x, int *y) {
    if (canPlacePiece(board, piece, x, *y - 1)) {
        *y -= 1;
        return 1; // movimento bem-sucedido
    }

    return 0; // não pode mover para a direita 
}

// função para mover para a direita
int moveRight(int board[ROWS][COLS], Piece *piece, int x, int *y) {
    if (canPlacePiece(board, piece, x, *y + 1)) {
        *y += 1;
        return 1; // movimento bem-sucedido 
    }

    return 0; // não pode mover para a direita 
}

// função que checa se o jogador perdeu
int gameOver(int board[ROWS][COLS], Piece *Piece){
    return !canPlacePiece(board, Piece, 0, 4);
}

// função para limpar a tela
void clear_screen(){
    int i;
    for(i = 0; i < 4800; i++){
        background_block(i,510);
    }
}
// transforma os números decimais em valores equivalentes ao binario para o display 7-segmentos
int number_trans(int num){
    switch (num)
    {
    case 0:
        return 192;
    case 1:
        return 249;
    case 2:
        return 420;
    case 3:
        return 176;
    case 4:
        return 153;
    case 5:
        return 146;
    case 6:
        return 130;
    case 7:
        return 248;
    case 8:
        return 128;    
    case 9:
        return 144;    
    default:
        break;
    }

}
// função que calcula o score e passa para display 7-seg
void score(int pontuacao){
    int r0,r1,r2,r3,r4,r5;
    r0=r1=r2=r3=r4=r5=0; 
    int lista_digitos[6];  // Array para armazenar os dígitos
    int i;

     // Se o número tiver menos de 6 dígitos, ele será preenchido com zeros à esquerda
    if (pontuacao < 1000000) {
        pontuacao = 1000000 + pontuacao;  // Ajuste para preencher com zeros à esquerda
    } 

    // Preenche o array 'digitos' com os dígitos do número
    for (int i = 5; i >= 0; i--) {
        lista_digitos[i] = pontuacao % 10; // Obtém o dígito menos significativo
        pontuacao /= 10;
    }
    
    for (i = 0; i< 6; i++){
        switch (i)
        {
        case 0:
            r5 = number_trans(lista_digitos[i]);
            break;
        case 1:
            r4 = number_trans(lista_digitos[i]);
            break;
        case 2:
            r3 = number_trans(lista_digitos[i]);
            break;
        case 3:
            r2 = number_trans(lista_digitos[i]);
            break;
        case 4:
            r1 = number_trans(lista_digitos[i]);
            break;
        case 5:
            r0 = number_trans(lista_digitos[i]);
            break;
        }
    }

    display_7seg(r0,r1,r2,r3,r4,r5);
}

