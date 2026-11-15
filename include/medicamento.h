#ifndef MEDICAMENTO_H
#define MEDICAMENTO_H

#define CODIGO_TAM 20
#define NOME_TAM 100
#define ATIVO_TAM 100
#define CATEGORIA_TAM 50
#define FABRICANTE_TAM 80

typedef struct {
    char codigo[CODIGO_TAM];
    char nome[NOME_TAM];
    char principio_ativo[ATIVO_TAM];
    char categoria[CATEGORIA_TAM];
    char fabricante[FABRICANTE_TAM];
    int data_fabricacao; /* AAAAMMDD */
    int data_validade;   /* AAAAMMDD */
    int quantidade;
    double preco_unitario;
} Lote;

int data_valida(int data);
int data_atual(void);
void formatar_data(int data, char *destino, int tamanho);
void imprimir_lote(const Lote *lote);

#endif
