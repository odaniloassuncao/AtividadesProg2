#include <stdio.h>
#include <stdlib.h>

int ler_inteiro(void)
{
    int valor;
    while (scanf("%d", &valor) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("Entrada invalida! Digite um numero inteiro: ");
    }
    return valor;
}

typedef struct no
{
    int valor;
    struct no *direita;
    struct no *esquerda;
} NoArv;

NoArv *inserir(NoArv *raiz, int num)
{
    if (raiz == NULL)
    {
        NoArv *aux = malloc(sizeof(NoArv));
        aux->valor = num;
        aux->esquerda = NULL;
        aux->direita = NULL;
        return aux;
    }
    else
    {
        if (num < raiz->valor)
        {
            raiz->esquerda = inserir(raiz->esquerda, num);
        }
        else
        {
            raiz->direita = inserir(raiz->direita, num);
        }
        return raiz;
    }
}

NoArv *buscar(NoArv *raiz, int num)
{
    if (raiz != NULL)
    {
        if (num == raiz->valor)
        {
            return raiz;
        }
        else if (num < raiz->valor)
        {
            return buscar(raiz->esquerda, num);
        }
        else
        {
            return buscar(raiz->direita, num);
        }
    }
    return NULL;
}

NoArv *remover(NoArv *raiz, int chave)
{
    if (raiz == NULL)
    {
        printf("Valor nao encontrado!");
        return NULL;
    }
    else
    {
        if (raiz->valor == chave)
        {
            if (raiz->esquerda == NULL && raiz->direita == NULL)
            {
                free(raiz);
                printf("No removido: %d", chave);
                return NULL;
            }
            else
            {
                if (raiz->esquerda != NULL && raiz->direita != NULL)
                {
                    NoArv *aux = raiz->esquerda;
                    while (aux->direita != NULL)
                    {
                        aux = aux->direita;
                    }
                    raiz->valor = aux->valor;
                    aux->valor = chave;
                    raiz->esquerda = remover(raiz->esquerda, chave);
                    return raiz;
                }
                else
                {
                    NoArv *aux;
                    if (raiz->esquerda != NULL)
                    {
                        aux = raiz->esquerda;
                    }
                    else
                    {
                        aux = raiz->direita;
                    }
                    free(raiz);
                    printf("No removido: %d", chave);
                    return aux;
                }
            }
        }
        else
        {
            if (chave < raiz->valor)
            {
                raiz->esquerda = remover(raiz->esquerda, chave);
            }
            else
            {
                raiz->direita = remover(raiz->direita, chave);
            }
            return raiz;
        }
    }
}

void pre_ordem(NoArv *raiz)
{
    printf("%d ", raiz->valor);
    if (raiz->esquerda != NULL)
    {
        pre_ordem(raiz->esquerda);
    }
    if (raiz->direita != NULL)
    {
        pre_ordem(raiz->direita);
    }
}

void em_ordem(NoArv *raiz)
{
    if (raiz->esquerda != NULL)
    {
        em_ordem(raiz->esquerda);
    }
    printf("%d ", raiz->valor);
    if (raiz->direita != NULL)
    {
        em_ordem(raiz->direita);
    }
}

void pos_ordem(NoArv *raiz)
{
    if (raiz->esquerda != NULL)
    {
        pos_ordem(raiz->esquerda);
    }
    if (raiz->direita != NULL)
    {
        pos_ordem(raiz->direita);
    }
    printf("%d ", raiz->valor);
}

void liberar_arvore(NoArv *raiz)
{
    if (raiz == NULL)
    {
        return;
    }
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

int main()
{
    int opcao, valor, ordem;
    NoArv *busca, *raiz = NULL;

    do
    {
        printf("\n=======================>    MENU    <=======================\n");
        printf("Selecione uma opcao (digite apenas o valor inteiro):\n");
        printf(">(1) Inserir valor\n");
        printf(">(2) Buscar valor\n");
        printf(">(3) Remover valor\n");
        printf(">(4) Percorrer arvore\n");
        printf(">(0) Sair\n");
        printf("Opcao: ");
        opcao = ler_inteiro();

        switch (opcao)
        {
        case 1:
            printf("Digite um numero que deseja inserir: ");
            valor = ler_inteiro();
            raiz = inserir(raiz, valor);
            printf("Numero inserido com sucesso!\n");
            break;

        case 2:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia(nenhum valor inserido)!\n");
                break;
            }
            else
            {
                printf("Digite um numero que deseja buscar: ");
                valor = ler_inteiro();
                busca = buscar(raiz, valor);
                if (busca)
                {
                    printf("\n Numero encontrado: %d\n", busca->valor);
                }
                else
                {
                    printf("Numero nao encontrado.\n");
                }
                break;
            }
        case 3:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia(nenhum valor inserido)!\n");
                break;
            }
            else
            {

                printf("Digite um valor que deseja remover: ");
                valor = ler_inteiro();
                raiz = remover(raiz, valor);
                break;
            }
        case 4:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }
            else
            {
                printf("Escolha um percurso: \n");
                printf("(1) Pre-ordem\n");
                printf("(2) Em ordem\n");
                printf("(3) Pos-ordem\n");
                printf("Opcao: \n");
                ordem = ler_inteiro();
                switch (ordem)
                {
                case 1:
                    pre_ordem(raiz);
                    break;
                case 2:
                    em_ordem(raiz);
                    break;
                case 3:
                    pos_ordem(raiz);
                    break;
                default:
                    printf("Opcao invalida, tente novamente\n");
                    break;
                }
                break;
            }

        default:
            if (opcao != 0)
            {
                printf("Opcao invalida, tente novamente.\n");
            }
            break;
        }

    } while (opcao != 0);

    liberar_arvore(raiz);
    return 0;
}