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
#define MAX_TEMPO 10

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
unsigned long int last_id = 0;

// modulo pra organização das pilhas de travessas
int mod_travessa = 0;

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
int qtd_containers(Navio * nav);
void inserir_container(Navio * nav, Container * c);
Container * remover_container(Navio * nav);

// controle das filas
void entrar_fila(int n);
void incrementar_tempo();
void movendo_gruas();
    
// entrada/saida das pilhas
int inserir_travessa(Container * c);
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
    last_id = 0;
    unsigned long int tempo = 0;
    while(tempo < MAX_TEMPO) {
        // coloca um numero aleatorio (0-3) de navios na fila
        entrar_fila(rand() % 3);
        // ultima travessa inserida
        printf("mover gruas\n");
        movendo_gruas();
        printf("esvaziar travessas\n");
        esvaziar_travessas();
        printf("media tempo\n");
        media_tempo();
        printf("incrementar tempo\n");
        incrementar_tempo();
        tempo++;
    }
    // desalocando memória
    liberar_filas();
    liberar_travessas();
}

/* 
    [INICIO] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

Container * alocar_container() {
    Container * c = NULL;
    if(!(c =(Container*)malloc(sizeof(Container))))
        return NULL;
    else
        c->id = rand(); 
    return c;
}

void liberar_container(Container * c) {
    free(c);
}

Navio * alocar_navio() {
    Navio * navio = NULL;
    if(!(navio =(Navio*)malloc(sizeof(Navio))))
        return NULL;
    else {
        navio->id = ++last_id;
        navio->tempo = 0;
        alocar_pilhas(navio);
    }
    int qtd = (rand() % 12) + 4;
    for (int i = 0; i < qtd; i++) {
        lista_ipush(navio->pilhas[i%4], alocar_container);
    }
    printf("navio id %d criado, %d containers.\n", (int)navio->id, qtd_containers(navio));
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

int qtd_containers(Navio * nav) {
    int tam = 0;
    for (int i = 0; i < 4; i++)
        tam += lista_tamanho(nav->pilhas[i]);
    return tam;
}

void inserir_container(Navio * nav, Container * c) {
    for (int i = 0; i < 4; i++) {
        if(lista_tamanho(nav->pilhas[i]) < 4) {
            lista_ipush(nav->pilhas[i], c);
            return;
        }
    }
}

Container * remover_container(Navio * nav) {
    if(qtd_containers(nav))
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
    printf("entrando %d navios\n", n);
    int mod = 0;
    for(int i = 0; i < 4; i++) 
        mod += lista_tamanho(fila_atracadouro[i]); 
    mod = mod % 4;
    for(int i = 0; i < n; i++) {
        lista_fpush(fila_atracadouro[mod], alocar_navio());
        mod++;
        mod = mod % 4;
    }
    for (int i = 0; i < 4; i++) {
        Node * n;
        Navio * nav;
        n = lista_inicio(fila_atracadouro[i]);
        printf("Fila %d:\n", i+1);
        while(n != NULL) {
            nav = n->valor;
            printf("%d ", (int)nav->id);
            n = lista_iterador(n, LISTA_FIM);
        }            
        printf("\n");
    }    
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

void movendo_gruas() {
    for (int i = 0; i < 4; i++) {
        Navio * nav = NULL;
        nav = lista_destruir_node(lista_ipop(fila_atracadouro[i]));
        if(nav != NULL) {
            Container * c = remover_container(nav);
            if(c != NULL) {
                int key = inserir_travessa(c);
                if(key == 1) {
                    mov_grua(i, nav->id);
                    if(qtd_containers(nav) == 0) {
                        liberar_navio(nav);
                        continue;
                    }
                    else {
                        lista_ipush(fila_atracadouro[i], nav);
                        continue;
                    }
                }
                else {
                    inserir_container(nav, c);
                    lista_ipush(fila_atracadouro[i], nav);
                    continue;
                }
            }
        }
    }         
}

/*
    [FIM] CONTROLE DAS FILAS
*/

/*
    [INICIO] CONTROLE DAS PILHAS
*/

int inserir_travessa(Container * c) {
    for (int i = 0; i < 5; i++) {
        if(lista_tamanho(pilha_travessa[mod_travessa]) < 5) {
            lista_ipush(pilha_travessa[mod_travessa], c);
            mod_travessa++;
            mod_travessa = mod_travessa % 5;
            return 1;
        }
        mod_travessa++;
        mod_travessa = mod_travessa % 5;
    }
    return 0;
}

void esvaziar_travessas() {
    for (int i = 0; i < 5; i++) {
        if(lista_tamanho(pilha_travessa[i]) >= 5) {
            printf("Liberando travessa %d\n", i);
            while(lista_tamanho(pilha_travessa[i]))
                liberar_container(lista_destruir_node(lista_ipop(pilha_travessa[i])));            
        }
    }
}

/*
    [FIM] CONTROLE DAS PILHAS
*/

void mov_grua(int grua, int navio) {
    printf("Grua %d movendo container do navio %d\n", grua, navio);
}

void media_tempo() {
    float media = 0;
    int qtd = 0;
    Node * n = NULL;
    Navio * nav = NULL;
    for (int i = 0; i < 4; i++) {
        n = fila_atracadouro[i]->inicio;
        while(n != NULL) {
            nav = n->valor;
            media += nav->tempo;
            qtd++;
            n = lista_iterador(n, LISTA_FIM);
        }        
    }
    printf("Tempo medio de espera: %.2f\n", media / (float)qtd);
}