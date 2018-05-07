/*
    Instituto Federal de Minas Gerais
    Campus Formiga - Ciência da Computação
    Trabalho de Estrutura de Dados
    Jonathan Arantes

    Arquivo: simulador-porto.c
    Descrição: Arquivo principal do simulador de porto
*/

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// setando quantidade máxima de unidades de tempo, para testes
#define MAX_TEMPO 100

typedef struct {
    unsigned long int id;
    Lista * pilhas[4];
} Navio;

typedef struct {
    int id;
} Container;

// filas de atracadouros
Lista * fila_atracadouro[4];

// pilhas de travessas
Lista * pilha_travessa[5];

// ultimo id utilizado na criação de navios
unsigned long int last_id;

// alocação/liberação de memória
Container * alocar_container();
void liberar_container(Container * c);
Navio * alocar_navio();
void liberar_navio(Navio * nav);
void alocar_filas();
void liberar_filas();
void alocar_pilhas(Navio * nav);
void liberar_pilhas(Navio * nav);
void alocar_travessas();
void liberar_travessas();

// entrada/saida das filas
void entrar_fila(int n);
    
// entrada/saida das pilhas
void criar_containers(Navio * nav, int n);

int main(int args, char** argv) {
    // iniciando semente do gerador de numeros
    srandom(time(NULL));

    alocar_filas();
    alocar_travessas();

    last_id = 0; 
    unsigned long int tempo = 0;
    do {
        // coloca um numero aleatorio (0-3) de navios na fila
        entrar_fila(random() % 3);

        tempo++;
    } while(tempo < MAX_TEMPO);

    liberar_filas();
    liberar_travessas();
}

/* 
    [INICIO] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

Container * alocar_container() {
    Container * c
    if(!(c =(Container*)malloc(sizeof(Container)))) {
        c->id = random();
        return c;
    }
    return NULL;
}

void liberar_container(Container * c) {
    free(c);
}

Navio * alocar_navio() {
    Navio * navio;
    if(!(navio =(Navio*)malloc(sizeof(Navio)))) {
        navio->id = ++last_id;
        alocar_pilhas(navio);
        return navio;
    }
    return NULL;
}

void liberar_navio(Navio * nav) {
    liberar_pilhas(nav);
    free(nav);
}

void alocar_filas() {
    for (int i = 0; i < 4; i++) 
        fila_atracadouro[i] = lista_nova();
}

void liberar_filas() {
    for (int i = 0; i < 4; i++)
        lista_destruir(fila_atracadouro[i]);
}

void alocar_pilhas(Navio * nav) {
    for (int i = 0; i < 4; i++)
        nav->pilhas[i] = lista_nova();
    // gerando containers
    int mod = (random() % 12) + 4;
    for (int i = 0; i < mod; i++)
        lista_ipush(nav->pilha[i%4], );
}

void liberar_pilhas(Navio * nav) {
    for (int i = 0; i < 4; i++)
        lista_destruir(nav->pilhas[i]);
}

void alocar_travessas() {
    for (int i = 0; i < 5; i++) {
        pilha_travessa[i] = lista_nova();
    }
}

void liberar_travessas() {
    for (int i = 0; i < 5; i++) {
        lista_destruir(pilha_travessa[i]);
    }
}

/*
    [FIM] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

/*
    ENTRADA E SAÍDA DAS FILAS
*/

void entrar_fila(int n) {
    int mod = 1;
    for(int i = 0; i < 4; i++) 
        mod += lista_tamanho(fila_atracadouro[i]); 
    mod = mod % 4;
    for(int i = 0; i < n; i++) 
        lista_fpush(fila_atracadouro[mod++], alocar_container());
}