#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h"

static Lote *criar(const char *codigo, int validade) {
    Lote *l = calloc(1, sizeof(*l));
    strcpy(l->codigo, codigo);
    strcpy(l->nome, "Medicamento de teste");
    l->data_fabricacao = 20250101;
    l->data_validade = validade;
    l->quantidade = 10;
    l->preco_unitario = 1.0;
    return l;
}

int main(void) {
    Estoque e;
    estoque_inicializar(&e);

    const int datas[] = {20261220, 20261120, 20270101, 20261010, 20261201, 20261101};
    for (int i = 0; i < 6; i++) {
        char codigo[20];
        snprintf(codigo, sizeof(codigo), "TEST%02d", i);
        assert(estoque_inserir(&e, criar(codigo, datas[i])));
        assert(avl_validar(e.por_codigo, comparar_por_codigo));
        assert(avl_validar(e.por_validade, comparar_por_validade));
    }

    assert(avl_contar_nos(e.por_codigo) == 6);
    assert(avl_contar_nos(e.por_validade) == 6);
    assert(estoque_buscar_codigo(&e, "TEST03") != NULL);
    assert(estoque_buscar_codigo(&e, "NAOEXISTE") == NULL);

    assert(estoque_alterar_validade(&e, "TEST03", 20271231));
    assert(avl_validar(e.por_validade, comparar_por_validade));

    assert(estoque_atualizar_quantidade(&e, "TEST02", 25));
    assert(estoque_buscar_codigo(&e, "TEST02")->quantidade == 25);

    assert(estoque_remover_codigo(&e, "TEST01"));
    assert(estoque_buscar_codigo(&e, "TEST01") == NULL);
    assert(avl_contar_nos(e.por_codigo) == 5);
    assert(avl_contar_nos(e.por_validade) == 5);
    assert(avl_validar(e.por_codigo, comparar_por_codigo));
    assert(avl_validar(e.por_validade, comparar_por_validade));

    estoque_destruir(&e);
    puts("Todos os testes AVL passaram.");
    return 0;
}
