#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

int csv_carregar(const char *caminho, Estoque *estoque) {
    FILE *f = fopen(caminho, "r");
    if (!f) return -1;

    char linha[1024];
    int carregados = 0, linha_num = 0;
    while (fgets(linha, sizeof(linha), f)) {
        linha_num++;
        if (linha_num == 1 && strstr(linha, "codigo") == linha) continue;
        linha[strcspn(linha, "\r\n")] = '\0';

        Lote *l = calloc(1, sizeof(*l));
        if (!l) break;

        char *campo = strtok(linha, ",");
        int idx = 0, ok = 1;
        while (campo) {
            switch (idx) {
                case 0: snprintf(l->codigo, sizeof(l->codigo), "%s", campo); break;
                case 1: snprintf(l->nome, sizeof(l->nome), "%s", campo); break;
                case 2: snprintf(l->principio_ativo, sizeof(l->principio_ativo), "%s", campo); break;
                case 3: snprintf(l->categoria, sizeof(l->categoria), "%s", campo); break;
                case 4: snprintf(l->fabricante, sizeof(l->fabricante), "%s", campo); break;
                case 5: l->data_fabricacao = atoi(campo); break;
                case 6: l->data_validade = atoi(campo); break;
                case 7: l->quantidade = atoi(campo); break;
                case 8: l->preco_unitario = atof(campo); break;
                default: break;
            }
            idx++;
            campo = strtok(NULL, ",");
        }
        if (idx != 9 || !estoque_inserir(estoque, l)) ok = 0;
        if (ok) carregados++;
        else free(l);
    }
    fclose(f);
    return carregados;
}

static void salvar_em_ordem(FILE *f, NoAVL *n) {
    if (!n) return;
    salvar_em_ordem(f, n->esquerda);
    Lote *l = n->lote;
    fprintf(f, "%s,%s,%s,%s,%s,%d,%d,%d,%.2f\n",
            l->codigo, l->nome, l->principio_ativo, l->categoria,
            l->fabricante, l->data_fabricacao, l->data_validade,
            l->quantidade, l->preco_unitario);
    salvar_em_ordem(f, n->direita);
}

int csv_salvar(const char *caminho, NoAVL *raiz_codigo) {
    FILE *f = fopen(caminho, "w");
    if (!f) return 0;
    fprintf(f, "codigo,nome,principio_ativo,categoria,fabricante,data_fabricacao,data_validade,quantidade,preco\n");
    salvar_em_ordem(f, raiz_codigo);
    fclose(f);
    return 1;
}
