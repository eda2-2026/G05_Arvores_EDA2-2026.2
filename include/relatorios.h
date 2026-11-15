#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "estoque.h"

void relatorio_vencimentos(Estoque *estoque, int dias);
void relatorio_vencidos(Estoque *estoque);
void relatorio_estoque_baixo(NoAVL *raiz_codigo, int limite);
void relatorio_estatisticas(Estoque *estoque);

#endif
