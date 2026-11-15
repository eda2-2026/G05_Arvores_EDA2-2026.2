#include <stdlib.h>
#include <string.h>
#include "estoque.h"

void estoque_inicializar(Estoque *e) {
    memset(e, 0, sizeof(*e));
}

int estoque_inserir(Estoque *e, Lote *lote) {
    if (!e || !lote || !data_valida(lote->data_fabricacao) ||
        !data_valida(lote->data_validade) ||
        lote->data_validade < lote->data_fabricacao ||
        lote->quantidade < 0 || lote->preco_unitario < 0) return 0;

    if (estoque_buscar_codigo(e, lote->codigo)) return 0;

    int inserido_codigo = 0, inserido_validade = 0;
    e->por_codigo = avl_inserir(e->por_codigo, lote, comparar_por_codigo,
                                &inserido_codigo, &e->stats_codigo);
    if (!inserido_codigo) return 0;

    e->por_validade = avl_inserir(e->por_validade, lote, comparar_por_validade,
                                  &inserido_validade, &e->stats_validade);
    if (!inserido_validade) {
        int removido = 0;
        e->por_codigo = avl_remover(e->por_codigo, lote, comparar_por_codigo,
                                    &removido, &e->stats_codigo);
        return 0;
    }
    return 1;
}

Lote *estoque_buscar_codigo(Estoque *e, const char *codigo) {
    Lote chave = {0};
    if (!e || !codigo) return NULL;
    strncpy(chave.codigo, codigo, sizeof(chave.codigo) - 1);
    return avl_buscar(e->por_codigo, &chave, comparar_por_codigo, &e->stats_codigo);
}

int estoque_remover_codigo(Estoque *e, const char *codigo) {
    Lote *lote = estoque_buscar_codigo(e, codigo);
    if (!lote) return 0;
    int removido_validade = 0, removido_codigo = 0;
    e->por_validade = avl_remover(e->por_validade, lote, comparar_por_validade,
                                  &removido_validade, &e->stats_validade);
    e->por_codigo = avl_remover(e->por_codigo, lote, comparar_por_codigo,
                                &removido_codigo, &e->stats_codigo);
    if (removido_validade && removido_codigo) {
        free(lote);
        return 1;
    }
    return 0;
}

int estoque_alterar_validade(Estoque *e, const char *codigo, int nova_validade) {
    Lote *lote = estoque_buscar_codigo(e, codigo);
    if (!lote || !data_valida(nova_validade) || nova_validade < lote->data_fabricacao)
        return 0;

    int removido = 0, inserido = 0;
    e->por_validade = avl_remover(e->por_validade, lote, comparar_por_validade,
                                  &removido, &e->stats_validade);
    if (!removido) return 0;

    int antiga = lote->data_validade;
    lote->data_validade = nova_validade;
    e->por_validade = avl_inserir(e->por_validade, lote, comparar_por_validade,
                                  &inserido, &e->stats_validade);
    if (!inserido) {
        lote->data_validade = antiga;
        int restaurado = 0;
        e->por_validade = avl_inserir(e->por_validade, lote, comparar_por_validade,
                                      &restaurado, &e->stats_validade);
        return 0;
    }
    return 1;
}

int estoque_atualizar_quantidade(Estoque *e, const char *codigo, int nova_quantidade) {
    Lote *lote = estoque_buscar_codigo(e, codigo);
    if (!lote || nova_quantidade < 0) return 0;
    lote->quantidade = nova_quantidade;
    return 1;
}

int estoque_movimentar(Estoque *e, const char *codigo, int variacao) {
    Lote *lote = estoque_buscar_codigo(e, codigo);
    if (!lote || lote->quantidade + variacao < 0) return 0;
    lote->quantidade += variacao;
    return 1;
}

static void liberar_lotes(NoAVL *n) {
    if (!n) return;
    liberar_lotes(n->esquerda);
    liberar_lotes(n->direita);
    free(n->lote);
}

void estoque_destruir(Estoque *e) {
    if (!e) return;
    /* Os nós não são donos dos Lote; os registros são liberados uma única vez. */
    liberar_lotes(e->por_codigo);
    avl_destruir_nos(e->por_codigo);
    avl_destruir_nos(e->por_validade);
    estoque_inicializar(e);
}
