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
unsigned long int last_id = 0;
int id_containers = 0;

// modulo pra organização das pilhas de travessas
int mod_travessa = 0;

// contador de vezes em que as travessas foram esvaziadas
int cont_travessa = 0;

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
        //printf("mover gruas\n");
        movendo_gruas();
        //printf("esvaziar travessas\n");
        esvaziar_travessas();
        //printf("incrementar tempo\n");
        incrementar_tempo();
        //printf("media tempo\n");
        media_tempo();
        tempo++;
    }
    printf("\nQuantidade de containers transportados para as travessas: %d", cont_travessa);
    // desalocando memória
    liberar_filas();
    liberar_travessas();
}

/* 
    [INICIO] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

Container * alocar_container() {
    //printf("1. Alocando container\n");
    Container * c = NULL;
    if(!(c =(Container*)malloc(sizeof(Container))))
        return NULL;
    c->id = id_containers++; 
    //printf("Container id: %d\n", c->id);
    return c;
}

void liberar_container(Container * c) {
    if(c == NULL) {
        return;
    }
    //printf("2. Liberando container: ");
    //printf("%d\n", c->id);    
    free(c);
    return;
}

Navio * alocar_navio() {
    //printf("3. Alocando navio\n");
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
        inserir_container(navio, alocar_container());
    }
    printf("Navio %d alocado na fila, transportando %d containers.\n", (int)navio->id, qtd_containers(navio));
    return navio;
}

void liberar_navio(Navio * nav) {
    //printf("4. Liberando navio\n");
    liberar_pilhas(nav);
    free(nav);
}

void alocar_filas() {
    //printf("5. Alocando filas\n");
    for (int i = 0; i < 4; i++) 
        fila_atracadouro[i] = lista_nova();
}

void liberar_filas() {
    //printf("6. Liberando filas\n");
    for (int i = 0; i < 4; i++) {
        while(lista_tamanho(fila_atracadouro[i]) != 0) {
            liberar_navio(lista_destruir_node(lista_ipop(fila_atracadouro[i])));
        }
        //printf("Liberando... \n");
        free(fila_atracadouro[i]);
    }        
}

void alocar_pilhas(Navio * nav) {
    //printf("7. Alocando pilhas\n");
    for (int i = 0; i < 4; i++)
        nav->pilhas[i] = lista_nova();
    // gerando containers
    int mod = (rand() % 12) + 4;
    for (int i = 0; i < mod; i++)
        lista_ipush(nav->pilhas[i%4], alocar_container());
}

void liberar_pilhas(Navio * nav) {
    //printf("8. Liberando pilhas\n");
    for (int i = 0; i < 4; i++) {                
        int tam = lista_tamanho(nav->pilhas[i]);
        if(tam == 0)
            continue;
        for(int j = 0; j < tam; j++) {
            liberar_container(lista_destruir_node(lista_ipop(nav->pilhas[i])));
        }        
        free(nav->pilhas[i]);
    } 
}

void alocar_travessas() {
    //printf("9. Alocando travessas\n");
    for (int i = 0; i < 5; i++) {
        pilha_travessa[i] = lista_nova();
    }
}

void liberar_travessas() {
    //printf("10. Liberando travessas\n");
    for (int i = 0; i < 5; i++) {
        while(lista_tamanho(pilha_travessa[i]) != 0) {
            liberar_container(lista_destruir_node(lista_ipop(pilha_travessa[i])));
        }
        free(pilha_travessa[i]);
    }
}

/*
    [FIM] FUNÇÕES DE CONTROLE DE MEMÓRIA 
*/

/*
    [INICIO] CONTROLE DOS NAVIOS
*/

int qtd_containers(Navio * nav) {
    //printf("11. Qtd containers\n");
    int tam = 0;
    for (int i = 0; i < 4; i++)
        tam += lista_tamanho(nav->pilhas[i]);
    return tam;
}

