#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define TAM_PALAVRA 256
#define CAPACIDADE_INICIAL 11
#define FATOR_CARGA_MAX 0.75

typedef struct Entrada {
    char *chave;
    struct Entrada *prox;
} Entrada;

typedef struct {
    Entrada **baldes;
    size_t capacidade;
    size_t tamanho;
} Tabela;


static int eh_primo(size_t n) {
    if (n < 2) return 0;
    for (size_t i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

static size_t proximo_primo(size_t n) {
    while (!eh_primo(n)) n++;
    return n;
}


uint32_t funcao_hash(const char *texto) {
    uint32_t h = 2166136261u;
    for (int i = 0; texto[i] != '\0'; i++) {
        h ^= (unsigned char)texto[i];
        h *= 16777619u;
    }
    return h;
}

void normalizar(char *palavra) {
    for (int i = 0; palavra[i] != '\0'; i++) {
        unsigned char c = (unsigned char)palavra[i];
        if (c < 128) {
            palavra[i] = (char)tolower(c);
        } else if (c == 0xC3 && palavra[i + 1] != '\0') {
            unsigned char seguinte = (unsigned char)palavra[i + 1];
            if ((seguinte >= 0x80 && seguinte <= 0x96) ||
                (seguinte >= 0x98 && seguinte <= 0x9E))
                palavra[i + 1] = (char)(seguinte + 0x20);
        }
    }
}

Entrada *criar_entrada(const char *chave) {
    Entrada *nova = (Entrada *)malloc(sizeof(Entrada));
    if (nova == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    size_t tam = strlen(chave) + 1;
    nova->chave = (char *)malloc(tam);
    if (nova->chave == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(nova->chave, chave, tam);
    nova->prox = NULL;
    return nova;
}

Tabela *criar_tabela(size_t capacidade) {
    Tabela *t = (Tabela *)malloc(sizeof(Tabela));
    if (t == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    t->capacidade = capacidade;
    t->tamanho = 0;
    t->baldes = (Entrada **)calloc(capacidade, sizeof(Entrada *));
    if (t->baldes == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return t;
}


int buscar_palavra(Tabela *t, const char *palavra) {
    size_t indice = funcao_hash(palavra) % t->capacidade;
    Entrada *atual = t->baldes[indice];
    while (atual != NULL) {
        if (strcmp(atual->chave, palavra) == 0) return 1;
        atual = atual->prox;
    }
    return 0;
}

void redimensionar(Tabela *t) {
    size_t nova_cap = proximo_primo(t->capacidade * 2 + 1);
    Entrada **novos = (Entrada **)calloc(nova_cap, sizeof(Entrada *));
    if (novos == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *atual = t->baldes[i];
        while (atual != NULL) {
            Entrada *proxima = atual->prox;          
            size_t idx = funcao_hash(atual->chave) % nova_cap;
            atual->prox = novos[idx];               
            novos[idx] = atual;
            atual = proxima;
        }
    }
    free(t->baldes);
    t->baldes = novos;
    t->capacidade = nova_cap;
}

int inserir(Tabela *t, const char *palavra) {
    if (buscar_palavra(t, palavra)) return 0;

    if ((double)(t->tamanho + 1) / (double)t->capacidade > FATOR_CARGA_MAX)
        redimensionar(t);

    size_t indice = funcao_hash(palavra) % t->capacidade;
    Entrada *nova = criar_entrada(palavra);
    nova->prox = t->baldes[indice];
    t->baldes[indice] = nova;
    t->tamanho++;
    return 1;
}

int remover_palavra(Tabela *t, const char *palavra) {
    size_t indice = funcao_hash(palavra) % t->capacidade;
    Entrada *atual = t->baldes[indice];
    Entrada *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->chave, palavra) == 0) {
            if (anterior == NULL) t->baldes[indice] = atual->prox;
            else anterior->prox = atual->prox;
            free(atual->chave);
            free(atual);
            t->tamanho--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

void listar_todas(Tabela *t) {
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *atual = t->baldes[i];
        while (atual != NULL) {
            printf("%s\n", atual->chave);
            atual = atual->prox;
        }
    }
}

void desenhar_tabela(Tabela *t) {
    int mostrar_vazios = (t->capacidade <= 50);
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *atual = t->baldes[i];
        if (atual == NULL && !mostrar_vazios) continue;
        printf("[%3zu] ", i);
        while (atual != NULL) {
            printf("%s -> ", atual->chave);
            atual = atual->prox;
        }
        printf("NULL\n");
    }
    if (!mostrar_vazios)
        printf("(baldes vazios omitidos)\n");
}

void mostrar_estatisticas(Tabela *t) {
    size_t usados = 0, maior_cadeia = 0;
    for (size_t i = 0; i < t->capacidade; i++) {
        size_t tamanho_cadeia = 0;
        for (Entrada *e = t->baldes[i]; e != NULL; e = e->prox) tamanho_cadeia++;
        if (tamanho_cadeia > 0) usados++;
        if (tamanho_cadeia > maior_cadeia) maior_cadeia = tamanho_cadeia;
    }
    printf("Capacidade (baldes)    : %zu\n", t->capacidade);
    printf("Palavras armazenadas   : %zu\n", t->tamanho);
    printf("Baldes ocupados        : %zu\n", usados);
    printf("Colisoes               : %zu\n", t->tamanho - usados);
    printf("Maior cadeia           : %zu\n", maior_cadeia);
    printf("Fator de carga         : %.2f (limite %.2f)\n",
           (double)t->tamanho / (double)t->capacidade, FATOR_CARGA_MAX);
}

void liberar_tabela(Tabela *t) {
    if (t == NULL) return;
    for (size_t i = 0; i < t->capacidade; i++) {
        Entrada *atual = t->baldes[i];
        while (atual != NULL) {
            Entrada *proxima = atual->prox;
            free(atual->chave);
            free(atual);
            atual = proxima;
        }
    }
    free(t->baldes);
    free(t);
}


void ler_palavra(char *destino, size_t tamanho, const char *mensagem) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\r\n")] = '\0';
    normalizar(destino);
}

int main(void) {
    Tabela *tabela = criar_tabela(CAPACIDADE_INICIAL);
    char palavra[TAM_PALAVRA], linha[32];
    int opcao;

    do {
        printf("\n========== TABELA HASH ==========\n");
        printf("1 - Inserir palavra\n");
        printf("2 - Buscar palavra\n");
        printf("3 - Remover palavra\n");
        printf("4 - Listar todas as palavras\n");
        printf("5 - Desenhar a tabela\n");
        printf("6 - Estatisticas (colisoes, carga)\n");
        printf("7 - Teste de estresse (inserir 50 palavras)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        if (sscanf(linha, "%d", &opcao) != 1) {
            printf("Opcao invalida. Digite um numero do menu.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra: ");
                if (palavra[0] == '\0') printf("Palavra vazia nao pode ser inserida.\n");
                else if (inserir(tabela, palavra)) printf("Palavra inserida com sucesso.\n");
                else printf("Essa palavra ja existe na tabela.\n");
                break;

            case 2:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra que deseja buscar: ");
                if (buscar_palavra(tabela, palavra)) printf("Palavra encontrada!\n");
                else printf("Palavra nao encontrada.\n");
                break;

            case 3:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra que deseja remover: ");
                if (remover_palavra(tabela, palavra)) printf("Palavra removida com sucesso.\n");
                else printf("Essa palavra nao existe na tabela.\n");
                break;

            case 4:
                printf("Palavras armazenadas:\n");
                if (tabela->tamanho == 0) printf("(tabela vazia)\n");
                else listar_todas(tabela);
                break;

            case 5:
                printf("Representacao da tabela (cada [i] e um balde):\n");
                desenhar_tabela(tabela);
                break;

            case 6:
                mostrar_estatisticas(tabela);
                break;

            case 7:
                for (int i = 0; i < 50; i++) {
                    char gerada[32];
                    sprintf(gerada, "palavra%d", i);
                    inserir(tabela, gerada);
                }
                printf("50 palavras inseridas. Veja a opcao 6 para as estatisticas.\n");
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberar_tabela(tabela);
    return 0;
}