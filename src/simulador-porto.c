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
    unsigned int id;
    Lista * pilhas[4];
} Navio;

// filas de atracadouros
Lista * fila_atracadouro[4];

// ultimo id utilizado na criação de navios
unsigned int last_id;

// alocação/liberação de memória
void alocar_navios(int qtd);
void liberar_navio();
void alocar_filas();
void liberar_filas();
void alocar_pilhas(Navio * nav);
void liberar_pilhas(Navio * nav);

int main(int args, char** argv) {
    // iniciando semente do gerador de numeros
    srand(time(NULL));    
    alocar_filas();   
    last_id = 0; 
    int tempo = 0;
    do {

        tempo++;
    } while(tempo < MAX_TEMPO);
    liberar_filas();
}

void alocar_filas() {
    // iniciando filas de atracadouros
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
}

void liberar_pilhas(Navio * nav) {
    for (int i = 0; i < 4; i++)
        lista_destruir(nav->pilhas[i]);
}