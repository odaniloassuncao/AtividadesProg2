#include <stdio.h>
#include <stdlib.h>

typedef struct no
{
    int valor;
    struct no *esquerdo, *direito;
    short altura;
}No;

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
    y->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;

    return y;
}

No* rotacaoDireita(No *r){
    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;

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
            printf("\n Insercao nao realizada\nO elemento %d nao existe!!!\n", x);
        }
    }
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) +1;
    raiz = balancear(raiz);

    return raiz;
}