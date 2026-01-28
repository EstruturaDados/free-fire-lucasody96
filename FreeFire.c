#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definições e Constantes
#define MAX_ITENS 10

// --- Estruturas de Dados ---

// 1. A Struct Item (Dados)
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

// 2. O Nó para a Lista Encadeada
struct No {
    struct Item dados;
    struct No* proximo;
};

// --- Variáveis Globais de Desempenho (Contadores) ---
int compSeqVetor = 0;
int compBinVetor = 0;
int compSeqLista = 0;

// --- Protótipos: Vetor ---
void menuVetor(struct Item mochila[], int *contador);
void inserirItemVetor(struct Item mochila[], int *contador);
void removerItemVetor(struct Item mochila[], int *contador);
void listarVetor(struct Item mochila[], int contador);
void ordenarVetor(struct Item mochila[], int contador); // Bubble Sort
void buscarSequencialVetor(struct Item mochila[], int contador);
void buscarBinariaVetor(struct Item mochila[], int contador);

// --- Protótipos: Lista Encadeada ---
void menuLista(struct No** inicio);
void inserirItemLista(struct No** inicio);
void removerItemLista(struct No** inicio);
void listarLista(struct No* inicio);
void buscarSequencialLista(struct No* inicio);

// --- Utilitários ---
void limparBuffer();

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    setlocale(LC_ALL, "Portuguese");

    // Inicialização das estruturas
    struct Item mochilaVetor[MAX_ITENS];
    int totalVetor = 0;
    struct No* mochilaLista = NULL; // Ponteiro para o início da lista

    int opcaoPrincipal;

    do {
        printf("\n=== SISTEMA DE GESTÃO DE INVENTÁRIO ===\n");
        printf("Escolha a estrutura de dados para operar:\n");
        printf("1. 'Mochila com Vetor' (Sequencial + Busca Binária)\n");
        printf("2. 'Mochila com Lista' (Dinâmica)\n");
        printf("0. 'Sair'\n");
        printf("Opção: ");
        scanf("%d", &opcaoPrincipal);
        limparBuffer();

        switch (opcaoPrincipal) {
            case 1:
                menuVetor(mochilaVetor, &totalVetor);
                break;
            case 2:
                menuLista(&mochilaLista);
                break;
            case 0:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcaoPrincipal != 0);

    // Limpeza de memória da lista ao sair (boa prática)
    struct No* atual = mochilaLista;
    while (atual != NULL) {
        struct No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO: VETOR
// ============================================================================

void menuVetor(struct Item mochila[], int *contador) {
    int opcao;
    do {
        printf("\n--- MENU VETOR (Rápido acesso, tamanho fixo) ---\n");
        printf("Itens: %d/%d\n", *contador, MAX_ITENS);
        printf("1. Inserir\n2. Remover\n3. Listar\n4. Busca Sequencial\n");
        printf("5. Ordenar (Bubble Sort)\n6. Busca Binária (Requer ordenação)\n0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: inserirItemVetor(mochila, contador); break;
            case 2: removerItemVetor(mochila, contador); break;
            case 3: listarVetor(mochila, *contador); break;
            case 4: buscarSequencialVetor(mochila, *contador); break;
            case 5: ordenarVetor(mochila, *contador); break;
            case 6: buscarBinariaVetor(mochila, *contador); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while(opcao != 0);
}

void inserirItemVetor(struct Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("[!] Mochila cheia.\n");
        return;
    }
    printf("Nome: "); scanf("%[^\n]", mochila[*contador].nome); limparBuffer();
    printf("Tipo: "); scanf("%[^\n]", mochila[*contador].tipo); limparBuffer();
    printf("Qtd: "); scanf("%d", &mochila[*contador].quantidade); limparBuffer();
    (*contador)++;
    printf("[OK] Inserido no vetor.\n");
}

void removerItemVetor(struct Item mochila[], int *contador) {
    char alvo[30];
    printf("Remover item: "); scanf("%[^\n]", alvo); limparBuffer();
    int i, j, achou = 0;
    
    for (i = 0; i < *contador; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) {
            achou = 1;
            // Deslocamento (Shift Left) - Custo O(n)
            for (j = i; j < *contador - 1; j++) {
                mochila[j] = mochila[j+1];
            }
            (*contador)--;
            printf("[OK] Item removido.\n");
            break;
        }
    }
    if (!achou) printf("[!] Item não encontrado.\n");
}

void listarVetor(struct Item mochila[], int contador) {
    printf("\n[VETOR] Lista de Itens:\n");
    for (int i = 0; i < contador; i++) {
        printf("[%d] %s (%s) - Qtd: %d\n", i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Algoritmo de Ordenação: Bubble Sort
void ordenarVetor(struct Item mochila[], int contador) {
    struct Item temp;
    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            // Compara strings alfabeticamente
            if (strcmp(mochila[j].nome, mochila[j+1].nome) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j+1];
                mochila[j+1] = temp;
            }
        }
    }
    printf("[OK] Vetor ordenado por nome.\n");
    listarVetor(mochila, contador);
}

void buscarSequencialVetor(struct Item mochila[], int contador) {
    char alvo[30];
    printf("Buscar (Sequencial): "); scanf("%[^\n]", alvo); limparBuffer();
    
    compSeqVetor = 0; // Resetar contador
    int achou = 0;

    for (int i = 0; i < contador; i++) {
        compSeqVetor++; // Contabiliza comparação
        if (strcmp(mochila[i].nome, alvo) == 0) {
            printf("[ACHOU] Índice %d após %d comparações.\n", i, compSeqVetor);
            achou = 1;
            break;
        }
    }
    if (!achou) printf("[404] Não encontrado. Comparações: %d\n", compSeqVetor);
}

void buscarBinariaVetor(struct Item mochila[], int contador) {
    // Busca Binária só funciona se estiver ordenado!
    char alvo[30];
    printf("Buscar (Binária - requer vetor ordenado): "); scanf("%[^\n]", alvo); limparBuffer();

    int inicio = 0, fim = contador - 1, meio;
    compBinVetor = 0;
    int achou = 0;

    while (inicio <= fim) {
        compBinVetor++;
        meio = (inicio + fim) / 2;
        
        int r = strcmp(mochila[meio].nome, alvo);
        
        if (r == 0) {
            printf("[ACHOU] Índice %d após %d comparações.\n", meio, compBinVetor);
            achou = 1;
            break;
        } else if (r > 0) {
            fim = meio - 1; // Busca na metade esquerda
        } else {
            inicio = meio + 1; // Busca na metade direita
        }
    }
    if (!achou) printf("[404] Não encontrado. Comparações: %d\n", compBinVetor);
}

// ============================================================================
// IMPLEMENTAÇÃO: LISTA ENCADEADA
// ============================================================================

void menuLista(struct No** inicio) {
    int opcao;
    do {
        printf("\n--- MENU LISTA (Memória Dinâmica) ---\n");
        printf("1. Inserir\n2. Remover\n3. Listar\n4. Busca Sequencial\n0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: inserirItemLista(inicio); break;
            case 2: removerItemLista(inicio); break;
            case 3: listarLista(*inicio); break;
            case 4: buscarSequencialLista(*inicio); break;
            case 0: break;
            default: printf("Opção inválida.\n");
        }
    } while(opcao != 0);
}

void inserirItemLista(struct No** inicio) {
    struct No* novoNo = (struct No*) malloc(sizeof(struct No));
    if (novoNo == NULL) {
        printf("[Erro] Memória insuficiente.\n");
        return;
    }

    printf("Nome: "); scanf("%[^\n]", novoNo->dados.nome); limparBuffer();
    printf("Tipo: "); scanf("%[^\n]", novoNo->dados.tipo); limparBuffer();
    printf("Qtd: "); scanf("%d", &novoNo->dados.quantidade); limparBuffer();

    // Inserção no INÍCIO (O(1) - Muito rápida)
    novoNo->proximo = *inicio;
    *inicio = novoNo;
    printf("[OK] Inserido na lista.\n");
}

void removerItemLista(struct No** inicio) {
    char alvo[30];
    printf("Remover item: "); scanf("%[^\n]", alvo); limparBuffer();

    struct No* atual = *inicio;
    struct No* anterior = NULL;
    int achou = 0;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, alvo) == 0) {
            achou = 1;
            if (anterior == NULL) {
                // Remover o primeiro nó
                *inicio = atual->proximo;
            } else {
                // Remover nó do meio ou fim
                anterior->proximo = atual->proximo;
            }
            free(atual); // Libera memória
            printf("[OK] Nó removido e memória liberada.\n");
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    if (!achou) printf("[!] Item não encontrado na lista.\n");
}

void listarLista(struct No* inicio) {
    printf("\n[LISTA] Itens encadeados:\n");
    struct No* temp = inicio;
    int i = 0;
    if (temp == NULL) printf("(Lista Vazia)\n");
    while (temp != NULL) {
        printf("Nó %d -> %s (%s) Qtd: %d\n", i++, temp->dados.nome, temp->dados.tipo, temp->dados.quantidade);
        temp = temp->proximo;
    }
}

void buscarSequencialLista(struct No* inicio) {
    char alvo[30];
    printf("Buscar na Lista: "); scanf("%[^\n]", alvo); limparBuffer();

    compSeqLista = 0;
    struct No* temp = inicio;
    int achou = 0;

    while (temp != NULL) {
        compSeqLista++;
        if (strcmp(temp->dados.nome, alvo) == 0) {
            printf("[ACHOU] Item encontrado após %d comparações.\n", compSeqLista);
            achou = 1;
            break;
        }
        temp = temp->proximo;
    }
    if (!achou) printf("[404] Não encontrado. Comparações: %d\n", compSeqLista);
}

// ============================================================================
// UTILITÁRIO
// ============================================================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}