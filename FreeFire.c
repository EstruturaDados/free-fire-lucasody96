#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definição da constante para capacidade máxima da mochila
#define MAX_ITENS 10

/* * Definição da Struct Item
 * Armazena as características de cada objeto coletado no jogo.
 */
struct Item {
    char nome[30];
    char tipo[20]; // Ex: Arma, Munição, Cura
    int quantidade;
};

// --- Protótipos das Funções ---
void inserirItem(struct Item mochila[], int *contador);
void removerItem(struct Item mochila[], int *contador);
void listarItens(struct Item mochila[], int contador);
void buscarItem(struct Item mochila[], int contador);
void limparBuffer(); // Função utilitária para limpar o buffer do teclado

int main() {
    // Configuração para aceitar acentos (Português)
    setlocale(LC_ALL, "Portuguese");

    struct Item mochila[MAX_ITENS]; // Vetor de structs (a mochila)
    int totalItens = 0;             // Variável de controle da quantidade atual
    int opcao;

    do {
        // --- Interface do Usuário ---
        printf("\n=== MOCHILA TÁTICA DO JOGADOR ===\n");
        printf("Capacidade: %d/%d slots ocupados\n", totalItens, MAX_ITENS);
        printf("1. Coletar Item (Inserir)\n");
        printf("2. Jogar fora (Remover)\n");
        printf("3. Inventário (Listar)\n");
        printf("4. Procurar Item (Buscar)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o "Enter" residual do scanf anterior

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens); // Passamos o endereço de totalItens
                break;
            case 2:
                removerItem(mochila, &totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                buscarItem(mochila, totalItens);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

/*
 * Função: inserirItem
 * Objetivo: Adiciona um novo item ao final do vetor se houver espaço.
 * Parâmetros: Vetor mochila e ponteiro para o contador total.
 */
void inserirItem(struct Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("\n[!] A mochila está cheia! Remova itens antes de coletar novos.\n");
        return;
    }

    printf("\n--- Novo Loot ---\n");
    printf("Nome do item: ");
    // scanf com formatação para ler string com espaços (ex: "Kit Medico")
    scanf("%[^\n]", mochila[*contador].nome);
    limparBuffer();

    printf("Tipo (Arma, Municao, Cura): ");
    scanf("%[^\n]", mochila[*contador].tipo);
    limparBuffer();

    printf("Quantidade: ");
    scanf("%d", &mochila[*contador].quantidade);
    limparBuffer();

    (*contador)++; // Incrementa o total de itens (acessa via ponteiro)
    printf("[OK] Item guardado na mochila com sucesso!\n");
    
    // Lista itens atualizados conforme solicitado no requisito funcional
    listarItens(mochila, *contador); 
}

/*
 * Função: removerItem
 * Objetivo: Busca um item pelo nome e o remove, reorganizando o vetor.
 */
void removerItem(struct Item mochila[], int *contador) {
    char nomeBusca[30];
    int i, j, encontrado = 0;

    if (*contador == 0) {
        printf("\n[!] A mochila já está vazia.\n");
        return;
    }

    printf("\n--- Descartar Item ---\n");
    printf("Digite o nome do item para remover: ");
    scanf("%[^\n]", nomeBusca);
    limparBuffer();

    // Busca sequencial para encontrar o índice do item
    for (i = 0; i < *contador; i++) {
        // strcasecmp ou stricmp não são padrão ANSI C, então usamos strcmp (case sensitive)
        // Para ignorar maiúsculas/minúsculas em sistemas reais, converteríamos antes.
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = 1;
            
            // Lógica de Remoção: Deslocar os itens à direita para a esquerda
            // para preencher o "buraco" deixado pelo item removido.
            for (j = i; j < *contador - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            
            (*contador)--; // Decrementa o total
            printf("[OK] Item '%s' removido.\n", nomeBusca);
            break; // Para o loop pois já encontrou e removeu
        }
    }

    if (!encontrado) {
        printf("[!] Item não encontrado na mochila.\n");
    } else {
        listarItens(mochila, *contador); // Mostra lista atualizada
    }
}

/*
 * Função: listarItens
 * Objetivo: Percorre o vetor e exibe todos os itens cadastrados.
 */
void listarItens(struct Item mochila[], int contador) {
    printf("\n--- Conteúdo da Mochila ---\n");
    if (contador == 0) {
        printf("(Mochila Vazia)\n");
        return;
    }

    printf("%-5s | %-20s | %-15s | %s\n", "ID", "Nome", "Tipo", "Qtd");
    printf("------------------------------------------------------\n");
    
    for (int i = 0; i < contador; i++) {
        printf("%-5d | %-20s | %-15s | %d\n", 
               i + 1, 
               mochila[i].nome, 
               mochila[i].tipo, 
               mochila[i].quantidade);
    }
    printf("------------------------------------------------------\n");
}

/*
 * Função: buscarItem
 * Objetivo: Realiza uma busca sequencial por nome e exibe os detalhes.
 */
void buscarItem(struct Item mochila[], int contador) {
    char nomeBusca[30];
    int encontrado = 0;

    printf("\n--- Buscar na Mochila ---\n");
    printf("Nome do item: ");
    scanf("%[^\n]", nomeBusca);
    limparBuffer();

    for (int i = 0; i < contador; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[Item Encontrado!]\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1;
            break; // Encerra o loop ao encontrar
        }
    }

    if (!encontrado) {
        printf("[!] Item não localizado.\n");
    }
}

/*
 * Função Auxiliar: limparBuffer
 * Objetivo: Consome caracteres restantes no buffer de entrada (como \n)
 * para evitar erros de leitura com scanf e strings.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}