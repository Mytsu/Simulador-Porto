/*
    Instituto Federal de Minas Gerais
    Campus Formiga - Ciência da Computação
    Jonathan Arantes

    Arquivo: lista.c
*/

#include "lista.h"

// Implementação do Node

// Cria um novo node, sem ponteiros para anterior e
// proximo, retorna o node com o valor entregue
Node * lista_novo_node(TIPO * valor) {
    Node * node;
    if(!(node =(Node*)malloc(sizeof(Node))))
        return NULL;
    node->anterior = node->proximo = NULL;
    node->valor = valor;
    return node;
}

void lista_destruir_node(Node * node) {
    free(node->valor);
    free(node);
    return;
}

// Implementação do Iterador

Node * lista_iterador(Node * node, Iterador it) {
    switch(it) {
        case LISTA_INICIO:
            return node->anterior;
            break;
        case LISTA_FIM:
            return node->proximo;
            break;
        default:
            break;
    }
}

// Implementação de LDE

// Cria uma nova lista
// retorna null se falhar na alocação da lista
Lista * lista_nova() {
    Lista * lst;
    if(!(lst = (Lista*)malloc(sizeof(Lista))))
        return NULL;
    lst->tam = 0;
    lst->inicio = NULL;
    lst->fim = NULL;
    return lst;
}

// Desaloca uma lista da memória
void lista_destruir(Lista * lst) {
    if(!lst)
        return;
    unsigned int tam = lst->tam;
    Node * aux = lst->inicio->proximo;
    while(tam--) {
        Node * aux2 = aux->proximo;
        free(aux->valor);
        free(aux);
        aux = aux2;
    }
    free(lst);
    return;
}

Node * lista_inicio(Lista * lst) {
    return lst->inicio;
}

// Insere um item no início da lista
// retorna null se falhar na alocação do node
Node * lista_ipush(Lista * lst, TIPO * item) {
    Node * node = lista_novo_node(item);
    node->valor = item;
    if(lst->tam) {
        node->proximo = lst->inicio;
        lst->inicio->anterior = node;
        lst->inicio = node;
    } else {
        lst->inicio = lst->fim = node;
    }
    lst->tam++;
    return node;    
}

// Remove um item do início da lista
// retorna null caso a lista esteja vazia
Node * lista_ipop(Lista * lst) {
    if(!lst->tam)
        return NULL;
    Node * node = lst->inicio;
    if(--lst->tam) {
        (lst->inicio = node->proximo)->anterior = NULL;
    } else {
        lst->inicio = lst->fim = NULL;
    }
    node->anterior = node->proximo = NULL;
    return node;
}

// Insere um item no fim da lista
// retorna NULL se falhar na alocação do node
Node * lista_fpush(Lista * lst, TIPO * item) {
    Node * node = lista_novo_node(item);
    node->valor = item;
    if(lst->tam) {
        node->anterior = lst->fim;
        lst->fim->proximo = node;
        lst->fim = node;
    } else {
        lst->fim = lst->inicio = node;
    }
    lst->tam++;
    return node;   
}

// Remove um item do fim da lista
// retorna null caso a lista esteja vazia
Node * lista_fpop(Lista * lst) {
    if(!lst->tam)
        return NULL;
    Node * node = lst->fim;
    if(--lst->tam) {
        (lst->fim = node->anterior)->proximo = NULL;
    } else {
        lst->inicio = lst->fim = NULL;
    }
    node->anterior = node->proximo = NULL;
    return node;
}

// Busca um item na lista
// retorna null caso seja uma lista vazia ou
// item não encontrado
Node * lista_buscar(Lista * lst, TIPO * item) {
    if(!lst->tam)
        return NULL;
    unsigned int tam = lst->tam;
    Node * aux = lst->inicio;
    while(--tam) {
        if(aux->valor == item)
            return aux;
        else
            aux = lista_iterador(aux, LISTA_FIM);
    }
    return NULL;
}

// Busca um node na lista baseado no index entregue
// retorna null caso seja uma lista vazia ou
// index de valor maior que o tamanho da lista
Node * lista_encontrar(Lista * lst, unsigned int index) {
    if(!lst->tam)
        return NULL;
    Node * aux = lst->inicio;
    if(index < lst->tam) {
        while(index--) aux = lista_iterador(aux, LISTA_FIM);
        return aux;
    }
    return NULL;
}

// Busca um node na lista baseado no index entregue e
// altera seu valor usando o item entregue,
// retorna null caso seja uma lista vazia ou index
// de valor maior que o tamanho da lista
Node * lista_alterar(Lista * lst, unsigned int index,
     TIPO * item) {
    if(!lst->tam)
        return NULL;
    Node * aux = lst->inicio;
    if(index < lst->tam) {
        while(index--) aux = lista_iterador(aux, LISTA_FIM);
        free(aux->valor);
        aux->valor = item;
        return aux;
    }
    return NULL;
}

// Busca um node na lista baseado no index entregue e
// insere um novo node em seu lugar, movendo o node de 
// index antigo para frente, retorna null caso seja uma
// lista vazia ou index de valor maior que a lista
// ou falha ao alocar o novo node
Node * lista_inserir(Lista * lst, unsigned int index,
     TIPO * item) {
    if(!lst->tam)
        return NULL;
    Node * novo = lista_novo_node(item);
    Node * aux = lst->inicio;
    if(index < lst->tam) {
        while(index--) aux = lista_iterador(aux, LISTA_FIM);
        novo->proximo = aux;
        novo->anterior = aux->anterior;
        novo->anterior->proximo = novo;
        aux->anterior = novo;
        lst->tam++;
        return novo;
    }
    return NULL;
}

// Remove um node da lista baseado no ponteiro do node
// entregue, o node removido tem seus valores de anterior
// e proximo nulos mas não é liberado da memoria
// retorna null caso seja uma lista vazia
Node * lista_remover(Lista * lst, Node * node) {
    if(!lst->tam) 
        return NULL;
    unsigned int tam = lst->tam;
    Node * aux = lst->inicio;
    while(--tam) {
        if(!(aux == node))
            aux = lista_iterador(aux, LISTA_FIM);
        else {
            Node * rm = aux;
            (aux = aux->anterior)->proximo = rm->proximo;
            rm->anterior = rm->proximo = NULL;
            lst->tam--;
            return rm;
        }
    }
    return NULL;
}

int lista_tamanho(Lista * lst) {
	return lst->tam;
}