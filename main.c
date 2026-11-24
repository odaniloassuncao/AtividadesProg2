
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_ALFABETO 256
#define TAM_PALAVRA 256

typedef struct No {
    struct No *filhos[TAM_ALFABETO];
    int fim_da_palavra;
} No;

No *criar_no(void) {
    No *novo = (No *)calloc(1, sizeof(No));

    if (novo == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    return novo;
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

void inserir(No *raiz, const char *palavra) {
    No *atual = raiz;

    for (int i = 0; palavra[i] != '\0'; i++) {
        unsigned char indice = (unsigned char)palavra[i];

        if (atual->filhos[indice] == NULL)
            atual->filhos[indice] = criar_no();

        atual = atual->filhos[indice];
    }

    atual->fim_da_palavra = 1;
}

No *buscar_no(No *raiz, const char *texto) {
    No *atual = raiz;

    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char indice = (unsigned char)texto[i];

        if (atual->filhos[indice] == NULL)
            return NULL;

        atual = atual->filhos[indice];
    }

    return atual;
}

int buscar_palavra(No *raiz, const char *palavra) {
    No *no = buscar_no(raiz, palavra);

    return no != NULL && no->fim_da_palavra;
}

void listar_recursivo(No *no, char *buffer, int profundidade) {
    if (no->fim_da_palavra) {
        buffer[profundidade] = '\0';
        printf("%s\n", buffer);
    }

    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (no->filhos[i] != NULL) {
            buffer[profundidade] = (char)i;

            listar_recursivo(no->filhos[i], buffer, profundidade + 1);
        }
    }
}

void listar_recursivo_sem_prefixo(No *no, char *buffer, int profundidade, int *quantidade) {
    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (no->filhos[i] != NULL) {
            buffer[profundidade] = (char)i;
            buffer[profundidade + 1] = '\0';

            No *filho = no->filhos[i];

            if (filho->fim_da_palavra) {
                printf("%s\n", buffer);
                (*quantidade)++;
            }

            listar_recursivo_sem_prefixo(filho, buffer, profundidade + 1, quantidade);
        }
    }
}

void buscar_por_prefixo(No *raiz, const char *prefixo) {
    No *no = buscar_no(raiz, prefixo);

    if (no == NULL) {
        printf("Nenhuma palavra encontrada com esse prefixo.\n");
        return;
    }

    char buffer[TAM_PALAVRA + 1];
    size_t tamanho = strlen(prefixo);

    if (tamanho >= sizeof(buffer)) {
        printf("Prefixo muito longo.\n");
        return;
    }

    strcpy(buffer, prefixo);

    int quantidade = 0;

    if (no->fim_da_palavra) {
        printf("%s\n", prefixo);
        quantidade++;
    }

    listar_recursivo_sem_prefixo(no, buffer, (int)tamanho, &quantidade);

    if (quantidade == 0)
        printf("Nenhuma palavra encontrada com esse prefixo.\n");
}

int tem_filhos(No *no) {
    for (int i = 0; i < TAM_ALFABETO; i++)
        if (no->filhos[i] != NULL)
            return 1;

    return 0;
}

int remover_recursivo(No *no, const unsigned char *palavra, int profundidade) {
    if (palavra[profundidade] == '\0') {
        if (!no->fim_da_palavra)
            return 0;

        no->fim_da_palavra = 0;

        return !tem_filhos(no);
    }

    unsigned char indice = palavra[profundidade];

    No *filho = no->filhos[indice];

    if (filho == NULL)
        return 0;

    int pode_apagar_filho = remover_recursivo(filho, palavra, profundidade + 1);

    if (pode_apagar_filho) {
        free(filho);
        no->filhos[indice] = NULL;
    }

    return !no->fim_da_palavra && !tem_filhos(no);
}

int remover_palavra(No *raiz, const char *palavra) {
    if (!buscar_palavra(raiz, palavra))
        return 0;

    remover_recursivo(raiz, (const unsigned char *)palavra, 0);

    return 1;
}

void liberar_arvore(No *no) {
    if (no == NULL)
        return;

    for (int i = 0; i < TAM_ALFABETO; i++)
        liberar_arvore(no->filhos[i]);

    free(no);
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
    No *raiz = criar_no();

    char palavra[TAM_PALAVRA], prefixo[TAM_PALAVRA], linha[32];

    int opcao;

    do {
        printf("\n========== ARVORE TRIE ==========\n");
        printf("1 - Inserir palavra\n");
        printf("2 - Buscar palavra\n");
        printf("3 - Buscar por prefixo\n");
        printf("4 - Remover palavra\n");
        printf("5 - Listar todas as palavras\n");
        printf("6 - Carregar exemplo (casa, casal, caso, cama)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(linha, sizeof(linha), stdin) == NULL)
            break;

        if (sscanf(linha, "%d", &opcao) != 1) {
            printf("Opcao invalida. Digite um numero do menu.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra: ");

                if (palavra[0] == '\0')
                    printf("Palavra vazia nao pode ser inserida.\n");
                else {
                    inserir(raiz, palavra);
                    printf("Palavra inserida com sucesso.\n");
                }
                break;

            case 2:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra que deseja buscar: ");

                if (buscar_palavra(raiz, palavra))
                    printf("Palavra encontrada!\n");
                else
                    printf("Palavra nao encontrada.\n");
                break;

            case 3:
                ler_palavra(prefixo, sizeof(prefixo), "Digite o prefixo: ");

                if (prefixo[0] == '\0')
                    printf("Prefixo vazio. Digite ao menos um caractere.\n");
                else {
                    printf("Palavras com o prefixo '%s':\n", prefixo);
                    buscar_por_prefixo(raiz, prefixo);
                }
                break;

            case 4:
                ler_palavra(palavra, sizeof(palavra), "Digite a palavra que deseja remover: ");

                if (remover_palavra(raiz, palavra))
                    printf("Palavra removida com sucesso.\n");
                else
                    printf("Essa palavra nao existe na arvore.\n");
                break;

            case 5: {
                char buffer[TAM_PALAVRA + 1];

                printf("Palavras armazenadas:\n");

                if (!tem_filhos(raiz) && !raiz->fim_da_palavra)
                    printf("(arvore vazia)\n");
                else
                    listar_recursivo(raiz, buffer, 0);

                break;
            }

            case 6:
                inserir(raiz, "casa");
                inserir(raiz, "casal");
                inserir(raiz, "caso");
                inserir(raiz, "cama");

                printf("Exemplo carregado.\n");
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    liberar_arvore(raiz);

    return 0;
}
