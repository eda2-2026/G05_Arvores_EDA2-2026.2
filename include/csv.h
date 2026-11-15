#ifndef CSV_H
#define CSV_H

#include "estoque.h"

int csv_carregar(const char *caminho, Estoque *estoque);
int csv_salvar(const char *caminho, NoAVL *raiz_codigo);

#endif
