#include <stdio.h>
#include <stdlib.h>


typedef struct no
{
    int valor;
    struct no *esquerdo, *direito;
    short altura;
}No;

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

No* novoNo(int x){
    No *novo = malloc(sizeof(No));

    if (novo)
    {
        novo->valor = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else{
        printf("\n erro ao alocar nó em novoNo!!!\n");
    }
    return novo;
}

short maior(short a, short b){
    return (a > b)? a:b;
}

short alturaDoNo(No *no){
    if (no == NULL)
    {
        return -1;
    }
    else{
        return no->altura;
    }
}

short fatorDeBalanceamento(No *no){
    if (no)
    {
        return (alturaDoNo(no->esquerdo) - alturaDoNo(no->direito));
    }
    else{
        return 0;
    }
}

No* rotacaoEsquerda(No *r){
    No *y, *f;

    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireita(No *r){
    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireitaEsquerda(No *r){
    r->direito = rotacaoDireita(r->direito);
    return rotacaoEsquerda(r);
}

No* rotacaoEsquerdaDireita(No *r){
    r->esquerdo = rotacaoEsquerda(r->esquerdo);
    return rotacaoDireita(r);
}

No* balancear(No *raiz){
short fb = fatorDeBalanceamento(raiz);

if (fb < -1 && fatorDeBalanceamento(raiz->direito) <= 0)
{
    raiz = rotacaoEsquerda(raiz);
}

else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0)
{
    raiz = rotacaoDireita(raiz);
}

else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0)
{
    raiz = rotacaoEsquerdaDireita(raiz);
}

else if (fb < -1 && fatorDeBalanceamento(raiz->direito) > 0)
{
    raiz = rotacaoDireitaEsquerda(raiz);
}

return raiz;
}

No* inserir(No *raiz, int x){
    if(raiz == NULL){
        return novoNo(x);
    }
    else{
        if(x < raiz->valor){
            raiz->esquerdo = inserir(raiz->esquerdo, x);
        }
        else if(x > raiz->valor){
            raiz->direito = inserir(raiz->direito, x);
        }
        else{
            return raiz;
        }
    }
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) +1;
    raiz = balancear(raiz);
    
    return raiz;
}

No* buscar(No *raiz, int num)
{
    if (raiz != NULL)
    {
        if (num == raiz->valor)
        {
            return raiz;
        }
        else if (num < raiz->valor)
        {
            return buscar(raiz->esquerdo, num);
        }
        else
        {
            return buscar(raiz->direito, num);
        }
    }

    return NULL;
}

No* remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("Valor nao encontrado!\n");
        return NULL;
    } else { 
        if(raiz->valor == chave) {
            if(raiz->esquerdo == NULL && raiz->direito == NULL) {
                free(raiz);
                printf("Elemento folha removido: %d\n", chave);
                return NULL;
            }
            else {
                if(raiz->esquerdo != NULL && raiz->direito != NULL) {
                    No *aux = raiz->esquerdo;

                    while(aux->direito != NULL)
                        aux = aux->direito;

                    raiz->valor = aux->valor;
                    aux->valor = chave;

                    printf("Elemento trocado: %d\n", chave);

                    raiz->esquerdo = remover(raiz->esquerdo, chave);
                }
                else {
                    No *aux;

                    if(raiz->esquerdo != NULL)
                        aux = raiz->esquerdo;
                    else
                        aux = raiz->direito;

                    free(raiz);

                    printf("Elemento com 1 filho removido: %d\n", chave);
                    return aux;
                }
            }
        } else {
            if(chave < raiz->valor)
                raiz->esquerdo = remover(raiz->esquerdo, chave);
            else
                raiz->direito = remover(raiz->direito, chave);
        }

        raiz->altura = maior(alturaDoNo(raiz->esquerdo),
                             alturaDoNo(raiz->direito)) + 1;

        raiz = balancear(raiz);

        return raiz;
    }
}

void pre_ordem(No *raiz)
{
    printf("%d ", raiz->valor);
    if (raiz->esquerdo != NULL)
    {
        pre_ordem(raiz->esquerdo);
    }
    if (raiz->direito != NULL)
    {
        pre_ordem(raiz->direito);
    }
}

void em_ordem(No *raiz)
{
    if (raiz->esquerdo != NULL)
    {
        em_ordem(raiz->esquerdo);
    }
    printf("%d ", raiz->valor);
    if (raiz->direito != NULL)
    {
        em_ordem(raiz->direito);
    }
}

void pos_ordem(No *raiz)
{
    if (raiz->esquerdo != NULL)
    {
        pos_ordem(raiz->esquerdo);
    }
    if (raiz->direito != NULL)
    {
        pos_ordem(raiz->direito);
    }
    printf("%d ", raiz->valor);
}


void alturaeFB(No *raiz){
    if (raiz == NULL)
    {
        return;
    }

    printf("No: %d | Altura: %d | FB: %d\n",
           raiz->valor,
           raiz->altura,
           fatorDeBalanceamento(raiz));

    alturaeFB(raiz->esquerdo);
    alturaeFB(raiz->direito);
}

void liberar_arvore(No *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    liberar_arvore(raiz->esquerdo);
    liberar_arvore(raiz->direito);

    free(raiz);
}

int main()
{
    int opcao, valor, ordem;
    No *busca, *raiz = NULL;

    do
    {
        printf("\n=======================>    MENU AVL    <=======================\n");
        printf("Selecione uma opcao (digite apenas o valor inteiro):\n");
        printf(">(1) Inserir valor\n");
        printf(">(2) Buscar valor\n");
        printf(">(3) Remover valor\n");
        printf(">(4) Percorrer arvore\n");
        printf(">(5) Exibir altura e fator de balanceamento\n");
        printf(">(0) Sair\n");
        printf("Opcao: ");

        opcao = ler_inteiro();

        switch (opcao)
        {
        case 1:
            printf("Digite um numero que deseja inserir: ");
            valor = ler_inteiro();

            if (buscar(raiz, valor) != NULL)
            {
                printf("\nInsercao nao realizada.\n");
                printf("O elemento %d ja existe!\n", valor);
            }
            else
            {
                raiz = inserir(raiz, valor);
                printf("Numero inserido com sucesso!\n");
            }

            break;

        case 2:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("Digite um numero que deseja buscar: ");
            valor = ler_inteiro();

            busca = buscar(raiz, valor);

            if (busca != NULL)
            {
                printf("\nNumero encontrado: %d\n", busca->valor);
            }
            else
            {
                printf("Numero nao encontrado.\n");
            }

            break;

        case 3:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("Digite um valor que deseja remover: ");
            valor = ler_inteiro();

            raiz = remover(raiz, valor);

            break;

        case 4:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("\nEscolha um percurso:\n");
            printf("(1) Pre-ordem\n");
            printf("(2) Em ordem\n");
            printf("(3) Pos-ordem\n");
            printf("Opcao: ");

            ordem = ler_inteiro();

            switch (ordem)
            {
            case 1:
                pre_ordem(raiz);
                printf("\n");
                break;

            case 2:
                em_ordem(raiz);
                printf("\n");
                break;

            case 3:
                pos_ordem(raiz);
                printf("\n");
                break;

            default:
                printf("Opcao invalida, tente novamente.\n");
                break;
            }

            break;

        case 5:
            if (raiz == NULL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("\nAltura da arvore: %d\n", raiz->altura);
            printf("Fator de balanceamento dos nos:\n");

            alturaeFB(raiz);

            break;

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