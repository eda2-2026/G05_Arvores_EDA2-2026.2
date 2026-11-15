#include <stdio.h>
#include <time.h>
#include "medicamento.h"

int data_valida(int data) {
    int ano = data / 10000;
    int mes = (data / 100) % 100;
    int dia = data % 100;
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1) return 0;

    int dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int bissexto = (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
    if (bissexto) dias_mes[1] = 29;
    return dia <= dias_mes[mes - 1];
}

int data_atual(void) {
    time_t agora = time(NULL);
    struct tm *local = localtime(&agora);
    if (!local) return 20260101;
    return (local->tm_year + 1900) * 10000 +
           (local->tm_mon + 1) * 100 + local->tm_mday;
}

void formatar_data(int data, char *destino, int tamanho) {
    snprintf(destino, (size_t)tamanho, "%02d/%02d/%04d",
             data % 100, (data / 100) % 100, data / 10000);
}

void imprimir_lote(const Lote *lote) {
    char fabricacao[16], validade[16];
    if (!lote) return;
    formatar_data(lote->data_fabricacao, fabricacao, sizeof(fabricacao));
    formatar_data(lote->data_validade, validade, sizeof(validade));
    printf("\nCodigo: %s\nMedicamento: %s\nPrincipio ativo: %s\n"
           "Categoria: %s\nFabricante: %s\nFabricacao: %s\n"
           "Validade: %s\nQuantidade: %d\nPreco unitario: R$ %.2f\n",
           lote->codigo, lote->nome, lote->principio_ativo,
           lote->categoria, lote->fabricante, fabricacao, validade,
           lote->quantidade, lote->preco_unitario);
}
