#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "csv.h"
#include "relatorios.h"

#define ARQUIVO_PADRAO "data/medicamentos.csv"

static void limpar_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void ler_texto(const char *rotulo, char *destino, size_t tamanho) {
    printf("%s", rotulo);
    if (fgets(destino, (int)tamanho, stdin)) {
        destino[strcspn(destino, "\r\n")] = '\0';
    }
}

static int ler_data(const char *rotulo) {
    int data;
    printf("%s (AAAAMMDD): ", rotulo);
    if (scanf("%d", &data) != 1) {
        limpar_entrada();
        return 0;
    }
    limpar_entrada();
    return data;
}

static void cadastrar(Estoque *e) {
    Lote *l = calloc(1, sizeof(*l));
    if (!l) {
        printf("Sem memoria disponivel.\n");
        return;
    }
    ler_texto("Codigo do lote: ", l->codigo, sizeof(l->codigo));
    ler_texto("Nome do medicamento: ", l->nome, sizeof(l->nome));
    ler_texto("Principio ativo: ", l->principio_ativo, sizeof(l->principio_ativo));
    ler_texto("Categoria: ", l->categoria, sizeof(l->categoria));
    ler_texto("Fabricante: ", l->fabricante, sizeof(l->fabricante));
    l->data_fabricacao = ler_data("Data de fabricacao");
    l->data_validade = ler_data("Data de validade");
    printf("Quantidade: ");
    if (scanf("%d", &l->quantidade) != 1) {
        limpar_entrada(); free(l); return;
    }
    printf("Preco unitario: ");
    if (scanf("%lf", &l->preco_unitario) != 1) {
        limpar_entrada(); free(l); return;
    }
    limpar_entrada();

    if (estoque_inserir(e, l)) printf("Lote cadastrado com sucesso.\n");
    else {
        printf("Cadastro rejeitado. Verifique codigo duplicado, datas e valores.\n");
        free(l);
    }
}

static void consultar_codigo(Estoque *e) {
    char codigo[CODIGO_TAM];
    ler_texto("Informe o codigo: ", codigo, sizeof(codigo));
    Lote *l = estoque_buscar_codigo(e, codigo);
    if (l) imprimir_lote(l);
    else printf("Lote nao encontrado.\n");
}

static void remover(Estoque *e) {
    char codigo[CODIGO_TAM];
    ler_texto("Codigo do lote a remover: ", codigo, sizeof(codigo));
    printf(estoque_remover_codigo(e, codigo) ?
           "Lote removido.\n" : "Lote nao encontrado.\n");
}

static void alterar_validade(Estoque *e) {
    char codigo[CODIGO_TAM];
    ler_texto("Codigo do lote: ", codigo, sizeof(codigo));
    int nova = ler_data("Nova data de validade");
    printf(estoque_alterar_validade(e, codigo, nova) ?
           "Validade atualizada.\n" : "Nao foi possivel atualizar.\n");
}

static void atualizar_quantidade(Estoque *e) {
    char codigo[CODIGO_TAM];
    int qtd;
    ler_texto("Codigo do lote: ", codigo, sizeof(codigo));
    printf("Nova quantidade: ");
    if (scanf("%d", &qtd) != 1) { limpar_entrada(); return; }
    limpar_entrada();
    printf(estoque_atualizar_quantidade(e, codigo, qtd) ?
           "Quantidade atualizada.\n" : "Operacao invalida.\n");
}

static void movimentar(Estoque *e, int entrada) {
    char codigo[CODIGO_TAM];
    int qtd;
    ler_texto("Codigo do lote: ", codigo, sizeof(codigo));
    printf("Quantidade: ");
    if (scanf("%d", &qtd) != 1 || qtd <= 0) { limpar_entrada(); return; }
    limpar_entrada();
    printf(estoque_movimentar(e, codigo, entrada ? qtd : -qtd) ?
           "Movimentacao registrada.\n" : "Lote inexistente ou quantidade insuficiente.\n");
}

static void consultar_intervalo(Estoque *e) {
    int inicio = ler_data("Data inicial");
    int fim = ler_data("Data final");
    if (!data_valida(inicio) || !data_valida(fim) || inicio > fim) {
        printf("Intervalo de datas invalido.\n");
        return;
    }
    avl_buscar_intervalo(e->por_validade, inicio, fim);
}

int main(int argc, char **argv) {
    Estoque estoque;
    estoque_inicializar(&estoque);

    const char *arquivo = argc > 1 ? argv[1] : ARQUIVO_PADRAO;
    int carregados = csv_carregar(arquivo, &estoque);
    if (carregados < 0) {
        printf("Nao foi possivel abrir %s. Iniciando estoque vazio.\n", arquivo);
    } else {
        printf("%d lotes carregados de %s.\n", carregados, arquivo);
    }

    int opcao;
    do {
        printf("\n========== GESTAO DE MEDICAMENTOS ==========\n"
               "1. Cadastrar lote\n"
               "2. Consultar lote por codigo\n"
               "3. Consultar lotes por intervalo de validade\n"
               "4. Atualizar quantidade\n"
               "5. Alterar data de validade\n"
               "6. Remover lote\n"
               "7. Registrar entrada de estoque\n"
               "8. Registrar saida de estoque\n"
               "9. Listar todos os lotes por validade\n"
               "10. Relatorio de vencidos\n"
               "11. Vencimentos nos proximos 7 dias\n"
               "12. Vencimentos nos proximos 15 dias\n"
               "13. Vencimentos nos proximos 30 dias\n"
               "14. Relatorio de estoque baixo\n"
               "15. Estatisticas e validacao das AVL\n"
               "16. Salvar agora\n"
               "0. Salvar e sair\n"
               "Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_entrada();
            opcao = -1;
            continue;
        }
        limpar_entrada();

        switch (opcao) {
            case 1: cadastrar(&estoque); break;
            case 2: consultar_codigo(&estoque); break;
            case 3: consultar_intervalo(&estoque); break;
            case 4: atualizar_quantidade(&estoque); break;
            case 5: alterar_validade(&estoque); break;
            case 6: remover(&estoque); break;
            case 7: movimentar(&estoque, 1); break;
            case 8: movimentar(&estoque, 0); break;
            case 9: avl_percurso_em_ordem(estoque.por_validade); break;
            case 10: relatorio_vencidos(&estoque); break;
            case 11: relatorio_vencimentos(&estoque, 7); break;
            case 12: relatorio_vencimentos(&estoque, 15); break;
            case 13: relatorio_vencimentos(&estoque, 30); break;
            case 14: {
                int limite;
                printf("Quantidade limite: ");
                if (scanf("%d", &limite) == 1 && limite >= 0)
                    relatorio_estoque_baixo(estoque.por_codigo, limite);
                else printf("Limite invalido.\n");
                limpar_entrada();
                break;
            }
            case 15: relatorio_estatisticas(&estoque); break;
            case 16:
                printf(csv_salvar(arquivo, estoque.por_codigo) ?
                       "Dados salvos.\n" : "Erro ao salvar arquivo.\n");
                break;
            case 0:
                if (!csv_salvar(arquivo, estoque.por_codigo))
                    printf("Aviso: nao foi possivel salvar os dados.\n");
                break;
            default: if (opcao != -1) printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    estoque_destruir(&estoque);
    return 0;
}
