#ifndef AVL_H
#define AVL_H

#include "medicamento.h"

typedef int (*Comparador)(const Lote *, const Lote *);

typedef struct {
    unsigned long long comparacoes;
    unsigned long long rotacoes_simples;
    unsigned long long rotacoes_duplas;
} EstatisticasAVL;

typedef struct NoAVL {
    Lote *lote;
    int altura;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
} NoAVL;

int comparar_por_codigo(const Lote *a, const Lote *b);
int comparar_por_validade(const Lote *a, const Lote *b);

NoAVL *avl_inserir(NoAVL *raiz, Lote *lote, Comparador comparar,
                   int *inserido, EstatisticasAVL *stats);
NoAVL *avl_remover(NoAVL *raiz, const Lote *chave, Comparador comparar,
                   int *removido, EstatisticasAVL *stats);
Lote *avl_buscar(NoAVL *raiz, const Lote *chave, Comparador comparar,
                 EstatisticasAVL *stats);

void avl_percurso_em_ordem(NoAVL *raiz);
void avl_buscar_intervalo(NoAVL *raiz, int inicio, int fim);
int avl_altura(NoAVL *raiz);
int avl_contar_nos(NoAVL *raiz);
int avl_validar(NoAVL *raiz, Comparador comparar);
void avl_destruir_nos(NoAVL *raiz);

#endif
