#include <stdio.h>
#include <time.h>
#include "relatorios.h"

static int adicionar_dias(int data, int dias) {
    struct tm tm_data = {0};
    tm_data.tm_year = data / 10000 - 1900;
    tm_data.tm_mon = (data / 100) % 100 - 1;
    tm_data.tm_mday = data % 100 + dias;
    tm_data.tm_isdst = -1;
    mktime(&tm_data);
    return (tm_data.tm_year + 1900) * 10000 +
           (tm_data.tm_mon + 1) * 100 + tm_data.tm_mday;
}

void relatorio_vencimentos(Estoque *e, int dias) {
    int hoje = data_atual();
    int fim = adicionar_dias(hoje, dias);
    printf("\nLotes com validade entre hoje e os proximos %d dias:\n", dias);
    avl_buscar_intervalo(e->por_validade, hoje, fim);
}

void relatorio_vencidos(Estoque *e) {
    int hoje = data_atual();
    printf("\nLotes vencidos antes de hoje:\n");
    if (hoje > 19000101) avl_buscar_intervalo(e->por_validade, 19000101, hoje - 1);
}

static void estoque_baixo_rec(NoAVL *n, int limite) {
    if (!n) return;
    estoque_baixo_rec(n->esquerda, limite);
    if (n->lote->quantidade <= limite) imprimir_lote(n->lote);
    estoque_baixo_rec(n->direita, limite);
}

void relatorio_estoque_baixo(NoAVL *raiz_codigo, int limite) {
    printf("\nLotes com quantidade igual ou inferior a %d:\n", limite);
    estoque_baixo_rec(raiz_codigo, limite);
}

void relatorio_estatisticas(Estoque *e) {
    printf("\n--- ESTATISTICAS AVL ---\n");
    printf("Nos na AVL por validade: %d\n", avl_contar_nos(e->por_validade));
    printf("Altura AVL por validade: %d\n", avl_altura(e->por_validade));
    printf("Comparacoes AVL validade: %llu\n", e->stats_validade.comparacoes);
    printf("Rotacoes simples validade: %llu\n", e->stats_validade.rotacoes_simples);
    printf("Rotacoes duplas validade: %llu\n", e->stats_validade.rotacoes_duplas);

    printf("\nNos na AVL por codigo: %d\n", avl_contar_nos(e->por_codigo));
    printf("Altura AVL por codigo: %d\n", avl_altura(e->por_codigo));
    printf("Comparacoes AVL codigo: %llu\n", e->stats_codigo.comparacoes);
    printf("Rotacoes simples codigo: %llu\n", e->stats_codigo.rotacoes_simples);
    printf("Rotacoes duplas codigo: %llu\n", e->stats_codigo.rotacoes_duplas);

    printf("\nIntegridade AVL validade: %s\n",
           avl_validar(e->por_validade, comparar_por_validade) ? "OK" : "FALHA");
    printf("Integridade AVL codigo: %s\n",
           avl_validar(e->por_codigo, comparar_por_codigo) ? "OK" : "FALHA");
}
