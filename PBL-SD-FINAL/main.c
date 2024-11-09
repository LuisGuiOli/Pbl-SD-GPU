#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> // Para srand e usleep
#include <pthread.h>
#include "i2c.c"
#include "board.c"
#include "pieces.c"
#include "screens.c"

int ACCEL = 1; // ativar e desativar a threads do acelerômetro
int BUTTON = 1; // ativar e desativar a threads do botão
int pausegame = 0; // ativar e desativar o pause do jogo
int endgame = 1; // verificar se o jogador perdeu o jogo para apresentar a tela do game over
int iniciarjogo = 1; // ativar o inicio do jogo
int fd; // variavel para armazenar o arq /dev/mem
I2C_Registers regs;
static int16_t X[1]; // recebe o valor do eixo X do acelerômetro
void inicializacao_accel(); // função que inicializa o acelerômetro
void* accel_working(void *args); // Função que coloca o acelerômetro para funcionar pela thread
void* button_threads(void *args); // função thread para verificar o botão


extern void mem(); // função em assembly que mapeia a memória
extern int push_button(); // função em assembly que 


int main() {

    int board[ROWS][COLS];
    pthread_t thread_accel, thread_button; // cria as variaveis para threads
    
    inicializacao_accel(); // chama a função para iniciar o acelerômetro
    pthread_create(&thread_accel, NULL, accel_working , NULL); // cria a função para que funcione com threads
    pthread_create(&thread_button, NULL, button_threads , NULL); // cria a função para que funcione com threads

    
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Inicializa o tabuleiro e a peça
    initBoard(board);
    Piece piece = getRandomPiece();

    int x = 0, y = 3; // Posição inicial da peça no tabuleiro
    mem(); // mapeia a memória

    clear_screen(); // limpa o blackground 

    // While que exibe a tela de iniciar o jogo até que o botão seja apertado
    while(iniciarjogo){
        desenha_tela(tetris_tela);
    }
    
    int pontuacao = 0; // inicia a pontuação do jogo

    //limpa a tela de inicio para iniciar o jogo
    clear_screen();

    while (endgame) {
        // atualiza a pontuação do display a cada rodada até que o jogador perca
        score(pontuacao);
        
        if (pausegame == 0){
            // Renderiza a peça na posição atual
            renderWithPiece(board, &piece, x, y);
        }
        
        
        //verifica se o jogo acabou
        if (gameOver(board, &piece)) {
            endgame = 0; // Se o jogo acabou, sai do loop
        }

        //verifica se o jogo foi pausado e exibe a tela de pause
        while(pausegame) {
            printf("Jogo pausado\n");
        }

        /*
        Verificação se o valor de X é maior, menor ou igual a 0 para mover para esqueda ou
        para direita.
        */ 
        if (X[0] > 20 && pausegame == 0) {
            moveRight(board, &piece, x, &y);
            
        } else if (X[0] < -20 && pausegame == 0) {
            moveLeft(board, &piece, x, &y);
            
        } 

        // Verifica se a peça pode descer
        if (canPlacePiece(board, &piece, x + 1, y)) {
            x++; // A peça desce se houver espaço
        }else if (pausegame == 0) {
            // Fixar a peça no tabuleiro quando ela não puder descer mais
            placePiece(board, &piece, x, y);
            clearLines(board, &pontuacao);

            // Reinicia com uma nova peça
            piece = getRandomPiece();
            x = 0;
            y = 3;
        }
        usleep(300000); // Controla o intervalo de tempo entre as quedas (50 ms)
    }
    //limpa a tela para exibir a tela de game over caso o jogador tenha perdido
    clear_screen();

    /// Exibe a tela de Game Over
    if (endgame == 0){
        desenha_tela(game_over);
    }
   
    //Encerrando as threads
    BUTTON = 0;
    ACCEL = 0;
    pthread_join(thread_accel, NULL);
    pthread_join(thread_button, NULL);
    
    return 0;
}

// função que inicializa o acelerômetro
void inicializacao_accel(){
    fd = open_fd();
    regs = map_i2c(fd);
    I2C0_Init(&regs); // Estabelece a comunicação com o acelerômetro
    accelerometer_init(regs); // Configura o acelerômetro
}

// função para funcionar o acelerômetro usando threads
void* accel_working(void *args){
    while (ACCEL){    
        if(accelereometer_isDataReady(regs)) {
            accelerometer_x_read(X, regs); // lê os dados do eixo x
        }
        usleep(300000);
    }
    return NULL;
}

// função para verificar se o botão foi pressionado usando threads
void* button_threads(void *args){
    while(BUTTON){
        int teste;
        teste = push_button();
        switch (teste) {
            case 14: // inicia o jogo caso o botão 1 seja pressionado
                iniciarjogo = 0;
                break;
            case 13: // pausa o jogo caso o botão 2 seja pressionado
                if (pausegame == 0 && iniciarjogo == 0){
                    pausegame = 1;
                    clear_screen();
                    desenha_tela(tela_pause);
                    break;
                }
                else{
                    clear_screen();
                    pausegame = 0;
                    break;
                }
            case 11: // encerra o jogo caso o botão 3 seja pressionado
                if(pausegame == 1){
                    clear_screen();
                    exit (0);
                }
                break;
            default:
                break;
        }
        usleep(300000);
    }
    return NULL;
}