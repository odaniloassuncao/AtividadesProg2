
#include <stdio.h>
#include <stdlib.h>


typedef struct No {
    int chaves[3];
    struct No *filhos[4];
    int n;
    int folha;
} No;

No *criar_no(int folha) {
    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    novo->n = 0;
    novo->folha = folha;

    for (int i = 0; i < 4; i++) {
        novo->filhos[i] = NULL;
    }

    return novo;
}


void liberar_arvore(No *raiz) {
    if (raiz == NULL)
        return;

    for (int i = 0; i <= raiz->n; i++) {
        liberar_arvore(raiz->filhos[i]);
    }

    free(raiz);
}


No *buscar(No *raiz, int valor) {
    if (raiz == NULL)
        return NULL;

    int i = 0;

    while (i < raiz->n && valor > raiz->chaves[i]) {
        i++;
    }

    if (i < raiz->n && valor == raiz->chaves[i]) {
        return raiz;
    }

    if (raiz->folha) {
        return NULL;
    }

    return buscar(raiz->filhos[i], valor);
}


void dividir_no(No *pai, int indice) {
    No *cheio = pai->filhos[indice];

    No *novo = criar_no(cheio->folha);

    novo->n = 1;
    novo->chaves[0] = cheio->chaves[2];

    if (!cheio->folha) {
        novo->filhos[0] = cheio->filhos[2];
        novo->filhos[1] = cheio->filhos[3];

        cheio->filhos[2] = NULL;
        cheio->filhos[3] = NULL;
    }

    int chave_meio = cheio->chaves[1];

    cheio->n = 1;

    for (int j = pai->n; j >= indice + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[indice + 1] = novo;


    for (int j = pai->n - 1; j >= indice; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    pai->chaves[indice] = chave_meio;
    pai->n++;
}

void inserir_nao_cheio(No *no, int valor) {
    int i = no->n - 1;

    if (no->folha) {


        while (i >= 0 && valor < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = valor;
        no->n++;

    } else {

        while (i >= 0 && valor < no->chaves[i]) {
            i--;
        }

        i++;


        if (no->filhos[i]->n == 3) {

            dividir_no(no, i);

            if (valor > no->chaves[i]) {
                i++;
            }
        }

        inserir_nao_cheio(no->filhos[i], valor);
    }
}


void inserir(No **raiz, int valor) {

    if (buscar(*raiz, valor) != NULL) {
        printf("Valor %d ja existe na arvore.\n", valor);
        return;
    }


    if (*raiz == NULL) {
        *raiz = criar_no(1);
        (*raiz)->chaves[0] = valor;
        (*raiz)->n = 1;
        return;
    }


    if ((*raiz)->n == 3) {

        No *nova_raiz = criar_no(0);

        nova_raiz->filhos[0] = *raiz;

        dividir_no(nova_raiz, 0);

        *raiz = nova_raiz;
    }

    inserir_nao_cheio(*raiz, valor);
}


int encontrar_chave(No *no, int valor) {
    int indice = 0;

    while (indice < no->n && no->chaves[indice] < valor) {
        indice++;
    }

    return indice;
}


int obter_predecessor(No *no) {
    while (!no->folha) {
        no = no->filhos[no->n];
    }

    return no->chaves[no->n - 1];
}


int obter_sucessor(No *no) {
    while (!no->folha) {
        no = no->filhos[0];
    }

    return no->chaves[0];
}


void emprestar_anterior(No *pai, int indice) {
    No *filho = pai->filhos[indice];
    No *irmao = pai->filhos[indice - 1];

    for (int i = filho->n - 1; i >= 0; i--) {
        filho->chaves[i + 1] = filho->chaves[i];
    }


    if (!filho->folha) {
        for (int i = filho->n; i >= 0; i--) {
            filho->filhos[i + 1] = filho->filhos[i];
        }

        filho->filhos[0] = irmao->filhos[irmao->n];
        irmao->filhos[irmao->n] = NULL;
    }


    filho->chaves[0] = pai->chaves[indice - 1];


    pai->chaves[indice - 1] = irmao->chaves[irmao->n - 1];

    filho->n++;
    irmao->n--;
}


void emprestar_seguinte(No *pai, int indice) {
    No *filho = pai->filhos[indice];
    No *irmao = pai->filhos[indice + 1];


    filho->chaves[filho->n] = pai->chaves[indice];


    if (!filho->folha) {
        filho->filhos[filho->n + 1] = irmao->filhos[0];
    }


    pai->chaves[indice] = irmao->chaves[0];


    for (int i = 1; i < irmao->n; i++) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }


    if (!irmao->folha) {
        for (int i = 1; i <= irmao->n; i++) {
            irmao->filhos[i - 1] = irmao->filhos[i];
        }

        irmao->filhos[irmao->n] = NULL;
    }

    filho->n++;
    irmao->n--;
}


void fundir(No *pai, int indice) {
    No *filho = pai->filhos[indice];
    No *irmao = pai->filhos[indice + 1];


    filho->chaves[1] = pai->chaves[indice];


    for (int i = 0; i < irmao->n; i++) {
        filho->chaves[i + 2] = irmao->chaves[i];
    }

 
    if (!filho->folha) {
        for (int i = 0; i <= irmao->n; i++) {
            filho->filhos[i + 2] = irmao->filhos[i];
            irmao->filhos[i] = NULL;
        }
    }

    filho->n = 3;


    for (int i = indice + 1; i < pai->n; i++) {
        pai->chaves[i - 1] = pai->chaves[i];
    }


    for (int i = indice + 2; i <= pai->n; i++) {
        pai->filhos[i - 1] = pai->filhos[i];
    }

    pai->filhos[pai->n] = NULL;
    pai->n--;

    free(irmao);
}


void preencher_filho(No *pai, int indice) {


    if (indice > 0 && pai->filhos[indice - 1]->n >= 2) {
        emprestar_anterior(pai, indice);
    }


    else if (indice < pai->n &&
             pai->filhos[indice + 1]->n >= 2) {
        emprestar_seguinte(pai, indice);
    }


    else {

        if (indice < pai->n) {
            fundir(pai, indice);
        } else {
            fundir(pai, indice - 1);
        }
    }
}

void remover_no(No *no, int valor) {

    int indice = encontrar_chave(no, valor);

    if (indice < no->n && no->chaves[indice] == valor) {


        if (no->folha) {

            for (int i = indice + 1; i < no->n; i++) {
                no->chaves[i - 1] = no->chaves[i];
            }

            no->n--;
        }


        else {

            No *esquerdo = no->filhos[indice];
            No *direito = no->filhos[indice + 1];


            if (esquerdo->n >= 2) {

                int predecessor = obter_predecessor(esquerdo);

                no->chaves[indice] = predecessor;

                remover_no(esquerdo, predecessor);
            }


            else if (direito->n >= 2) {

                int sucessor = obter_sucessor(direito);

                no->chaves[indice] = sucessor;

                remover_no(direito, sucessor);
            }


            else {

                fundir(no, indice);

                remover_no(esquerdo, valor);
            }
        }

    } else {


        if (no->folha) {
            return;
        }

        int era_ultimo = (indice == no->n);

 
        if (no->filhos[indice]->n == 1) {
            preencher_filho(no, indice);
        }


        if (era_ultimo && indice > no->n) {
            remover_no(no->filhos[indice - 1], valor);
        } else {
            remover_no(no->filhos[indice], valor);
        }
    }
}


void remover(No **raiz, int valor) {

    if (*raiz == NULL) {
        printf("Arvore vazia.\n");
        return;
    }

    if (buscar(*raiz, valor) == NULL) {
        printf("Valor %d nao encontrado.\n", valor);
        return;
    }

    remover_no(*raiz, valor);


    if ((*raiz)->n == 0) {

        No *antiga_raiz = *raiz;

        if (antiga_raiz->folha) {
            *raiz = NULL;
        } else {
            *raiz = antiga_raiz->filhos[0];
            antiga_raiz->filhos[0] = NULL;
        }

        free(antiga_raiz);
    }

    printf("Valor %d removido com sucesso.\n", valor);
}


void percorrer_em_ordem(No *raiz) {

    if (raiz == NULL)
        return;

    int i;

    for (i = 0; i < raiz->n; i++) {

        if (!raiz->folha) {
            percorrer_em_ordem(raiz->filhos[i]);
        }

        printf("%d ", raiz->chaves[i]);
    }

    if (!raiz->folha) {
        percorrer_em_ordem(raiz->filhos[i]);
    }
}


int altura(No *raiz) {

    if (raiz == NULL)
        return 0;

    if (raiz->folha)
        return 1;

    return 1 + altura(raiz->filhos[0]);
}


void exibir_nivel(No *raiz, int nivel) {

    if (raiz == NULL)
        return;

    if (nivel == 0) {

        printf("[");

        for (int i = 0; i < raiz->n; i++) {
            printf("%d", raiz->chaves[i]);

            if (i < raiz->n - 1) {
                printf(" | ");
            }
        }

        printf("] ");

    } else {

        for (int i = 0; i <= raiz->n; i++) {
            exibir_nivel(raiz->filhos[i], nivel - 1);
        }
    }
}


void exibir_por_niveis(No *raiz) {

    if (raiz == NULL) {
        printf("Arvore vazia.\n");
        return;
    }

    int h = altura(raiz);

    for (int i = 0; i < h; i++) {

        printf("Nivel %d: ", i);

        exibir_nivel(raiz, i);

        printf("\n");
    }
}


int main() {

    No *raiz = NULL;

    int opcao, valor;

    do {

        printf("\n========== ARVORE 2-3-4 ==========\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer em ordem\n");
        printf("5 - Exibir arvore por niveis\n");
        printf("0 - Sair\n");
        printf("=================================\n");

        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            continue;
        }

        switch (opcao) {

            case 1:
                printf("Digite o valor a inserir: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido.\n");
                    while (getchar() != '\n');
                    break;
                }

                inserir(&raiz, valor);
                printf("Operacao de insercao concluida.\n");
                break;

            case 2:
                printf("Digite o valor a buscar: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido.\n");
                    while (getchar() != '\n');
                    break;
                }

                if (buscar(raiz, valor) != NULL) {
                    printf("Valor %d encontrado!\n", valor);
                } else {
                    printf("Valor %d nao encontrado.\n", valor);
                }
                break;

            case 3:
                printf("Digite o valor a remover: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido.\n");
                    while (getchar() != '\n');
                    break;
                }

                remover(&raiz, valor);
                break;

            case 4:
                printf("Percurso em ordem: ");

                if (raiz == NULL) {
                    printf("Arvore vazia.");
                } else {
                    percorrer_em_ordem(raiz);
                }

                printf("\n");
                break;

            case 5:
                printf("\nArvore por niveis:\n");
                exibir_por_niveis(raiz);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberar_arvore(raiz);

    return 0;
}
