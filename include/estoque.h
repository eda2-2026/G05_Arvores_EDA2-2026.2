#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "avl.h"

typedef struct {
    NoAVL *por_validade;
    NoAVL *por_codigo;
    EstatisticasAVL stats_validade;
    EstatisticasAVL stats_codigo;
} Estoque;

void estoque_inicializar(Estoque *estoque);
int estoque_inserir(Estoque *estoque, Lote *lote);
Lote *estoque_buscar_codigo(Estoque *estoque, const char *codigo);
int estoque_remover_codigo(Estoque *estoque, const char *codigo);
int estoque_alterar_validade(Estoque *estoque, const char *codigo, int nova_validade);
int estoque_atualizar_quantidade(Estoque *estoque, const char *codigo, int nova_quantidade);
int estoque_movimentar(Estoque *estoque, const char *codigo, int variacao);
void estoque_destruir(Estoque *estoque);

#endif
