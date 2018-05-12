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
    unsigned long int tempo;
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

// controle dos navios
unsigned int qtd_containers(Navio * nav);
Container * remover_container(Navio * nav);

// controle das filas
void entrar_fila(int n);
void incrementar_tempo();
void movendo_gruas(int * mod);
    
// entrada/saida das pilhas
int inserir_travessa(Container * c, int * mod);
void esvaziar_travessas();

// prints
void mov_grua();
void media_tempo();

int main(int args, char** argv) {
    // iniciando semente do gerador de numeros
    srand(time(NULL));
    // alocando memória
    alocar_filas();
    alocar_travessas();
    unsigned long int tempo = 0;    
    int mod_travessa = 0;
    do {
        // coloca um numero aleatorio (0-3) de navios na fila
        entrar_fila(rand() % 3);
        // ultima travessa inserida
        movendo_gruas(&mod_travessa);
        esvaziar_travessas();
        media_tempo();
        incrementar_tempo();
        tempo++;
    } while(tempo < MAX_TEMPO);
    // desalocando memória
    liberar_filas();
    liberar_travessas();
}

/* 
    [INICIO] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

Container * alocar_container() {
    Container * c = NULL;
    if(!(c =(Container*)malloc(sizeof(Container)))) {
        c->id = rand();
    }
    return c;
}

void liberar_container(Container * c) {
    free(c);
}

Navio * alocar_navio() {
    Navio * navio = NULL;
    if(!(navio =(Navio*)malloc(sizeof(Navio)))) {
        navio->id = ++last_id;
        navio->tempo = 0;
        alocar_pilhas(navio);
    }
    //TODO: criar containers
    return navio;
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
    int mod = (rand() % 12) + 4;
    for (int i = 0; i < mod; i++)
        lista_ipush(nav->pilhas[i%4], alocar_container());
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
    [INICIO] CONTROLE DOS NAVIOS
*/

unsigned int qtd_containers(Navio * nav) {
    unsigned int tam = 0;
    for (int i = 0; i < 4; i++)
        tam += lista_tamanho(nav->pilhas[i]);
    return tam;
}

Container * remover_container(Navio * nav) {
    if(!(qtd_containers(nav)))
        return NULL;
    for (int i = 0; i < 4; i++)
        if(lista_tamanho(nav->pilhas[i])) 
            return lista_destruir_node(lista_ipop(nav->pilhas[i]));
    return NULL;
}

/*
    [FIM] CONTROLE DOS NAVIOS
*/

/*
    [INICIO] CONTROLE DAS FILAS
*/

void entrar_fila(int n) {
    int mod = 1;
    for(int i = 0; i < 4; i++) 
        mod += lista_tamanho(fila_atracadouro[i]); 
    mod = mod % 4;
    for(int i = 0; i < n; i++)
        lista_fpush(fila_atracadouro[mod++], alocar_navio());
}

void incrementar_tempo() {
    Node * n;
    for (int i = 0; i < 4; i++) {
        if(!(n = lista_inicio(fila_atracadouro[i]))) {
            while(n != NULL) {
                Navio * nav;
                (nav = n->valor)->tempo++;
                n = lista_iterador(n, LISTA_INICIO);
            } //endwhile           
        }     //endif   
    }         //endfor
}

void movendo_gruas(int * mod) {
    for (int i = 0; i < 4; i++) {
        Navio * nav = NULL;
        if((nav = lista_destruir_node(
            lista_ipop(fila_atracadouro[i]))) != NULL) {
            // movendo containers para travessas
            for (int j = 0; j < 4; j++) {
                Container * c = lista_destruir_node(lista_ipop(nav->pilhas[j]));
                if(lista_tamanho(nav->pilhas[j])) {                                     
                    if(inserir_travessa(c, mod)) {
                        mov_grua(i, nav->id);
                        // container inserido, sai para fora do for
                        break;
                    }  //endif
                }      //endif
                // inserção não ocorreu, então o container volta para o navio
                lista_ipush(c, nav->pilhas[j]);
            }          //endfor
            // se ainda houver containers no navio, ele volta para a fila
            if(qtd_containers(nav))
                lista_ipush(fila_atracadouro[i], nav);
        }              //endif
    }                  //endfor
}

/*
    [FIM] CONTROLE DAS FILAS
*/

/*
    [INICIO] CONTROLE DAS PILHAS
*/

int inserir_travessa(Container * c, int * mod) {
    int m = *mod;
    for (int i = 0; i < 5; i++) {
        if(lista_tamanho(pilha_travessa[m]) < 5) {
            lista_ipush(pilha_travessa[m], c);
            * mod = ++m % 5;
            return 1;
        }
        m++;
        m = m % 5;
    }
    return 0;
}

/*
    [FIM] CONTROLE DAS PILHAS
*/

void mov_grua(int grua, int navio) {
    printf("Grua %d movendo container do navio %d", grua, navio);
}

void media_tempo() {

}