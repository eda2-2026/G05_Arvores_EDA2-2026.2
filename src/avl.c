#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

static int maximo(int a, int b) { return a > b ? a : b; }
int avl_altura(NoAVL *n) { return n ? n->altura : 0; }

int comparar_por_codigo(const Lote *a, const Lote *b) {
    return strcmp(a->codigo, b->codigo);
}

int comparar_por_validade(const Lote *a, const Lote *b) {
    if (a->data_validade < b->data_validade) return -1;
    if (a->data_validade > b->data_validade) return 1;
    return strcmp(a->codigo, b->codigo);
}

static int fator_balanceamento(NoAVL *n) {
    return n ? avl_altura(n->esquerda) - avl_altura(n->direita) : 0;
}

static NoAVL *novo_no(Lote *lote) {
    NoAVL *n = malloc(sizeof(*n));
    if (!n) return NULL;
    n->lote = lote;
    n->altura = 1;
    n->esquerda = n->direita = NULL;
    return n;
}

static NoAVL *rotacao_direita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *t2 = x->direita;
    x->direita = y;
    y->esquerda = t2;
    y->altura = 1 + maximo(avl_altura(y->esquerda), avl_altura(y->direita));
    x->altura = 1 + maximo(avl_altura(x->esquerda), avl_altura(x->direita));
    return x;
}

static NoAVL *rotacao_esquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *t2 = y->esquerda;
    y->esquerda = x;
    x->direita = t2;
    x->altura = 1 + maximo(avl_altura(x->esquerda), avl_altura(x->direita));
    y->altura = 1 + maximo(avl_altura(y->esquerda), avl_altura(y->direita));
    return y;
}

NoAVL *avl_inserir(NoAVL *raiz, Lote *lote, Comparador comparar,
                   int *inserido, EstatisticasAVL *stats) {
    if (!raiz) {
        NoAVL *n = novo_no(lote);
        if (n) *inserido = 1;
        return n;
    }

    if (stats) stats->comparacoes++;
    int cmp = comparar(lote, raiz->lote);
    if (cmp < 0) raiz->esquerda = avl_inserir(raiz->esquerda, lote, comparar, inserido, stats);
    else if (cmp > 0) raiz->direita = avl_inserir(raiz->direita, lote, comparar, inserido, stats);
    else return raiz;

    if (!*inserido) return raiz;

    raiz->altura = 1 + maximo(avl_altura(raiz->esquerda), avl_altura(raiz->direita));
    int fb = fator_balanceamento(raiz);

    if (fb > 1) {
        if (comparar(lote, raiz->esquerda->lote) < 0) {
            if (stats) stats->rotacoes_simples++;
            return rotacao_direita(raiz); /* LL */
        }
        if (stats) stats->rotacoes_duplas++;
        raiz->esquerda = rotacao_esquerda(raiz->esquerda);
        return rotacao_direita(raiz); /* LR */
    }
    if (fb < -1) {
        if (comparar(lote, raiz->direita->lote) > 0) {
            if (stats) stats->rotacoes_simples++;
            return rotacao_esquerda(raiz); /* RR */
        }
        if (stats) stats->rotacoes_duplas++;
        raiz->direita = rotacao_direita(raiz->direita);
        return rotacao_esquerda(raiz); /* RL */
    }
    return raiz;
}

static NoAVL *menor_no(NoAVL *raiz) {
    NoAVL *atual = raiz;
    while (atual && atual->esquerda) atual = atual->esquerda;
    return atual;
}

NoAVL *avl_remover(NoAVL *raiz, const Lote *chave, Comparador comparar,
                   int *removido, EstatisticasAVL *stats) {
    if (!raiz) return NULL;

    if (stats) stats->comparacoes++;
    int cmp = comparar(chave, raiz->lote);
    if (cmp < 0) raiz->esquerda = avl_remover(raiz->esquerda, chave, comparar, removido, stats);
    else if (cmp > 0) raiz->direita = avl_remover(raiz->direita, chave, comparar, removido, stats);
    else {
        *removido = 1;
        if (!raiz->esquerda || !raiz->direita) {
            NoAVL *filho = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(raiz);
            return filho;
        } else {
            NoAVL *sucessor = menor_no(raiz->direita);
            raiz->lote = sucessor->lote;
            int dummy = 0;
            raiz->direita = avl_remover(raiz->direita, sucessor->lote, comparar, &dummy, stats);
        }
    }

    if (!raiz) return NULL;
    raiz->altura = 1 + maximo(avl_altura(raiz->esquerda), avl_altura(raiz->direita));
    int fb = fator_balanceamento(raiz);

    if (fb > 1 && fator_balanceamento(raiz->esquerda) >= 0) {
        if (stats) stats->rotacoes_simples++;
        return rotacao_direita(raiz);
    }
    if (fb > 1 && fator_balanceamento(raiz->esquerda) < 0) {
        if (stats) stats->rotacoes_duplas++;
        raiz->esquerda = rotacao_esquerda(raiz->esquerda);
        return rotacao_direita(raiz);
    }
    if (fb < -1 && fator_balanceamento(raiz->direita) <= 0) {
        if (stats) stats->rotacoes_simples++;
        return rotacao_esquerda(raiz);
    }
    if (fb < -1 && fator_balanceamento(raiz->direita) > 0) {
        if (stats) stats->rotacoes_duplas++;
        raiz->direita = rotacao_direita(raiz->direita);
        return rotacao_esquerda(raiz);
    }
    return raiz;
}

Lote *avl_buscar(NoAVL *raiz, const Lote *chave, Comparador comparar,
                 EstatisticasAVL *stats) {
    while (raiz) {
        if (stats) stats->comparacoes++;
        int cmp = comparar(chave, raiz->lote);
        if (cmp == 0) return raiz->lote;
        raiz = cmp < 0 ? raiz->esquerda : raiz->direita;
    }
    return NULL;
}

void avl_percurso_em_ordem(NoAVL *raiz) {
    if (!raiz) return;
    avl_percurso_em_ordem(raiz->esquerda);
    imprimir_lote(raiz->lote);
    avl_percurso_em_ordem(raiz->direita);
}

void avl_buscar_intervalo(NoAVL *raiz, int inicio, int fim) {
    if (!raiz) return;
    int validade = raiz->lote->data_validade;
    if (validade >= inicio) avl_buscar_intervalo(raiz->esquerda, inicio, fim);
    if (validade >= inicio && validade <= fim) imprimir_lote(raiz->lote);
    if (validade <= fim) avl_buscar_intervalo(raiz->direita, inicio, fim);
}

int avl_contar_nos(NoAVL *raiz) {
    return raiz ? 1 + avl_contar_nos(raiz->esquerda) + avl_contar_nos(raiz->direita) : 0;
}

int avl_validar(NoAVL *raiz, Comparador comparar) {
    if (!raiz) return 1;
    int fb = fator_balanceamento(raiz);
    if (fb < -1 || fb > 1) return 0;
    if (raiz->esquerda && comparar(raiz->esquerda->lote, raiz->lote) >= 0) return 0;
    if (raiz->direita && comparar(raiz->direita->lote, raiz->lote) <= 0) return 0;
    int altura_esperada = 1 + maximo(avl_altura(raiz->esquerda), avl_altura(raiz->direita));
    if (raiz->altura != altura_esperada) return 0;
    return avl_validar(raiz->esquerda, comparar) && avl_validar(raiz->direita, comparar);
}

void avl_destruir_nos(NoAVL *raiz) {
    if (!raiz) return;
    avl_destruir_nos(raiz->esquerda);
    avl_destruir_nos(raiz->direita);
    free(raiz);
}