void inserir_container(Navio * nav, Container * c) {
    //printf("12. Inserindo container\n");
    for (int i = 0; i < 4; i++) {
        if(lista_tamanho(nav->pilhas[i]) < 4) {
            //printf("Container inserido: \n");
            lista_ipush(nav->pilhas[i], c);
            return;
        }
    }
}

Container * remover_container(Navio * nav) {
    //printf("13. Removendo container\n");
    if(nav == NULL)
        return NULL;
    for (int i = 0; i < 4; i++) {
        if(lista_tamanho(nav->pilhas[i]) != 0) {
            Container * c = (Container*)lista_destruir_node(lista_ipop(nav->pilhas[i]));
            //printf("Container removido: %d\n", c->id);
            return c;
        }
    }    
    return NULL;
}

/*
    [FIM] CONTROLE DOS NAVIOS
*/

/*
    [INICIO] CONTROLE DAS FILAS
*/

void entrar_fila(int n) {
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
    //printf("14. Incrementando tempo\n");
    Node * n;
    for (int i = 0; i < 4; i++) {
        if(lista_tamanho(fila_atracadouro[i]) == 0)
            continue;
        if((n = lista_inicio(fila_atracadouro[i])) != NULL) {
            while(n != NULL) {
                Navio * nav;
                (nav = n->valor)->tempo++;
                //printf("Tempo incrementado.");
                n = lista_iterador(n, LISTA_INICIO);
            } //endwhile           
        }     //endif   
    }         //endfor
}

void movendo_gruas() {
    //printf("15. Movendo gruas\n");
    for (int i = 0; i < 4; i++) {
        Navio * nav = NULL;
        if(lista_tamanho(fila_atracadouro[i]) == 0)
            continue;
        nav = lista_destruir_node(lista_ipop(fila_atracadouro[i]));
        if(nav != NULL) {            
            //printf("movendo_gruas navio : %d\n", (int)nav->id);
            Container * c = remover_container(nav);
            if(c != NULL) {
                //printf("movendo_gruas container: %d\n", c->id);
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
    //printf("16. Inserindo na travessa\n");
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
    //printf("16. Esvaziando travessas\n");
    for (int i = 0; i < 5; i++) {
        if(lista_tamanho(pilha_travessa[i]) >= 5) {
            //printf("Liberando travessa %d: tamanho: %d\n", i, lista_tamanho(pilha_travessa[i]));
            /*
            Node * n = lista_inicio(pilha_travessa[i]);
            while (n != NULL) {
                Container * c = n->valor;
                printf("%d ", c->id);
                n = lista_iterador(n, LISTA_FIM);
            }
            printf("\n");
            */
            int tam = lista_tamanho(pilha_travessa[i]);
            for (int j = 0; j < tam; j++) {
                liberar_container(lista_destruir_node(lista_ipop(pilha_travessa[i])));            
            }
            printf("Transportando containers da travessa %d para o patio.\n", i+1);
            cont_travessa++;
        }
    }
}

/*
    [FIM] CONTROLE DAS PILHAS
*/

/* 
    [INICIO] PRINTS
*/

void mov_grua(int grua, int navio) {
    printf("Grua %d movendo container do navio %d\n", grua+1, navio);
}

void media_tempo() {
    //printf("17. Media de tempo\n");
    float media = 0;
    int qtd = 0;
    Node * n = NULL;
    Navio * nav = NULL;    
    for (int i = 0; i < 4; i++) {
        if(lista_tamanho(fila_atracadouro[i]) == 0)
            continue;
        n = fila_atracadouro[i]->inicio;
        while(n != NULL) {
            nav = n->valor;
            media += (float)nav->tempo;
            qtd++;
            n = lista_iterador(n, LISTA_FIM);
        }        
    }
    if(qtd == 0) {
        return;
    }        
    printf("Tempo medio de espera: %.2f\n", media / (float)qtd);
}

/*
    [FIM] PRINTS
*/