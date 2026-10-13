#include <stdio.h>
#include <stdlib.h>

/*
 * Arvore Rubro-Negra (Red-Black Tree)
 *
 * Propriedades:
 *  1. Todo no e vermelho ou preto.
 *  2. A raiz e preta.
 *  3. Toda folha (NIL) e preta.
 *  4. Filhos de um no vermelho sao pretos (nao existem dois vermelhos seguidos).
 *  5. Todo caminho de um no ate as folhas NIL tem o mesmo numero de nos pretos.
 *
 * Valores repetidos sao ignorados (nao sao inseridos).
 * Na remocao de no com dois filhos, usa-se o antecessor in-order
 * (maior no da subarvore esquerda).
 */

typedef enum { VERMELHO, PRETO } Cor;

typedef struct no
{
    int valor;
    Cor cor;
    struct no *esquerdo, *direito, *pai;
} No;

/* Sentinela: todas as folhas apontam para este unico no preto */
No *NIL;

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

int iniciarNIL(void)
{
    NIL = malloc(sizeof(No));

    if (NIL == NULL)
    {
        printf("\n erro ao alocar sentinela NIL!!!\n");
        return 0;
    }

    NIL->valor = 0;
    NIL->cor = PRETO;
    NIL->esquerdo = NIL;
    NIL->direito = NIL;
    NIL->pai = NIL;

    return 1;
}

No* novoNo(int x)
{
    No *novo = malloc(sizeof(No));

    if (novo)
    {
        novo->valor = x;
        novo->cor = VERMELHO; /* todo no novo entra vermelho */
        novo->esquerdo = NIL;
        novo->direito = NIL;
        novo->pai = NIL;
    }
    else
    {
        printf("\n erro ao alocar nó em novoNo!!!\n");
    }

    return novo;
}

short maior(short a, short b)
{
    return (a > b) ? a : b;
}

/* ===================== ROTACOES ===================== */

void rotacaoEsquerda(No **raiz, No *x)
{
    No *y = x->direito;

    x->direito = y->esquerdo;
    if (y->esquerdo != NIL)
    {
        y->esquerdo->pai = x;
    }

    y->pai = x->pai;

    if (x->pai == NIL)
    {
        *raiz = y;
    }
    else if (x == x->pai->esquerdo)
    {
        x->pai->esquerdo = y;
    }
    else
    {
        x->pai->direito = y;
    }

    y->esquerdo = x;
    x->pai = y;
}

void rotacaoDireita(No **raiz, No *x)
{
    No *y = x->esquerdo;

    x->esquerdo = y->direito;
    if (y->direito != NIL)
    {
        y->direito->pai = x;
    }

    y->pai = x->pai;

    if (x->pai == NIL)
    {
        *raiz = y;
    }
    else if (x == x->pai->direito)
    {
        x->pai->direito = y;
    }
    else
    {
        x->pai->esquerdo = y;
    }

    y->direito = x;
    x->pai = y;
}

/* ===================== INSERCAO ===================== */

void corrigirInsercao(No **raiz, No *z)
{
    No *tio;

    while (z->pai->cor == VERMELHO)
    {
        if (z->pai == z->pai->pai->esquerdo)
        {
            tio = z->pai->pai->direito;

            if (tio->cor == VERMELHO)
            {
                /* Caso 1: tio vermelho -> recolorir e subir */
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->direito)
                {
                    /* Caso 2: triangulo -> rotacao esquerda no pai */
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }

                /* Caso 3: linha -> recolorir e rotacao direita no avo */
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            }
        }
        else
        {
            tio = z->pai->pai->esquerdo;

            if (tio->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->esquerdo)
                {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }

                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }

    (*raiz)->cor = PRETO;
}

No* inserir(No *raiz, int x)
{
    No *z, *y = NIL, *p = raiz;

    while (p != NIL)
    {
        y = p;

        if (x < p->valor)
        {
            p = p->esquerdo;
        }
        else if (x > p->valor)
        {
            p = p->direito;
        }
        else
        {
            return raiz; /* valor repetido: ignorado */
        }
    }

    z = novoNo(x);
    if (z == NULL)
    {
        return raiz;
    }

    z->pai = y;

    if (y == NIL)
    {
        raiz = z;
    }
    else if (x < y->valor)
    {
        y->esquerdo = z;
    }
    else
    {
        y->direito = z;
    }

    corrigirInsercao(&raiz, z);

    return raiz;
}

