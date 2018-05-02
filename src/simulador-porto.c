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

// filas de atracadouros
Lista * fila_atracadouro[4];

void alocar_navios(int qtd);
void alocar_filas();

int main(int args, char** argv) {
    // iniciando semente do gerador de numeros
    srand(time(NULL));
    alocar_filas();    

    int tempo = 0;
    do {

        tempo++;
    } while(tempo < MAX_TEMPO);

}

void alocar_filas() {
    // iniciando filas de atracadouros
    for (int i = 0; i < 4; i++) 
        fila_atracadouro[i] = lista_nova();
}