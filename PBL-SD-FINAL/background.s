.type mem, %function
.global mem
.type background_block, %function
.global background_block
.type dp, %function
.global dp
.type sprite,%function
.global sprite
.type background_color,%function @WBR BACKGROUND
.global background_color
.type push_button, %function 
.global push_button
.type display_7seg, %function 
.global display_7seg

@ Constants
.equ data_b, 0x70
.equ data_a, 0x80
.equ wrreg,  0xc0
.equ address_button, 0x0
.equ wrfull, 0xb0
.equ hex5, 0x10
.equ hex4, 0x20
.equ hex3, 0x30
.equ hex2, 0x40
.equ hex1, 0x50
.equ hex0, 0x60

.section .text 

@ Função que mapeia a memória
mem:
    @Guardando os valores dos registradores na pilha para garantir que os dados se mantenham corretos
    sub sp, sp, #28
    str r9, [sp, #24]
    str r7, [sp, #20]
    str r5, [sp, #16]
    str r4, [sp, #12]
    str r3, [sp, #8]
    str r2, [sp, #4]
    str r1, [sp, #0]


    @Chamada open do sistema para abrir o arquivo /dev/mem
    mov r7, #5 
    ldr r0, =fdopen
    mov r1, #2
    mov r2, #0  
    svc 0
    
    @ Carregando o endereço de LW_Bridge
    ldr r9, =ponte
    ldr r9, [r9]

    @ Chamada MMAP2 do sistema para mapear a memória

    mov r7, #192
    mov r1, #4096
    mov r2, #3
    mov r3, #1
    mov r4, r0
    mov r0, #0
    mov r5, r9
    svc 0

    @ Carregando em r1 o endereço apontado pelo mapped_address

    ldr r1, =mapped_address
    str r0, [r1] @ guarda em r1 o conteudo de r0 (endereço mapeado)

    @ liberando a pilha
    ldr r9, [sp, #24]
    ldr r7, [sp, #20]
    ldr r5, [sp, #16]
    ldr r4, [sp, #12]
    ldr r3, [sp, #8]
    ldr r2, [sp, #4]
    ldr r1, [sp, #0]
    add sp, sp, #28

    bx lr


@ Função responsavel por acender um bloco do background
@
@ Atributos
@ r0 -> coordenadas
@ r1 -> cor do bloco

background_block:
    
    @Guardando os valores dos registradores na pilha para garantir que os dados se mantenham corretos
    
    sub sp, sp, #24
    str r2, [sp, #20]
    str lr, [sp, #16]
    str r7, [sp, #12]
    str r6, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]

    @ guarda em r6 o endereço do mapa 
    ldr r6, =mapped_address
    ldr r6, [r6]

    @Background
    @ Carrega o valor de a cor do bloco no buffer b
    str r1, [r6, #data_b]

    
    mov r7, #2 @ move o valor do opcode para r7
    lsl r0, #4 @ desloca 4 bits para esquerda
    add r0, r0, r7 @ soma o conteudo de r0 (coordenadas) com o opcode
    str r0, [r6, #data_a] @ carrega o conteudo de r0 no buffer a

    bl wreg_full @ verifica se o buffer está cheio 

    mov r1, #1 @ move 1 em r1 para mandar o bit de sinal de start para a modulo gerador de pulso
    str r1, [r6, #wrreg] @ carrega o conteudo de r1 no modulo gerador de pulso para enviar as instruções para gpu
    mov r1, #0  @ move 0 para r1 para mandar o bit de sinal para reiniciar o modulo gerador de pulso
    str r1, [r6, #wrreg] @ carrega o conteudo de r1 na modulo gerador de puso

    @ libera a pilha
    ldr r2, [sp, #20]
    ldr lr, [sp, #16]
    ldr r7, [sp, #12]
    ldr r6, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]
    add sp, sp, #24
    
    bx lr

dp:
    @ Alocando espaço na pilha
    sub sp, sp, #28    
    str r8, [sp, #24]
    str r7, [sp, #20]
    str r5, [sp, #16]
    str r4, [sp, #12]
    str r3, [sp, #8] @ pega o quarto atributo na chamada da função
    str r2, [sp, #4] @ pega o terceiro atributo na chamada da função
    str r1, [sp, #0] @ pega o segundo atributo na chamada da função
    
    ldr r4, [sp, #28]    @ Acessa o 5º argumento na pilha (sp + 28)
    ldr r5, [sp, #32]    @ Acessa o 6º argumento na pilha (sp + 32)

   

    ldr r8, =mapped_address
    ldr r8, [r8]


    @DATA_A
    
    mov r7, #3 @ Opcode
    lsl r5, #4 @ Deslocamento
    add r7, r7, r5 @ Soma

    
        
    @DATA_B
    
    @ Forma - R0
    @ Tamanho - R1
    @ BGR - R2
    @ X - R3
    @ Y - R4
    @ ENDEREÇO - R5 
    
    @deslocamentos
    lsl r0, #31 @ deloca 31 posições para que o valor de r0 (forma) fique como o bit mais significativo como tá escrito no tcc de grabiel
    lsl r2, #22 @ deloca 22 posições para que o valor de r2 (cor)fique como o bit mais significativo como tá escrito no tcc de grabiel
    lsl r1, #18 @ deloca 18 posições para que o valor de r1 (tamanho)fique como o bit mais significativo como tá escrito no tcc de grabiel
    lsl r4, #9 @ deloca 9 posições para que o valor de r4 (valor de coordendada Y)fique como o bit mais significativo como tá escrito no tcc de grabiel

    mov r6, #0 @ zera o registrador

    add r6, r6, r0 @ Soma forma 
    add r6, r6, r2 @ Soma cor
    add r6, r6, r1 @ Soma o tamanho
    add r6, r4, r3 @ Soma o X e Y
    

    str r7, [r8, #data_a]
    str r6, [r8, #data_b]

    bl wreg_full
    
    mov r3, #0
    mov r4, #1
    
    @ manda as instruções para a pilha
    str r4, [r8, #wrreg]
    str r3, [r8, #wrreg]  


    @ libera a pilha

    ldr r8, [sp, #24]
    ldr r7, [sp, #20]
    ldr r5, [sp, #16]
    ldr r4, [sp, #12]
    ldr r3, [sp, #8]
    ldr r2, [sp, #4]
    ldr r1, [sp, #0]
    add sp, sp, #28

    bx lr


sprite:

    @ aloca espaço na pilha para os atributos 
    sub sp, sp, #28
    str r8, [sp, #24]
    str r7, [sp, #20]
    str r5, [sp, #16]
    str r4, [sp, #12]
    str r3, [sp, #8]
    str r2, [sp, #4]
    str r1, [sp, #0]

    @ adiciona o mapemanto
    ldr r8, =mapped_address
    ldr r8, [r8]


    @data_A
    @r6 opcode
    @r7 registrador

    mov r6, #0
    mov r7, #1
    lsl r7, #4
    add r6, r7, r6

    @data_B

    @r0 offset
    @r1 Y
    @r2 X
    @r3 sp

    @ Deslocamentos 

    lsl r3, #29  @ deloca 29 posições para que o valor de r3 (sp) fique como o bit mais significativo como tá escrito no tcc de grabiel
    lsl r2, #19 @ deloca 19 posições para que o valor de r2 (X) fique como o bit mais significativo como tá escrito no tcc de grabiel
    lsl r1, #9 @ deloca 9 posições para que o valor de r1 (Y) fique como o bit mais significativo como tá escrito no tcc de grabiel

    @ Adicionando os valores em r0 para mandar o valor para o buffer
    add r0, r0, r1 
    add r0, r0, r2
    add r0, r0, r3

    mov r4, #0
    mov r5, #1

    str r6, [r8, #data_a]
    str r0, [r8, #data_b]
   
    str r5, [r8, #wrreg]
    str r4, [r8, #wrreg] 


    @ Libera a pilha

    ldr r8, [sp, #24]
    ldr r7, [sp, #20]
    ldr r5, [sp, #16]
    ldr r4, [sp, #12]
    ldr r3, [sp, #8]
    ldr r2, [sp, #4]
    ldr r1, [sp, #0]
    add sp, sp, #28

    bx lr

background_color:

    @ Aloca a pilha
    sub sp, sp, #12
    str lr, [sp, #8]
    str r6, [sp, #4]
    str r0, [sp, #0]

    @ carrega em r1 o ponteiro
    ldr r6, =mapped_address
    ldr r6, [r6]
    
    @ Carrega o valor que vem como atributo no buffer
    str r0, [r6, #data_b]
    mov r0, #0 @ Opcode
    str r0, [r6, #data_a] @ Manda o opcode para o data A

    bl wreg_full
    
    @ Da o sinal para o gerador de pulso
    mov r0, #1
    str r0, [r6, #wrreg]
    mov r0, #0
    str r0, [r6, #wrreg]

    @ Descarrega a pilha
    ldr lr, [sp, #8]
    ldr r6, [sp, #4]
    ldr r0, [sp, #0]
    add sp, sp, #12

    bx lr


wreg_full:
    
    @ Aloca espaço na pilha
    sub sp, sp, #8
    str r0, [sp, #4]
    str r1, [sp, #0]

    @ carrega em r1 o ponteiro
    ldr r0, =mapped_address
    ldr r0, [r0]

    @ Verifica se o buffer está cheio
    loop:
        ldr r1, [r0, #wrfull]
        cmp r1, #1 @ verifica se tem espaço na pilha e compara o retorno se está cheio, se estiver ele se mantem no loop até conseguir enviar a instrução
        beq loop 
    
    @ Libera a pilha
    ldr r0, [sp, #4]
    ldr r1, [sp, #0]
    add sp, sp, #8

    bx lr

push_button:
    @ Aloca espaço na pilha
    sub sp, sp, #8
    str r1, [sp, #4]
    str r0, [sp, #0]

    @ carrega em r1 o ponteiro
    ldr r1, =mapped_address
    ldr r1, [r1]
    
    @ Carrega em r0 o valor no do endereço do botão
    ldr r0, [r1, #address_button]
    
    bx lr @ retorna o valor
    
    @ libera a pilha

    ldr r0, [sp, #0]
    ldr r1, [sp, #4]
    add sp, sp, #8

    bx lr

display_7seg:

    @ Aloca espaço na pilha
    sub sp, sp, #24
    str r7, [sp, #20]
    str r5, [sp, #16]
    str r4, [sp, #12]
    str r3, [sp, #8]
    str r2, [sp, #4]
    str r1, [sp, #0]

    @ Mapeia a memória

    ldr r7, =mapped_address
    ldr r7, [r7]


    ldr r4, [sp, #24]    @ Acessa o 5º argumento na pilha (sp + 24)
    ldr r5, [sp, #28]    @ Acessa o 6º argumento na pilha (sp + 28)


    str r0, [r7, #hex0] @ carrega o valor de r0 no primeiro digito 7 seg
    str r1, [r7, #hex1] @ carrega o valor de r1 no primeiro digito 7 seg
    str r2, [r7, #hex2] @ carrega o valor de r2 no primeiro digito 7 seg
    str r3, [r7, #hex3] @ carrega o valor de r3 no primeiro digito 7 seg 
    str r4, [r7, #hex4] @ carrega o valor de r4 no primeiro digito 7 seg
    str r5, [r7, #hex5] @ carrega o valor de r5 no primeiro digito 7 seg

    @ Libera espaço na pilha
    ldr r7, [sp, #20]
    ldr r5, [sp, #16]
    ldr r4, [sp, #12]
    ldr r3, [sp, #8]
    ldr r2, [sp, #4]
    ldr r1, [sp, #0]
    add sp, sp, #24

    bx lr

.section .data
    fdopen: .asciz "/dev/mem"
    ponte: .word 0xff200
    mapped_address: .space 4

	