/* ===================== BUSCA ===================== */

No* buscar(No *raiz, int num)
{
    if (raiz != NIL)
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

/* ===================== REMOCAO ===================== */

No* maximo(No *no)
{
    while (no->direito != NIL)
    {
        no = no->direito;
    }

    return no;
}

/* Substitui a subarvore com raiz em u pela subarvore com raiz em v */
void transplantar(No **raiz, No *u, No *v)
{
    if (u->pai == NIL)
    {
        *raiz = v;
    }
    else if (u == u->pai->esquerdo)
    {
        u->pai->esquerdo = v;
    }
    else
    {
        u->pai->direito = v;
    }

    v->pai = u->pai;
}

void corrigirRemocao(No **raiz, No *x)
{
    No *w;

    while (x != *raiz && x->cor == PRETO)
    {
        if (x == x->pai->esquerdo)
        {
            w = x->pai->direito;

            if (w->cor == VERMELHO)
            {
                /* Caso 1: irmao vermelho */
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->direito;
            }

            if (w->esquerdo->cor == PRETO && w->direito->cor == PRETO)
            {
                /* Caso 2: irmao preto com dois sobrinhos pretos */
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                if (w->direito->cor == PRETO)
                {
                    /* Caso 3: sobrinho distante preto, proximo vermelho */
                    w->esquerdo->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x->pai->direito;
                }

                /* Caso 4: sobrinho distante vermelho */
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direito->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        }
        else
        {
            w = x->pai->esquerdo;

            if (w->cor == VERMELHO)
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esquerdo;
            }

            if (w->direito->cor == PRETO && w->esquerdo->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                if (w->esquerdo->cor == PRETO)
                {
                    w->direito->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esquerdo;
                }

                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerdo->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }

    x->cor = PRETO;
}

No* remover(No *raiz, int chave)
{
    No *z = buscar(raiz, chave);
    No *y, *x;
    Cor corOriginal;

    if (z == NULL)
    {
        printf("Valor nao encontrado!\n");
        return raiz;
    }

    y = z;
    corOriginal = y->cor;

    if (z->esquerdo == NIL && z->direito == NIL)
    {
        printf("Elemento folha removido: %d\n", chave);
        x = NIL;
        transplantar(&raiz, z, NIL);
    }
    else if (z->esquerdo == NIL)
    {
        printf("Elemento com 1 filho removido: %d\n", chave);
        x = z->direito;
        transplantar(&raiz, z, z->direito);
    }
    else if (z->direito == NIL)
    {
        printf("Elemento com 1 filho removido: %d\n", chave);
        x = z->esquerdo;
        transplantar(&raiz, z, z->esquerdo);
    }
    else
    {
        /* Dois filhos: usa o antecessor in-order */
        y = maximo(z->esquerdo);
        corOriginal = y->cor;
        x = y->esquerdo;

        printf("Elemento com 2 filhos removido (antecessor: %d): %d\n", y->valor, chave);

        if (y->pai == z)
        {
            x->pai = y;
        }
        else
        {
            transplantar(&raiz, y, y->esquerdo);
            y->esquerdo = z->esquerdo;
            y->esquerdo->pai = y;
        }

        transplantar(&raiz, z, y);
        y->direito = z->direito;
        y->direito->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (corOriginal == PRETO)
    {
        corrigirRemocao(&raiz, x);
    }

    NIL->pai = NIL; /* limpa o ponteiro temporario usado na correcao */

    return raiz;
}

/* ===================== PERCURSOS ===================== */

void pre_ordem(No *raiz)
{
    printf("%d ", raiz->valor);
    if (raiz->esquerdo != NIL)
    {
        pre_ordem(raiz->esquerdo);
    }
    if (raiz->direito != NIL)
    {
        pre_ordem(raiz->direito);
    }
}

void em_ordem(No *raiz)
{
    if (raiz->esquerdo != NIL)
    {
        em_ordem(raiz->esquerdo);
    }
    printf("%d ", raiz->valor);
    if (raiz->direito != NIL)
    {
        em_ordem(raiz->direito);
    }
}

void pos_ordem(No *raiz)
{
    if (raiz->esquerdo != NIL)
    {
        pos_ordem(raiz->esquerdo);
    }
    if (raiz->direito != NIL)
    {
        pos_ordem(raiz->direito);
    }
    printf("%d ", raiz->valor);
}

/* ===================== ALTURA / COR / VALIDACAO ===================== */

short alturaDoNo(No *no)
{
    if (no == NIL)
    {
        return -1;
    }

    return maior(alturaDoNo(no->esquerdo), alturaDoNo(no->direito)) + 1;
}

/* Altura negra: numero de nos pretos ate uma folha NIL (sem contar o proprio no) */
short alturaNegra(No *no)
{
    short e, d;

    if (no == NIL)
    {
        return 0;
    }

    e = alturaNegra(no->esquerdo);
    d = alturaNegra(no->direito);

    return maior(e, d) + ((no->cor == PRETO) ? 1 : 0);
}

/* Retorna a altura negra se as propriedades 4 e 5 valem na subarvore, ou -1 se violadas */
int validarSubarvore(No *no)
{
    int e, d;

    if (no == NIL)
    {
        return 0;
    }

    if (no->cor == VERMELHO &&
        (no->esquerdo->cor == VERMELHO || no->direito->cor == VERMELHO))
    {
        return -1;
    }

    e = validarSubarvore(no->esquerdo);
    d = validarSubarvore(no->direito);

    if (e == -1 || d == -1 || e != d)
    {
        return -1;
    }

    return e + ((no->cor == PRETO) ? 1 : 0);
}

int arvoreValida(No *raiz)
{
    if (raiz == NIL)
    {
        return 1;
    }

    if (raiz->cor != PRETO)
    {
        return 0;
    }

    return validarSubarvore(raiz) != -1;
}

void alturaeCor(No *raiz)
{
    if (raiz == NIL)
    {
        return;
    }

    printf("No: %d | Cor: %s | Altura: %d | Altura negra: %d\n",
           raiz->valor,
           (raiz->cor == PRETO) ? "PRETO" : "VERMELHO",
           alturaDoNo(raiz),
           alturaNegra(raiz));

    alturaeCor(raiz->esquerdo);
    alturaeCor(raiz->direito);
}

/* ===================== LIBERAR MEMORIA ===================== */

void liberar_arvore(No *raiz)
{
    if (raiz == NIL)
    {
        return;
    }

    liberar_arvore(raiz->esquerdo);
    liberar_arvore(raiz->direito);

    free(raiz);
}

/* ===================== MAIN ===================== */

int main()
{
    int opcao, valor, ordem;
    No *busca, *raiz;

    if (!iniciarNIL())
    {
        return 1;
    }

    raiz = NIL;

    do
    {
        printf("\n=================>    MENU RUBRO-NEGRA    <=================\n");
        printf("Selecione uma opcao (digite apenas o valor inteiro):\n");
        printf(">(1) Inserir valor\n");
        printf(">(2) Buscar valor\n");
        printf(">(3) Remover valor\n");
        printf(">(4) Percorrer arvore\n");
        printf(">(5) Exibir altura, cor e altura negra\n");
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
            if (raiz == NIL)
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
            if (raiz == NIL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("Digite um valor que deseja remover: ");
            valor = ler_inteiro();

            raiz = remover(raiz, valor);

            break;

        case 4:
            if (raiz == NIL)
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
            if (raiz == NIL)
            {
                printf("\nA arvore esta vazia (nenhum valor inserido)!\n");
                break;
            }

            printf("\nAltura da arvore: %d\n", alturaDoNo(raiz));
            printf("Altura negra da arvore: %d\n", alturaNegra(raiz));
            printf("Cor, altura e altura negra dos nos:\n");

            alturaeCor(raiz);

            printf("\nPropriedades rubro-negras: %s\n",
                   arvoreValida(raiz) ? "VALIDAS" : "VIOLADAS");

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
    free(NIL);

    return 0;
}