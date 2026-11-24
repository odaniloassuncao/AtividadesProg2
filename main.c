#include <stdio.h>
#include <stdlib.h>

#define TAM 11  

typedef struct No {
    int chave;
    struct No *prox;   
} No;

typedef struct {
    No *posicoes[TAM]; 
    int qtd;           
} TabelaHash;

int hash(int chave) {
    return ((chave % TAM) + TAM) % TAM; 
}

void inicializar(TabelaHash *t) {
    for (int i = 0; i < TAM; i++)
        t->posicoes[i] = NULL;
    t->qtd = 0;
}

No *buscar(TabelaHash *t, int chave) {
    No *atual = t->posicoes[hash(chave)];
    while (atual != NULL) {
        if (atual->chave == chave)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

int inserir(TabelaHash *t, int chave) {
    if (buscar(t, chave) != NULL)
        return 0; /* chave repetida */

    int pos = hash(chave);
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        exit(1);
    }
    novo->chave = chave;
    novo->prox = t->posicoes[pos]; 
    t->posicoes[pos] = novo;       
    t->qtd++;
    return 1;
}

int remover(TabelaHash *t, int chave) {
    int pos = hash(chave);
    No *atual = t->posicoes[pos];
    No *anterior = NULL;

    while (atual != NULL && atual->chave != chave) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
        return 0; 

    if (anterior == NULL)
        t->posicoes[pos] = atual->prox; 
    else
        anterior->prox = atual->prox;   

    free(atual);
    t->qtd--;
    return 1;
}

void listar(TabelaHash *t) {
    printf("\n---------------- CONTEUDO DA TABELA ----------------\n");
    for (int i = 0; i < TAM; i++) {
        printf("[%2d] ", i);
        No *atual = t->posicoes[i];
        if (atual == NULL)
            printf("(vazio)");
        while (atual != NULL) {
            printf("%d", atual->chave);
            if (atual->prox != NULL)
                printf(" -> ");
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("----------------------------------------------------\n");
}

void estatisticas(TabelaHash *t) {
    int ocupadas = 0, colisoes = 0, maiorLista = 0;

    for (int i = 0; i < TAM; i++) {
        int tamLista = 0;
        for (No *p = t->posicoes[i]; p != NULL; p = p->prox)
            tamLista++;

        if (tamLista > 0) ocupadas++;
        if (tamLista > 1) colisoes += tamLista - 1;
        if (tamLista > maiorLista) maiorLista = tamLista;
    }

    printf("\n---------------- ESTATISTICAS ----------------\n");
    printf("Tamanho da tabela .............. %d\n", TAM);
    printf("Elementos armazenados .......... %d\n", t->qtd);
    printf("Posicoes ocupadas .............. %d\n", ocupadas);
    printf("Posicoes vazias ................ %d\n", TAM - ocupadas);
    printf("Fator de carga ................. %.2f\n", (float) t->qtd / TAM);
    printf("Colisoes ....................... %d\n", colisoes);
    printf("Maior lista (pior caso de busca) %d\n", maiorLista);
    if (ocupadas > 0)
        printf("Media de elementos por lista ... %.2f (considerando so as ocupadas)\n",
               (float) t->qtd / ocupadas);
    printf("----------------------------------------------\n");
}


void destruir(TabelaHash *t) {
    for (int i = 0; i < TAM; i++) {
        No *atual = t->posicoes[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        t->posicoes[i] = NULL;
    }
    t->qtd = 0;
}


int lerInteiro(const char *mensagem) {
    int valor;
    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1) {
        while (getchar() != '\n'); 
        printf("Entrada invalida! %s", mensagem);
    }
    return valor;
}


int main(void) {
    TabelaHash tabela;
    inicializar(&tabela);

    int opcao, chave;

    do {
        printf("\n=======================>    TABELA HASH    <=======================\n");
        printf("Selecione uma opcao (digite apenas o valor inteiro):\n");
        printf(">(1) Inserir\n");
        printf(">(2) Remover\n");
        printf(">(3) Buscar\n");
        printf(">(4) Listar\n");
        printf(">(5) Estatisticas\n");
        printf(">(0) Sair\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao) {
            case 0:
                printf("Encerrando o programa...\n");
                break;

            case 1:
                chave = lerInteiro("Digite a chave a ser inserida: ");
                if (inserir(&tabela, chave))
                    printf("Chave %d inserida na posicao [%d].\n", chave, hash(chave));
                else
                    printf("A chave %d ja existe na tabela!\n", chave);
                break;

            case 2:
                chave = lerInteiro("Digite a chave a ser removida: ");
                if (remover(&tabela, chave))
                    printf("Chave %d removida com sucesso.\n", chave);
                else
                    printf("Chave %d nao encontrada!\n", chave);
                break;

            case 3:
                chave = lerInteiro("Digite a chave a ser buscada: ");
                if (buscar(&tabela, chave) != NULL)
                    printf("Chave %d encontrada na posicao [%d].\n", chave, hash(chave));
                else
                    printf("Chave %d nao encontrada!\n", chave);
                break;

            case 4:
                listar(&tabela);
                break;

            case 5:
                estatisticas(&tabela);
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    destruir(&tabela);
    return 0;
}