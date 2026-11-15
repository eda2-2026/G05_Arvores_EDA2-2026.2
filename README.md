# Sistema de Controle e Gestão de Validade de Medicamentos

Projeto acadêmico desenvolvido em **linguagem C** para aplicar estruturas de dados do tipo **árvore AVL** em um cenário de gerenciamento de estoque de medicamentos. O sistema mantém registros de lotes, permite consultas por código e por período de validade, registra movimentações de estoque e gera relatórios para apoiar o acompanhamento dos produtos.

> **Finalidade acadêmica:** demonstrar a implementação e o uso de árvores AVL em operações de inserção, busca, remoção e consulta. O projeto utiliza dados fictícios e não deve ser utilizado para controle farmacêutico ou validação sanitária em ambiente real.

## Sumário

- [1. Visão geral](#1-visão-geral)
- [2. Funcionalidades](#2-funcionalidades)
- [3. Estruturas de dados](#3-estruturas-de-dados)
- [4. Organização do projeto](#4-organização-do-projeto)
- [5. Requisitos](#5-requisitos)
- [6. Como executar](#6-como-executar)
- [7. Utilização do sistema](#7-utilização-do-sistema)
- [8. Arquivo de dados CSV](#8-arquivo-de-dados-csv)
- [9. Complexidade das operações](#9-complexidade-das-operações)
- [10. Testes](#10-testes)
- [11. Limitações e observações](#11-limitações-e-observações)

## 1. Visão geral

O sistema representa um estoque de medicamentos no qual cada registro corresponde a um **lote**. Um lote reúne informações de identificação, características do medicamento, datas, quantidade disponível e preço unitário.

O desafio central é permitir diferentes formas de consulta sem precisar percorrer todos os registros em cada operação. Para isso, o estoque é indexado simultaneamente por duas árvores AVL:

- uma árvore organizada pelo **código do lote**, utilizada principalmente nas operações de identificação e manutenção de registros;
- uma árvore organizada pela **data de validade**, utilizada nas consultas por intervalo e nos relatórios de vencimento.

As duas árvores referenciam o mesmo registro `Lote`. Dessa forma, o sistema mantém dois critérios de organização sem precisar manter duas cópias independentes dos dados de cada lote.

## 2. Funcionalidades

O programa é executado em terminal e apresenta um menu interativo com as seguintes operações:

| Opção | Funcionalidade | Descrição |
|---|---|---|
| 1 | Cadastrar lote | Inclui um novo lote no estoque, após as validações realizadas pela camada de estoque. |
| 2 | Consultar por código | Localiza e exibe um lote a partir de seu código. |
| 3 | Consultar por intervalo de validade | Lista os lotes cuja validade está dentro das datas inicial e final informadas. |
| 4 | Atualizar quantidade | Define uma nova quantidade para um lote existente. |
| 5 | Alterar data de validade | Atualiza a validade de um lote e sua posição na árvore indexada por validade. |
| 6 | Remover lote | Remove o lote do estoque. |
| 7 | Registrar entrada | Acrescenta uma quantidade ao estoque de um lote. |
| 8 | Registrar saída | Subtrai uma quantidade, respeitando a disponibilidade informada pelo sistema. |
| 9 | Listar lotes por validade | Percorre a árvore de validade em ordem, exibindo os lotes ordenados por esse critério. |
| 10 | Relatório de vencidos | Lista os lotes com validade anterior à data atual. |
| 11 | Próximos 7 dias | Lista os lotes com validade entre hoje e os próximos sete dias. |
| 12 | Próximos 15 dias | Lista os lotes com validade entre hoje e os próximos quinze dias. |
| 13 | Próximos 30 dias | Lista os lotes com validade entre hoje e os próximos trinta dias. |
| 14 | Relatório de estoque baixo | Lista os lotes cuja quantidade é menor ou igual ao limite informado. |
| 15 | Estatísticas e validação AVL | Exibe informações das duas árvores e verifica sua integridade estrutural. |
| 16 | Salvar agora | Grava os registros atuais no arquivo CSV. |
| 0 | Salvar e sair | Tenta salvar os dados e encerra o programa. |

## 3. Estruturas de dados

### 3.1. Registro de lote

A estrutura `Lote`, declarada em `include/medicamento.h`, representa os dados de cada lote:

| Campo | Tipo | Descrição |
|---|---|---|
| `codigo` | `char[]` | Código identificador do lote. |
| `nome` | `char[]` | Nome do medicamento. |
| `principio_ativo` | `char[]` | Princípio ativo informado para o medicamento. |
| `categoria` | `char[]` | Categoria do medicamento. |
| `fabricante` | `char[]` | Fabricante do medicamento. |
| `data_fabricacao` | `int` | Data de fabricação no formato `AAAAMMDD`. |
| `data_validade` | `int` | Data de validade no formato `AAAAMMDD`. |
| `quantidade` | `int` | Quantidade disponível do lote. |
| `preco_unitario` | `double` | Preço unitário informado. |

### 3.2. Árvore AVL por código

A primeira AVL utiliza o código como chave de ordenação. Ela permite localizar um lote pelo identificador e dá suporte às operações de cadastro, atualização e remoção.

### 3.3. Árvore AVL por validade

A segunda AVL organiza os lotes pela data de validade. Quando dois lotes possuem a mesma data, o código do lote é utilizado como critério de desempate. Essa organização permite realizar consultas por intervalo de datas e percursos em ordem cronológica.

### 3.4. Compartilhamento dos registros

Os nós das duas árvores guardam ponteiros para o mesmo objeto `Lote`. A estrutura `Estoque`, definida em `include/estoque.h`, mantém as raízes das duas árvores e as estatísticas de cada uma:

- `por_validade`: raiz da AVL ordenada por validade;
- `por_codigo`: raiz da AVL ordenada por código;
- `stats_validade`: estatísticas de operações da AVL por validade;
- `stats_codigo`: estatísticas de operações da AVL por código.

Essa organização exige que operações que alteram uma chave de ordenação, como a alteração da validade, atualizem adequadamente o índice correspondente. A lógica de integração entre as árvores fica concentrada no módulo de estoque.

### 3.5. Balanceamento e estatísticas

A implementação AVL armazena a altura em cada nó e utiliza rotações para manter o balanceamento após inserções e remoções. O sistema contabiliza comparações, rotações simples e rotações duplas. Também disponibiliza altura, quantidade de nós e validação da integridade de cada árvore.

## 4. Organização do projeto

```text
G05_Arvores_EDA2-2026.2/
├── data/
│   └── medicamentos.csv
├── include/
│   ├── avl.h
│   ├── csv.h
│   ├── estoque.h
│   ├── medicamento.h
│   └── relatorios.h
├── src/
│   ├── avl.c
│   ├── csv.c
│   ├── estoque.c
│   ├── main.c
│   ├── medicamento.c
│   └── relatorios.c
├── tests/
│   └── test_avl.c
├── gerar_base.py
├── Makefile
└── README.md
```

### Responsabilidade dos módulos

| Arquivo | Responsabilidade |
|---|---|
| `src/main.c` | Inicialização do estoque, carregamento do CSV, menu e interação com o usuário. |
| `src/avl.c` | Operações fundamentais das árvores AVL, percursos, consultas por intervalo, contagem, altura e validação. |
| `src/estoque.c` | Integração das duas árvores e operações sobre os lotes. |
| `src/medicamento.c` | Funções relacionadas à validação e apresentação dos dados de medicamento. |
| `src/csv.c` | Leitura dos registros do CSV e gravação do estoque no arquivo. |
| `src/relatorios.c` | Relatórios de vencimento, estoque baixo e estatísticas das AVL. |
| `include/*.h` | Declarações de estruturas, tipos e funções utilizadas pelos módulos. |
| `tests/test_avl.c` | Testes automatizados das operações centrais de AVL e estoque. |
| `gerar_base.py` | Geração de uma base fictícia para experimentação. |
| `Makefile` | Automação da compilação, execução dos testes e limpeza dos arquivos gerados. |

## 5. Requisitos

Para compilar e executar o projeto, são necessários:

- **GCC**, para compilar os arquivos em C;
- **GNU Make**, recomendado para utilizar os comandos definidos no `Makefile`;
- **Python 3**, somente se desejar gerar novamente a base fictícia com `gerar_base.py`.

O projeto utiliza recursos da linguagem C padrão e configura a compilação com `-std=c11`, além de opções de avisos e otimização.

## 6. Como executar

Os comandos a seguir devem ser executados no terminal, a partir da pasta raiz do projeto.

### 6.1. Obter ou gerar a base de dados

O repositório já inclui `data/medicamentos.csv`. Caso queira gerar novamente a base fictícia, execute:

```bash
python3 gerar_base.py
```

O script gera um arquivo CSV com **10.000 lotes fictícios**. A distribuição das datas de validade é aleatória e tem finalidade de teste.

> A geração da base substitui o conteúdo do arquivo de dados gerado pelo script. Se houver alterações importantes no CSV atual, faça uma cópia antes de executar o gerador.

### 6.2. Compilar com Make

```bash
make
```

O comando gera o executável `sistema` na raiz do projeto.

### 6.3. Executar

```bash
./sistema
```

Por padrão, o programa utiliza `data/medicamentos.csv`, caminho relativo à pasta de execução.

Também é possível informar outro arquivo CSV como argumento:

```bash
./sistema caminho/para/arquivo.csv
```

### 6.4. Compilar manualmente

Se preferir não utilizar o `Makefile`, execute:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude \
  src/main.c src/medicamento.c src/avl.c src/estoque.c \
  src/csv.c src/relatorios.c -o sistema
```

## 7. Utilização do sistema

Ao iniciar, o programa tenta abrir o CSV indicado (ou o arquivo padrão), carregar os lotes e inserir os registros nas estruturas de índice. Em seguida, apresenta o menu no terminal.

Para utilizar uma funcionalidade:

1. Digite o número correspondente à opção desejada.
2. Informe os dados solicitados pelo programa.
3. Confira a mensagem de resultado ou os registros apresentados.
4. Escolha `16` para salvar durante a execução ou `0` para salvar e sair.

### Formato das datas

As datas devem ser informadas como números no formato `AAAAMMDD`, por exemplo:

```text
20261215
```

Isso representa 15 de dezembro de 2026. Nas consultas por intervalo, a data inicial deve ser anterior ou igual à data final.

## 8. Arquivo de dados CSV

O arquivo padrão é `data/medicamentos.csv`. Ele armazena os registros de forma persistente entre execuções: ao iniciar, os dados são carregados para a memória; ao salvar, o conteúdo do estoque é gravado novamente no CSV.

Características do formato utilizado:

- separador de campos: vírgula;
- datas: inteiros no formato `AAAAMMDD`;
- campos textuais: não devem conter vírgulas, pois o formato utilizado não trata vírgulas internas como conteúdo escapado;
- os dados incluídos na base fornecida são fictícios.

É possível passar outro caminho de CSV como argumento na inicialização, desde que o arquivo siga o formato esperado pelo programa.

## 9. Complexidade das operações

Considere `N` como o número de lotes armazenados e `K` como a quantidade de lotes encontrados em uma consulta por intervalo.

| Operação | Complexidade | Justificativa |
|---|---:|---|
| Busca em uma AVL | `O(log N)` | A árvore AVL mantém altura logarítmica. |
| Inserção em uma AVL | `O(log N)` | A busca pela posição e o rebalanceamento percorrem a altura da árvore. |
| Remoção em uma AVL | `O(log N)` | A localização e o rebalanceamento são limitados pela altura. |
| Consulta por intervalo de validade | `O(log N + K)` | A árvore permite alcançar a região relevante e percorrer os resultados encontrados. |
| Percurso completo em ordem | `O(N)` | Cada nó é visitado uma vez. |

As operações do estoque que precisam atualizar os dois índices realizam o trabalho correspondente em cada árvore. A tabela apresenta o custo das operações em uma AVL, não o custo total combinado de toda operação de alto nível.

## 10. Testes

O projeto possui testes automatizados em `tests/test_avl.c`. Para compilar e executar a suíte disponível, utilize:

```bash
make test
```

O `Makefile` compila o executável `test_avl` e o executa em seguida. Os testes verificam operações centrais das estruturas AVL e do gerenciamento de estoque.

Para remover os arquivos de compilação gerados pelo Makefile, execute:

```bash
make clean
```

Esse comando remove os arquivos objeto, o executável principal `sistema` e o executável de testes `test_avl`.

## 11. Limitações e observações

- O projeto é um protótipo acadêmico em C, executado por interface de terminal.
- Os registros da base são fictícios e destinados a testes e demonstrações.
- O CSV utiliza vírgula como separador e, conforme o formato implementado, campos textuais não devem conter vírgulas.
- As datas são representadas internamente como inteiros no formato `AAAAMMDD`.
- A persistência é feita em arquivo CSV; o projeto não utiliza um sistema gerenciador de banco de dados.
- O sistema não substitui ferramentas profissionais de gestão farmacêutica nem realiza validação sanitária de medicamentos.

---

## Vídeo de apresentação

- Link Video: [TRABALHO 03](https://youtu.be/7B9tamIlS9w)

<div align="center">

## Membros da Equipe

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/Beatriz-ge">
        <img src="https://avatars.githubusercontent.com/u/129804255?v=4" width="120" alt="Beatriz Lins"><br>
        <sub><b>Beatriz Lins - 222021844 </b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/dudaa28">
        <img src="https://avatars.githubusercontent.com/u/164348330?v=4" width="120" alt="Maria Eduarda"><br>
        <sub><b>Maria Eduarda - 222037577 </b></sub>
      </a>
    </td>
  </tr>
</table>

</div>
