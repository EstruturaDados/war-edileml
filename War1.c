#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_TERRITORIOS 5

// --- Estruturas ---
typedef struct {
    char nome[50];
    char cor_exercito[30];
    int qtd_tropas;
} Territorio;

typedef enum {
    MISSAO_DESTRUIR_VERDE = 1,
    MISSAO_CONQUISTAR_TRES
} TipoMissao;

typedef struct {
    TipoMissao tipo;
    char descricao[100];
} Missao;

// --- Protótipos das Funções ---
void limparBuffer();
void removerNovaLinha(char *str);
void inicializarTerritorios(Territorio *territorios);
void exibirMapa(const Territorio *territorios);
void atribuirMissao(Missao *missao);
void realizarAtaque(Territorio *territorios);
int verificarMissao(const Territorio *territorios, const Missao *missao);

// --- Função Principal ---
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Alocação dinâmica utilizando calloc (Nível Aventureiro/Mestre)
    Territorio *mapa = (Territorio *)calloc(TOTAL_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    Missao missaoJogador;
    int opcao = 0;

    printf("=========================================\n");
    printf("     BEM-VINDO AO WAR ESTRUTURADO!       \n");
    printf("=========================================\n\n");

    // Inicialização do Jogo
    inicializarTerritorios(mapa);
    atribuirMissao(&missaoJogador);

    // Menu Interativo
    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ver Mapa Atual\n");
        printf("2. Atacar\n");
        printf("3. Verificar Missão\n");
        printf("0. Sair do Jogo\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            limparBuffer();
        }

        switch (opcao) {
            case 1:
                exibirMapa(mapa);
                break;
            case 2:
                realizarAtaque(mapa);
                break;
            case 3:
                if (verificarMissao(mapa, &missaoJogador)) {
                    printf("\n🎉 PARABÉNS! Você cumpriu sua missão e venceu o jogo! 🎉\n");
                    opcao = 0; // Encerra o jogo
                } else {
                    printf("\nA missão ainda não foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo. Até a próxima, General!\n");
                break;
            default:
                printf("\n⚠️ Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    // Liberação de memória (Boa prática essencial)
    free(mapa);
    return 0;
}

// --- Implementação das Funções ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void inicializarTerritorios(Territorio *territorios) {
    printf("-> Fase de Cadastro dos %d Territórios:\n", TOTAL_TERRITORIOS);
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        limparBuffer();
        printf("\n[Território %d]\n", i + 1);
        
        printf("Nome do Território: ");
        fgets(territorios[i].nome, 50, stdin);
        removerNovaLinha(territorios[i].nome);

        printf("Cor do Exército Dominante: ");
        fgets(territorios[i].cor_exercito, 30, stdin);
        removerNovaLinha(territorios[i].cor_exercito);

        printf("Quantidade Inicial de Tropas: ");
        while (scanf("%d", &territorios[i].qtd_tropas) != 1 || territorios[i].qtd_tropas < 1) {
            printf("Por favor, insira um número válido de tropas (mínimo 1): ");
            limparBuffer();
        }
    }
    printf("\n✅ Mapa inicializado com sucesso!\n");
}

void exibirMapa(const Territorio *territorios) {
    printf("\n================ MAPA ATUAL ================\n");
    printf("%-4s | %-20s | %-12s | %-6s\n", "ID", "Território", "Exército", "Tropas");
    printf("--------------------------------------------\n");
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        printf("%-4d | %-20s | %-12s | %-6d\n", 
               i + 1, territorios[i].nome, territorios[i].cor_exercito, territorios[i].qtd_tropas);
    }
    printf("============================================\n");
}

void atribuirMissao(Missao *missao) {
    // Sorteia entre a missão 1 e 2
    missao->tipo = (rand() % 2) + 1;
    
    if (missao->tipo == MISSAO_DESTRUIR_VERDE) {
        strcpy(missao->descricao, "Destruir todos os exércitos da cor \"Verde\".");
    } else {
        strcpy(missao->descricao, "Conquistar ao menos 3 territórios com a cor do seu exército inicial.");
    }
    
    printf("\n📜 SUA MISSÃO SECRETA: %s\n", missao->descricao);
}

void realizarAtaque(Territorio *territorios) {
    int atacante_id, defensor_id;

    exibirMapa(territorios);
    
    printf("\nEscolha o ID do território ATACANTE (1-%d): ", TOTAL_TERRITORIOS);
    scanf("%d", &atacante_id);
    printf("Escolha o ID do território DEFENSOR (1-%d): ", TOTAL_TERRITORIOS);
    scanf("%d", &defensor_id);

    // Validações básicas
    if (atacante_id < 1 || atacante_id > TOTAL_TERRITORIOS || defensor_id < 1 || defensor_id > TOTAL_TERRITORIOS) {
        printf("\n⚠️ IDs inválidos!\n");
        return;
    }
    if (atacante_id == defensor_id) {
        printf("\n⚠️ Um território não pode atacar a si mesmo!\n");
        return;
    }
    if (territorios[atacante_id - 1].qtd_tropas <= 1) {
        printf("\n⚠️ O atacante precisa de pelo menos 2 tropas para iniciar um ataque!\n");
        return;
    }

    // Ponteiros para facilitar a leitura do código
    Territorio *atq = &territorios[atacante_id - 1];
    Territorio *def = &territorios[defensor_id - 1];

    printf("\n⚔️ BATALHA: %s (%s) contra %s (%s) ⚔️\n", atq->nome, atq->cor_exercito, def->nome, def->cor_exercito);

    // Simulação de dados (1 a 6)
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    printf("🎲 Dado de Ataque: %d\n", dado_ataque);
    printf("🎲 Dado de Defesa: %d\n", dado_defesa);

    // Regra: Empates ou vitórias do atacante reduzem a tropa do defensor
    if (dado_ataque >= dado_defesa) {
        printf("💥 O atacante venceu o confronto!\n");
        def->qtd_tropas--;
        
        // Verifica se o território foi conquistado
        if (def->qtd_tropas <= 0) {
            printf("🚩 CONQUISTA! O território %s agora pertence ao exército %s!\n", def->nome, atq->cor_exercito);
            strcpy(def->cor_exercito, atq->cor_exercito);
            def->qtd_tropas = 1; // Nova tropa de ocupação
            atq->qtd_tropas--;  // Transfere a tropa do atacante
        }
    } else {
        printf("🛡️ O defensor segurou o ataque! O atacante perdeu 1 tropa.\n");
        atq->qtd_tropas--;
    }
}

int verificarMissao(const Territorio *territorios, const Missao *missao) {
    if (missao->tipo == MISSAO_DESTRUIR_VERDE) {
        // Se encontrar qualquer território com a cor "Verde", a missão ainda não acabou
        for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
            if (strcasecmp(territorios[i].cor_exercito, "Verde") == 0) {
                return 0; // Missão não cumprida
            }
        }
        return 1; // Missão cumprida (Nenhum "Verde" encontrado)
    } 
    else if (missao->tipo == MISSAO_CONQUISTAR_TRES) {
        // Para simplificar a lógica de "sua cor", assume-se a cor do Território 1 no início do turno
        // e conta quantos territórios possuem essa mesma cor atualmente.
        char cor_jogador[30];
        strcpy(cor_jogador, territorios[0].cor_exercito);
        
        int contagem = 0;
        for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
            if (strcasecmp(territorios[i].cor_exercito, cor_jogador) == 0) {
                contagem++;
            }
        }
        return (contagem >= 3);
    }
    return 0;
}