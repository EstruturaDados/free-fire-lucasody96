#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_COMP 20

// 1. Criação da Struct conforme requisitos
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 (Baixa) a 10 (Alta/Crítica)
} Componente;

// Variável global para controle de estado (segurança da busca binária)
int ordenadoPorNome = 0; 

// --- Protótipos das Funções ---
void carregarDadosIniciais(Componente lista[], int *qtd);
void mostrarComponentes(Componente lista[], int qtd);
void bubbleSortNome(Componente lista[], int qtd);
void insertionSortTipo(Componente lista[], int qtd);
void selectionSortPrioridade(Componente lista[], int qtd);
int buscaBinariaPorNome(Componente lista[], int qtd, char chave[]);
void limparBuffer();

int main() {
    setlocale(LC_ALL, "Portuguese");
    Componente lista[MAX_COMP];
    int qtd = 0;
    int opcao;
    char termoBusca[30];
    int indiceEncontrado;
    
    // Carrega dados simulados para facilitar o teste (cenário de jogo)
    carregarDadosIniciais(lista, &qtd);

    do {
        printf("\n=== SISTEMA DE MONTAGEM DA TORRE DE FUGA ===\n");
        printf("Componentes atuais: %d/%d\n", qtd, MAX_COMP);
        printf("Estado: %s\n", ordenadoPorNome ? "Pronto para Busca Binária" : "Desordenado/Outra Ordem");
        printf("------------------------------------------------\n");
        printf("1. 'Listar componentes'\n");
        printf("2. 'Ordenar por Nome' (Bubble Sort)\n");
        printf("3. 'Ordenar por Tipo' (Insertion Sort)\n");
        printf("4. 'Ordenar por Prioridade' (Selection Sort)\n");
        printf("5. 'Buscar Componente-Chave' (Busca Binária)\n");
        printf("6. 'Adicionar Componente Manualmente'\n");
        printf("0. 'Sair'\n");
        printf("Escolha uma estratégia: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1:
                mostrarComponentes(lista, qtd);
                break;
            case 2:
                bubbleSortNome(lista, qtd);
                ordenadoPorNome = 1; // Habilita busca binária
                break;
            case 3:
                insertionSortTipo(lista, qtd);
                ordenadoPorNome = 0; // Desabilita busca binária (ordem mudou)
                break;
            case 4:
                selectionSortPrioridade(lista, qtd);
                ordenadoPorNome = 0; // Desabilita busca binária
                break;
            case 5:
                if (ordenadoPorNome) {
                    printf("Digite o nome do componente-chave: ");
                    if (fgets(termoBusca, sizeof(termoBusca), stdin)) {
                        termoBusca[strcspn(termoBusca, "\n")] = 0; // Remove \n
                        indiceEncontrado = buscaBinariaPorNome(lista, qtd, termoBusca);
                        
                        if (indiceEncontrado != -1) {
                            printf("\n[SUCESSO] Componente '%s' localizado no slot %d.\n", 
                                   lista[indiceEncontrado].nome, indiceEncontrado + 1);
                            printf("Status: PRONTO PARA INSTALAÇÃO.\n");
                        } else {
                            printf("\n[ALERTA] Componente não encontrado na lista.\n");
                        }
                    }
                } else {
                    printf("\n[ERRO TÁTICO] A lista precisa estar ordenada por NOME para a Busca Binária.\n");
                    printf("Execute a opção 'Ordenar por Nome' primeiro.\n");
                }
                break;
            case 6:
                if (qtd < MAX_COMP) {
                    printf("Nome: "); fgets(lista[qtd].nome, 30, stdin);
                    lista[qtd].nome[strcspn(lista[qtd].nome, "\n")] = 0;
                    printf("Tipo: "); fgets(lista[qtd].tipo, 20, stdin);
                    lista[qtd].tipo[strcspn(lista[qtd].tipo, "\n")] = 0;
                    printf("Prioridade (1-10): "); scanf("%d", &lista[qtd].prioridade);
                    limparBuffer();
                    qtd++;
                    ordenadoPorNome = 0; // Novo item pode quebrar a ordenação
                } else {
                    printf("Capacidade máxima atingida!\n");
                }
                break;
            case 0:
                printf("Encerrando sistema da torre...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
        
        if(opcao != 0 && opcao != 1) {
             // Pausa para leitura após operações que geram logs
             printf("\nPressione Enter para continuar...");
             getchar();
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação dos Algoritmos ---

/* * Bubble Sort: Ordena por NOME
 * Característica: Compara pares adjacentes. Simples, mas O(n^2).
 */
void bubbleSortNome(Componente lista[], int qtd) {
    Componente aux;
    long comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            comparacoes++;
            if (strcmp(lista[j].nome, lista[j+1].nome) > 0) {
                aux = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = aux;
            }
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n--- Relatório Bubble Sort (Nome) ---\n");
    printf("Comparações realizadas: %ld\n", comparacoes);
    printf("Tempo de execução: %f segundos\n", tempo);
    mostrarComponentes(lista, qtd);
}

/* * Insertion Sort: Ordena por TIPO
 * Característica: Constrói a lista ordenada um item por vez. Eficiente para listas pequenas.
 */
void insertionSortTipo(Componente lista[], int qtd) {
    Componente pivo;
    int j;
    long comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 1; i < qtd; i++) {
        pivo = lista[i];
        j = i - 1;

        // Note: Contar comparações dentro do while é complexo, aproximamos aqui:
        while (j >= 0) {
            comparacoes++;
            if (strcmp(lista[j].tipo, pivo.tipo) > 0) {
                lista[j + 1] = lista[j];
                j = j - 1;
            } else {
                break; 
            }
        }
        lista[j + 1] = pivo;
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n--- Relatório Insertion Sort (Tipo) ---\n");
    printf("Comparações (aprox): %ld\n", comparacoes);
    printf("Tempo de execução: %f segundos\n", tempo);
    mostrarComponentes(lista, qtd);
}

/* * Selection Sort: Ordena por PRIORIDADE (Decrescente: Maior prioridade primeiro)
 * Característica: Seleciona o maior/menor elemento e o coloca na posição correta.
 */
void selectionSortPrioridade(Componente lista[], int qtd) {
    int idxMaior;
    Componente aux;
    long comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < qtd - 1; i++) {
        idxMaior = i;
        for (int j = i + 1; j < qtd; j++) {
            comparacoes++;
            // Ordenando de forma decrescente (Maior prioridade no topo)
            if (lista[j].prioridade > lista[idxMaior].prioridade) {
                idxMaior = j;
            }
        }
        if (idxMaior != i) {
            aux = lista[i];
            lista[i] = lista[idxMaior];
            lista[idxMaior] = aux;
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n--- Relatório Selection Sort (Prioridade) ---\n");
    printf("Comparações realizadas: %ld\n", comparacoes);
    printf("Tempo de execução: %f segundos\n", tempo);
    mostrarComponentes(lista, qtd);
}

/*
 * Busca Binária: Requer vetor ordenado por NOME
 * Complexidade: O(log n) - Divide o espaço de busca pela metade a cada passo.
 */
int buscaBinariaPorNome(Componente lista[], int qtd, char chave[]) {
    int inicio = 0;
    int fim = qtd - 1;
    int meio;
    int comparacoes = 0;
    int resultado;

    printf("\n--- Iniciando Busca Binária ---\n");

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;
        
        resultado = strcmp(chave, lista[meio].nome);

        if (resultado == 0) {
            printf("Total de comparações na busca: %d\n", comparacoes);
            return meio;
        } else if (resultado < 0) {
            fim = meio - 1; // Busca na metade esquerda
        } else {
            inicio = meio + 1; // Busca na metade direita
        }
    }
    
    printf("Total de comparações na busca: %d\n", comparacoes);
    return -1; // Não encontrado
}

// --- Funções Auxiliares ---

void mostrarComponentes(Componente lista[], int qtd) {
    printf("\n%-25s | %-15s | %s\n", "COMPONENTE", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-25s | %-15s | %d\n", lista[i].nome, lista[i].tipo, lista[i].prioridade);
    }
    printf("----------------------------------------------------------\n");
}

void carregarDadosIniciais(Componente lista[], int *qtd) {
    // Simulando itens coletados no jogo
    strcpy(lista[0].nome, "Modulo de Energia"); strcpy(lista[0].tipo, "Energia"); lista[0].prioridade = 10;
    strcpy(lista[1].nome, "Antena de Longo Alcance"); strcpy(lista[1].tipo, "Comunicacao"); lista[1].prioridade = 8;
    strcpy(lista[2].nome, "Cabo Reforçado"); strcpy(lista[2].tipo, "Suporte"); lista[2].prioridade = 2;
    strcpy(lista[3].nome, "Processador Quântico"); strcpy(lista[3].tipo, "Controle"); lista[3].prioridade = 9;
    strcpy(lista[4].nome, "Bateria Auxiliar"); strcpy(lista[4].tipo, "Energia"); lista[4].prioridade = 5;
    strcpy(lista[5].nome, "Fusível de Proteção"); strcpy(lista[5].tipo, "Seguranca"); lista[5].prioridade = 7;
    strcpy(lista[6].nome, "Refrigerador Liquido"); strcpy(lista[6].tipo, "Suporte"); lista[6].prioridade = 4;
    strcpy(lista[7].nome, "Chip de Criptografia"); strcpy(lista[7].tipo, "Controle"); lista[7].prioridade = 6;
    *qtd = 8;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}