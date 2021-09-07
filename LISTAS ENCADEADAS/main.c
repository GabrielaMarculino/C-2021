#include "gfx.h"
#include <stdio.h> /* printf */
#include <stdlib.h>
#include <unistd.h> /* sleep  */

/*
Feito por: Gabriela Marculino
RGM: 41431

Comando para compilar no Terminal:
gcc -o main main.c gfx.h gfx.c -lSDL -lSDL_gfx -lSDL_ttf -Wall -Wextra -pedantic  
*/

/*
    GFX não implementado :()
*/

/*
Observações:
neste programa foi utilizado caracteres especiais, pois o terminal suporta estas condições, entretanto se for executado 
em outro lugar, recomendo utilizar a função <locale.h> para haver o reconhecimento dos caracteres
*/

/********************************************************* LISTA SIMPLESMENTE ENCADEADA SEM NÓ CABEÇA ORDENADA *********************************************************/

//construindo lista simplesmente encadeada sem nó cabeça
struct listaSimples
{
    int valorRecebido;
    struct listaSimples* proximo;
};

struct listaSimples* novaListaSimples(int novoValorRecebido)
{

    struct listaSimples* novoNo = (struct listaSimples*)malloc(sizeof(struct listaSimples));

    novoNo->valorRecebido = novoValorRecebido;
    novoNo->proximo = NULL;

    return novoNo;
}

//inserindo valores na lista simplesmente encadeada sem nó cabeça encadeada
void insercaoListaSimples(struct listaSimples** ponteiroDoPonteiroListaSimples, struct listaSimples* novoNo)
{

    struct listaSimples* valorAtual;

    //se o primeiro nó for o último
    if ((*ponteiroDoPonteiroListaSimples == NULL) || ((*ponteiroDoPonteiroListaSimples)->valorRecebido >= novoNo->valorRecebido))
    {

        novoNo->proximo = *ponteiroDoPonteiroListaSimples;
        *ponteiroDoPonteiroListaSimples = novoNo;
    }
    else
    {

        //busca para inserir o nó
        valorAtual = *ponteiroDoPonteiroListaSimples;
        while ((valorAtual->proximo != NULL) && (valorAtual->proximo->valorRecebido < novoNo->valorRecebido))
        {
            valorAtual = valorAtual->proximo;
        }
        novoNo->proximo = valorAtual->proximo;
        valorAtual->proximo = novoNo;
    }
}

//buscando valores na lista simplesmente encadeada sem nó cabeça encadeada
int buscaListaSimplesEncadeada(struct listaSimples* ponteiroListaSimples, int buscaValor)
{

    if (ponteiroListaSimples == NULL)
    {
        return 0;
    }
    if (ponteiroListaSimples->valorRecebido == buscaValor)
    {
        return 1;
    }
    return (buscaListaSimplesEncadeada(ponteiroListaSimples->proximo, buscaValor)); //busca de maneira recursiva
}

//remoção de valores na lista simplesmente encadeada sem nó cabeça encadeada
void remocaoListaSimplesEncadeada(struct listaSimples **ponteiroDoPonteiroListaSimples, int buscaValor)
{

    struct listaSimples* valorTemporarioListaSimples = *ponteiroDoPonteiroListaSimples, *valorAnterior;
    
    //se o valor estiver no primeiro nó, apaga ele
    if (valorTemporarioListaSimples != NULL && valorTemporarioListaSimples->valorRecebido == buscaValor)
    {
        *ponteiroDoPonteiroListaSimples = valorTemporarioListaSimples->proximo;
        free(valorTemporarioListaSimples);
        return;
    }
    //busca para apagara o nó
    while (valorTemporarioListaSimples != NULL && valorTemporarioListaSimples->valorRecebido != buscaValor)
    {
        valorAnterior = valorTemporarioListaSimples;
        valorTemporarioListaSimples = valorTemporarioListaSimples->proximo;
    }
    if (valorTemporarioListaSimples == NULL)
    {
        printf("VALOR NAO ENCONTRADO\n");
        return;
    }
    valorAnterior->proximo = valorTemporarioListaSimples->proximo;
    free(valorTemporarioListaSimples);
}

//desalocando lista simplesmente encadeada sem nó cabeça encadeada
void desalocandoValorListaSimples(struct listaSimples **apontaVariavelDescartavel)
{

    struct listaSimples *valorDescartado;

    if (*apontaVariavelDescartavel == NULL)
    {

        printf("LISTA SIMPLESMENTE ENCADEADA DESALOCADA\n\n");
    }
    else
    {

        valorDescartado = *apontaVariavelDescartavel;
        *apontaVariavelDescartavel = valorDescartado->proximo;

        free(valorDescartado);
        desalocandoValorListaSimples(apontaVariavelDescartavel); //desalocamento de maneira recursiva
    }
}

void mostrarListaSimples(struct listaSimples* pls)
{

    struct listaSimples* valorTemporarioListaSimples = pls;

    if (valorTemporarioListaSimples == NULL)
    {

        printf("LISTA SIMPLESMENTE ENCADEADA VAZIA\n\n");

        return;
    }
    else
    {

        printf("LISTA SIMPLESMENTE ENCADEADA CRIADA:\n");

        while (valorTemporarioListaSimples != NULL)
        {

            printf("%d", valorTemporarioListaSimples->valorRecebido);
            valorTemporarioListaSimples = valorTemporarioListaSimples->proximo;
        }
        printf("\n\n");
    }
}

/********************************************************* LISTA CIRCULAR SIMPLESMENTE ENCADEADA SEM NÓ CABEÇA ORDENADA *********************************************************/

//construindo lista circular simplesmente encadeada sem nó cabeça ordenada
struct listaCircular
{
    int valorRecebido;
    struct listaCircular* proximo;
};

struct listaCircular* novaListaCircular(int novoValorRecebido)
{

    struct listaCircular* novoNo = (struct listaCircular*)malloc(sizeof(struct listaCircular));

    novoNo->valorRecebido = novoValorRecebido;
    novoNo->proximo = NULL;

    return novoNo;
}

//insercao lista circular encadeada
void insercaoListaCircular(struct listaCircular** ponteiroListaCircular, struct listaCircular* novoNo)
{

    struct listaCircular* valorAtual = *ponteiroListaCircular;

    if (valorAtual == NULL)
    {

        novoNo->proximo = novoNo;
        *ponteiroListaCircular = novoNo;
    }
    else if (valorAtual->valorRecebido >= novoNo->valorRecebido)
    {

        while (valorAtual->proximo != *ponteiroListaCircular)
        {

            valorAtual = valorAtual->proximo;
        }

        valorAtual->proximo = novoNo;
        novoNo->proximo = *ponteiroListaCircular;
        *ponteiroListaCircular = novoNo;
    }
    else
    {

        while ((valorAtual->proximo != *ponteiroListaCircular) && (valorAtual->proximo->valorRecebido < novoNo->valorRecebido))
        {
            valorAtual = valorAtual->proximo;
        }

        novoNo->proximo = valorAtual->proximo;
        valorAtual->proximo = novoNo;
    }
}

//busca lista circular encadeada
void buscaListaCircular(struct listaCircular* valorInicio, int buscaValor)
{

    struct listaCircular *valorTemporarioListaCircular;

    if (valorInicio != NULL)
    {

        valorTemporarioListaCircular = valorInicio;

        do
        {
            if (valorTemporarioListaCircular->valorRecebido == buscaValor)
            {

                printf("VALOR ENCONTRADO\n");
                return;
            }

            valorTemporarioListaCircular = valorTemporarioListaCircular->proximo;

        } while (valorTemporarioListaCircular != valorInicio);
    }
    printf("VALOR NAO ENCONTRADO\n");
}

//remocao lista circular encadeada
void remocaoListaCircular(struct listaCircular **ponteiroListaCircular, struct listaCircular *removido)
{
    if (*ponteiroListaCircular == NULL || removido == NULL)
    {
        return;
    }
    struct listaCircular *valorTemporarioListaCircular = *ponteiroListaCircular;
    if ((*ponteiroListaCircular) == removido)
    {
        (*ponteiroListaCircular) = removido->proximo;
    }
    while (valorTemporarioListaCircular->proximo != removido)
    {
        valorTemporarioListaCircular = valorTemporarioListaCircular->proximo;
    }
    valorTemporarioListaCircular->proximo = removido->proximo;
    free(removido);
    //return;
}

//busca e remoção lista circular encadeada
struct listaCircular* buscarDeletarListaCircular(struct listaCircular* valorInicio, int buscaValor)
{

    struct listaCircular *valorTemporarioListaCircular;

    if (valorInicio != NULL)
    {

        valorTemporarioListaCircular = valorInicio;

        do
        {
            if (valorTemporarioListaCircular->valorRecebido == buscaValor)
            {
                return (valorTemporarioListaCircular);
            }
            valorTemporarioListaCircular = valorTemporarioListaCircular->proximo;

        } while (valorTemporarioListaCircular != valorInicio);
    }
    printf("VALOR NAO ENCONTRADO\n");

    return NULL;
}

//mostra a lista circular encadeada
void mostrarListaCircular(struct listaCircular *valorInicio)
{

    struct listaCircular *valorTemporarioListaCircular;

    if (valorInicio == NULL)
    {

        printf("LISTA CIRCULAR SIMPLESMENTE ENCADEADA VAZIA\n\n");
    }
    else
    {

        printf("LISTA CIRCULAR SIMPLESMENTE ENCADEADA CRIADA:\n");

        valorTemporarioListaCircular = valorInicio;

        do
        {
            printf("%d", valorTemporarioListaCircular->valorRecebido);

            valorTemporarioListaCircular = valorTemporarioListaCircular->proximo;

        } while (valorTemporarioListaCircular != valorInicio);

        printf("\n\n");
    }
}

//desalocando os termos da lista circular
void desalocandoListaCircular(struct listaCircular** ponteiroListaCircular)
{

    struct listaCircular *valorAtual = *ponteiroListaCircular, *valorAnterior;

    if (valorAtual->proximo == valorAtual)
    {

        *ponteiroListaCircular = NULL;

        return;
    }
    while (valorAtual->proximo != *ponteiroListaCircular)
    {

        valorAnterior = valorAtual;
        valorAtual = valorAtual->proximo;
    }

    valorAnterior->proximo = valorAtual->proximo;

    *ponteiroListaCircular = valorAnterior->proximo;

    free(valorAtual);

    return;
}

/********************************************************* LISTA DUPLAMENTE ENCADEADA COM NÓ CABEÇA (SEM ORDENAÇÃO) *********************************************************/

//construindo lista duplamente encadeada com nó cabeça
struct listaDuplamente
{

    int valorRecebido;

    struct listaDuplamente* proximo;
    struct listaDuplamente* valorAnterior;
};

//inserindo valores na lista duplamente encadeada
void insercaoListaDuplamente(struct listaDuplamente** noCabeca, int novoValor)
{

    struct listaDuplamente* novoNo = (struct listaDuplamente*)malloc(sizeof(struct listaDuplamente));

    novoNo->valorRecebido = novoValor;
    novoNo->proximo = (*noCabeca)->proximo;
    novoNo->valorAnterior = *noCabeca;

    if ((*noCabeca)->proximo != NULL)
    {
        (*noCabeca)->proximo->valorAnterior = novoNo;
    }
    (*noCabeca)->proximo = novoNo;
}

//buscando elementos na lista duplamente encadeada
void buscaListaDuplamente(struct listaDuplamente** ponteiroListaDuplamente, int buscaValor)
{

    struct listaDuplamente* valorAtual = NULL;

    if (ponteiroListaDuplamente != NULL)
    {

        valorAtual = *ponteiroListaDuplamente;

        while (valorAtual != NULL)
        {

            if (valorAtual->valorRecebido == buscaValor)
            {

                printf("VALOR ENCONTRADO\n");

                return;
            }

            valorAtual = valorAtual->proximo;
        }
        printf("VALOR NAO ENCONTRADO\n");
        return;
    }
}

//remoção de termos da lista duplamente encadeada
void remocaoListaDuplamente(struct listaDuplamente** noCabeca, struct listaDuplamente* removido)
{

    if (*noCabeca == NULL || removido == NULL)
    {

        return;
    }

    if (*noCabeca == removido)
    {

        *noCabeca = removido->proximo;
    }
    if (removido->proximo != NULL)
    {
        removido->proximo->valorAnterior = removido->valorAnterior;
    }
    if (removido->valorAnterior != NULL)
    {
        removido->valorAnterior->proximo = removido->proximo;
    }
    free(removido);
    return;
}

//busca para a remoção dos termos da lista duplamente encadeada
struct listaDuplamente* buscarDeletarListaDuplamente(struct listaDuplamente** ponteiroListaDuplamente, int buscaValor)
{

    struct listaDuplamente *valorAtual = NULL;

    if (ponteiroListaDuplamente != NULL)
    {

        valorAtual = *ponteiroListaDuplamente;

        while (valorAtual != NULL)
        {

            if (valorAtual->valorRecebido == buscaValor)
            {

                return (valorAtual);
            }
            valorAtual = valorAtual->proximo;
        }
    }
    printf("VALOR NAO ENCONTRADO\n");
    return NULL;
}

//mostra a lista duplamente encadeada
void mostrarListaDuplamente(struct listaDuplamente* noCabeca)
{

    if (noCabeca->proximo != NULL)
    {

        struct listaDuplamente* valorTemporarioListaDuplamente = noCabeca->proximo;

        printf("LISTA DUPLAMENTE ENCADEADA CRIADA:\n");
        printf("cabeca ");

        while (valorTemporarioListaDuplamente != NULL)
        {
            printf("%d", valorTemporarioListaDuplamente->valorRecebido);
            valorTemporarioListaDuplamente = valorTemporarioListaDuplamente->proximo;
        }
        printf("\n\n");
    }
    else
    {
        printf("LISTA DUPLAMENTE ENCADEADA VAZIA\n\n");
    }
}

//desalocando a fila duplamente encadeada
void desalocaListaDuplamente(struct listaDuplamente** noCabeca, struct listaDuplamente* desalocado)
{

    if (*noCabeca == desalocado)
    {
        *noCabeca = desalocado->proximo;
    }
    if (desalocado->proximo != NULL)
    {
        desalocado->proximo->valorAnterior = desalocado->valorAnterior;
    }
    if (desalocado->valorAnterior != NULL)
    {
        desalocado->valorAnterior->proximo = desalocado->proximo;
    }
    free(desalocado);
}

/******************************************************************************* FILA (FIFO) *******************************************************************************/

//construindo fila (FIFO)
struct filaFIFO
{
    int valorRecebido;
    struct filaFIFO* proximo;
};

//inserindo elementos na fila
void inserirElementoPilhaFIFO(struct filaFIFO** ponteiroFilaFIFO, int novoValor)
{

    struct filaFIFO* novaFila = (struct filaFIFO*)malloc(sizeof(struct filaFIFO));
    struct filaFIFO *ultimoValorFila = *ponteiroFilaFIFO;

    novaFila->valorRecebido = novoValor;
    novaFila->proximo = NULL;

    if (*ponteiroFilaFIFO == NULL)
    {
        *ponteiroFilaFIFO = novaFila;
        return;
    }
    while (ultimoValorFila->proximo != NULL)
    {
        ultimoValorFila = ultimoValorFila->proximo;
    }
    ultimoValorFila->proximo = novaFila;
    return;
}

//removendo elementos da fila
void removeElementosFilaFIFO(struct filaFIFO** ponteiroFilaFIFO)
{

    if (*ponteiroFilaFIFO != NULL)
    {

        struct filaFIFO *valorTemporarioFilaFIFO = *ponteiroFilaFIFO;
        *ponteiroFilaFIFO = (*ponteiroFilaFIFO)->proximo;

        free(valorTemporarioFilaFIFO);
    }
}

//desalocacao dos elementos da fila
void desalocaFilaFIFO(struct filaFIFO** ponteiroFilaFIFO)
{

    if (*ponteiroFilaFIFO != NULL)
    {

        struct filaFIFO* valorTemporarioFilaFIFO = *ponteiroFilaFIFO;

        *ponteiroFilaFIFO = (*ponteiroFilaFIFO)->proximo;

        free(valorTemporarioFilaFIFO);
        desalocaFilaFIFO(ponteiroFilaFIFO);
    }
    else
    {

        printf("FILA DESALOCADA\n\n");
    }
}

//mostrar no console a fila
void mostrarFilaFIFO(struct filaFIFO *ponteiroFilaFIFO)
{

    if (ponteiroFilaFIFO != NULL)
    {

        struct filaFIFO *valorTemporarioFilaFIFO = ponteiroFilaFIFO;

        printf("FILA CRIADA:\n");

        while (valorTemporarioFilaFIFO != NULL)
        {

            printf("%d", valorTemporarioFilaFIFO->valorRecebido);
            valorTemporarioFilaFIFO = valorTemporarioFilaFIFO->proximo;
        }
        printf("\n\n");
    }
    else
    {
        printf("FILA VAZIA\n\n");
    }
}

/******************************************************************************* PILHA (LIFO) *******************************************************************************/

//construindo pilha LIFO
struct pilhaLIFO
{

    int valorRecebido;
    struct pilhaLIFO* proximo;
};

struct pilhaLIFO* novo(int valorRecebido)
{

    struct pilhaLIFO* no = (struct pilhaLIFO*)malloc(sizeof(struct pilhaLIFO));

    no->valorRecebido = valorRecebido;
    no->proximo = NULL;

    return no;
}

//inserção de elementos na pilha
void inserePilhaLIFO(struct pilhaLIFO** ponteiroPilhaLIFO, int novoValor)
{

    struct pilhaLIFO* no = novo(novoValor);

    no->proximo = *ponteiroPilhaLIFO;

    *ponteiroPilhaLIFO = no;
}

//remove elementos da pilha
void removeElementosPilhaLIFO(struct pilhaLIFO** ponteiroPilhaLIFO)
{

    if (*ponteiroPilhaLIFO != NULL)
    {

        struct pilhaLIFO *valorTemporarioPilhaLIFO = *ponteiroPilhaLIFO;

        *ponteiroPilhaLIFO = (*ponteiroPilhaLIFO)->proximo;

        free(valorTemporarioPilhaLIFO);
    }
}

//desaloca pilha
void desalocaPilhaLIFO(struct pilhaLIFO** ponteiroPilhaLIFO)
{

    if (*ponteiroPilhaLIFO != NULL)
    {

        struct pilhaLIFO *valorTemporarioPilhaLIFO = *ponteiroPilhaLIFO;

        *ponteiroPilhaLIFO = (*ponteiroPilhaLIFO)->proximo;

        free(valorTemporarioPilhaLIFO);
        desalocaPilhaLIFO(ponteiroPilhaLIFO);
    }
    else
    {

        printf("PILHA DESALOCADA\n\n");
    }
}

//mostra a pilha no console
void mostraPilhaLIFO(struct pilhaLIFO *ponteiroPilhaLIFO)
{

    if (ponteiroPilhaLIFO != NULL)
    {

        struct pilhaLIFO *valorTemporarioPilhaLIFO = ponteiroPilhaLIFO;

        printf("PILHA CRIADA:\n");

        while (valorTemporarioPilhaLIFO != NULL)
        {

            printf("%d\n", valorTemporarioPilhaLIFO->valorRecebido);

            valorTemporarioPilhaLIFO = valorTemporarioPilhaLIFO->proximo;
        }
        printf("\n3");
    }
    else
    {
        printf("PILHA VAZIA\n\n");
    }
}

void menu()
{

    int escolha = 0;           //menu principal
    int escolhaMenuUm = 0;     //lista simplesmente encadeada
    int escolhaMenuDois = 0;   //lista circular
    int escolhaMenuTres = 0;   //lista duplamente encadeada
    int escolhaMenuQuatro = 0; //fila
    int escolhaMenuCinco = 0;  //pilha
    int busca = 0;
    int valor = 0;

    do
    {
        printf("Feito por: Gabriela Marculino\n");
        printf("RGM: 41431\n\n");
        printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
        printf("(1) - Busca, inserção e remoção em lista simplesmente encadeada sem nó cabeça ordenada\n");
        printf("(2) - Busca, inserção e remoção em lista circular simplesmente encadeada sem nó cabeça ordenada\n");
        printf("(3) - Busca, inserção e remoção em lista duplamente encadeada com nó cabeça (sem ordenação)\n");
        printf("(4) - Inserção e remoção em fila (FIFO)\n");
        printf("(5) - Inserção e remoção em pilha (LIFO)\n");
        printf("(0) - SAIR\n\n");

        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:;
            struct listaSimples* pls = NULL;
            //Busca, inserção e remoção em lista simplesmente encadeada sem nó cabeça ordenada
            do{
            printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
            printf("(1) - Fazer BUSCA em uma lista simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(2) - Fazer INSERÇÃO em uma lista simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(3) - Fazer REMOÇÃO em lista simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(4) - MOSTRAR Lista Simplesmente Encadeada\n");
            printf("(5) - DESALOCAR Lista Simplesmente Encadeada\n");
            printf("(0) - SAIR\n\n");

            scanf("%d", &escolhaMenuUm);

            switch (escolhaMenuUm)
            {
            case 1:
                //Fazer BUSCA em uma lista simplesmente encadeada sem nó cabeça ordenada
                printf("DIGITE O VALOR A SER BUSCADO\n");
                scanf("%d", &busca);

                if (buscaListaSimplesEncadeada(pls,busca))
                {

                    printf("VALOR ENCONTRADO\n");
                }
                else
                {

                    printf("VALOR NAO ENCONTRADO\n");
                }
                mostrarListaSimples(pls);
                break;
            case 2:;
                //Fazer INSERÇÃO em uma lista simplesmente encadeada sem nó cabeça ordenada
                struct listaSimples* novoNo;

                printf("DIGITE O VALOR A SER INSERIDO\n");
                scanf("%d", &valor);

                novoNo = novaListaSimples(valor);

                insercaoListaSimples(&pls, novoNo);
                mostrarListaSimples(pls);

                break;
            case 3:
                //Fazer REMOÇÃO em lista simplesmente encadeada sem nó cabeça ordenada
                printf("DIGITE O VALOR A SER EXCLUIDO\n");
                scanf("%d", &busca);

                remocaoListaSimplesEncadeada(&pls, busca);
                mostrarListaSimples(pls);

                break;
            case 4:
                //MOSTRAR Lista Simplesmente Encadeada
                mostrarListaSimples(pls);
                break;
            case 5:
                //Desalocar lista simplesmente encadeada
                desalocandoValorListaSimples(&pls);
                break;
            case 0:
                //Sair menu um
                break;
            default:
                printf("Opção inválida\n");
                break;
            }
            }while(escolhaMenuUm != 0);
            break;

        case 2:;
            //Busca, inserção e remoção em lista circular simplesmente encadeada sem nó cabeça ordenada
            struct listaCircular *plc = NULL;
            
            do{
            printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
            printf("(1) - Fazer BUSCA em lista circular simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(2) - Fazer INSERÇÃO em lista circular simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(3) - Fazer REMOÇÃO em lista circular simplesmente encadeada sem nó cabeça ordenada\n");
            printf("(4) - MOSTRAR Lista Circular\n");
            printf("(5) - DESALOCAR Lista Circular\n");
            printf("(0) - SAIR\n\n");

            scanf("%d", &escolhaMenuDois);

            switch (escolhaMenuDois)
            {
            case 1:
                //Fazer BUSCA em lista circular simplesmente encadeada sem nó cabeça ordenada
                printf("DIGITE O VALOR A SER BUSCADO\n");
                scanf("%d", &busca);

                buscaListaCircular(plc, busca);
                mostrarListaCircular(plc);

                break;
            case 2:;
                //Fazer INSERÇÃO em lista circular simplesmente encadeada sem nó cabeça ordenada
                struct listaCircular *novoNo;
                printf("DIGITE O VALOR A SER INSERIDO\n");
                scanf("%d", &valor);

                novoNo = novaListaCircular(valor);

                insercaoListaCircular(&plc, novoNo);
                mostrarListaCircular(plc);

                break;
            case 3:
                //Fazer REMOÇÃO em lista circular simplesmente encadeada sem nó cabeça ordenada
                printf("DIGITE O VALOR A SER EXCLUIDO\n");
                scanf("%d", &busca);

                remocaoListaCircular(&plc, buscarDeletarListaCircular(plc, busca));
                mostrarListaCircular(plc);

                break;
            case 4:
                //MOSTRAR Lista Circular
                mostrarListaCircular(plc);
                break;
            case 5:
                //DESALOCAR Lista Circular
                if (plc == NULL)
                {
                    printf("LISTA CIRCULAR SIMPLESMENTE ENCADEADA DESALOCADA\n\n");
                }
                while (plc != NULL)
                {
                    desalocandoListaCircular(&plc);

                    if (plc == NULL)
                    {
                        printf("LISTA CIRCULAR SIMPLESMENTE ENCADEADA DESALOCADA\n\n");
                    }
                }
                break;
            case 0:
                //Sair menu dois
                break;
            default:
                printf("Opção inválida\n");
                break;
            }
            }while(escolhaMenuDois != 0);
            break;
        case 3:;
            //Busca, inserção e remoção em lista duplamente encadeada com nó cabeça (sem ordenação)
            struct listaDuplamente *noCabeca = (struct listaDuplamente*)malloc(sizeof(struct listaDuplamente));
            noCabeca->valorRecebido = 0;
            noCabeca->valorAnterior = NULL;
            noCabeca->proximo = NULL;

            do{
            printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
            printf("(1) - Fazer BUSCA em lista duplamente encadeada com nó cabeça (sem ordenação)\n");
            printf("(2) - Fazer INSERÇÃO em lista duplamente encadeada com nó cabeça (sem ordenação)\n");
            printf("(3) - Fazer REMOÇÃO em lista duplamente encadeada com nó cabeça (sem ordenação)\n");
            printf("(4) - MOSTRAR Lista Duplamente Encadeada\n");
            printf("(5) - DESALOCAR Lista Duplamente Encadeada\n");
            printf("(0) - SAIR\n\n");

            scanf("%d", &escolhaMenuTres);

            switch (escolhaMenuTres)
            {

            case 1:
                //Fazer BUSCA em lista duplamente encadeada com nó cabeça (sem ordenação)
                printf("DIGITE O VALOR A SER BUSCADO\n");
                scanf("%d", &busca);

                buscaListaDuplamente(&noCabeca, busca);
                mostrarListaDuplamente(noCabeca);

                break;
            case 2:
                //Fazer INSERÇÃO em lista duplamente encadeada com nó cabeça (sem ordenação)
                printf("DIGITE O VALOR A SER INSERIDO\n");
                scanf("%d", &valor);

                insercaoListaDuplamente(&noCabeca, valor);
                mostrarListaDuplamente(noCabeca);

                break;
            case 3:
                //Fazer REMOÇÃO em lista duplamente encadeada com nó cabeça (sem ordenação)
                printf("DIGITE O VALOR A SER REMOVIDO\n");
                scanf("%d", &busca);

                remocaoListaDuplamente(&noCabeca, buscarDeletarListaDuplamente(&noCabeca, busca));
                mostrarListaDuplamente(noCabeca);

                break;
            case 4:
                //MOSTRAR Lista Duplamente Encadeada
                mostrarListaDuplamente(noCabeca);

                break;
            case 5:
                //DESALOCAR Lista Duplamente Encadeada
                do
                {
                    desalocaListaDuplamente(&noCabeca,noCabeca);
                    if (noCabeca == NULL)
                    {
                        printf("LISTA DUPLAMENTE ENCADEADA DESALOCADA\n\n");
                    }
                } while (noCabeca != NULL);
                break;

            case 0:
                //Sair menu tres
                break;

            default:
                printf("Opção inválida\n");
                break;
            }
            }while(escolhaMenuTres != 0);
            break;
        case 4:;
            //Inserção e remoção em fila (FIFO)
            struct filaFIFO* ponteiroFilaFIFO = NULL;
            do{
            printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
            printf("(1) - Fazer INSERÇÃO em fila (FIFO)\n");
            printf("(2) - Fazer REMOÇÃO em fila (FIFO)\n");
            printf("(3) - MOSTRAR Fila FIFO\n");
            printf("(4) - DESALOCAR Fila FIFO\n");
            printf("(0) - SAIR\n\n");

            scanf("%d", &escolhaMenuQuatro);

            switch (escolhaMenuQuatro)
            {
            case 1:
                //Fazer INSERÇÃO em fila (FIFO)
                printf("DIGITE O VALOR A SER INSERIDO\n");
                scanf("%d", &valor);

                inserirElementoPilhaFIFO(&ponteiroFilaFIFO, valor);
                mostrarFilaFIFO(ponteiroFilaFIFO);

                break;
            case 2:
                //Fazer REMOÇÃO em fila (FIFO)
                removeElementosFilaFIFO(&ponteiroFilaFIFO);
                mostrarFilaFIFO(ponteiroFilaFIFO);

                break;
            case 3:
                //MOSTRAR Fila FIFO
                mostrarFilaFIFO(ponteiroFilaFIFO);
                break;
            case 4:
                //DESALOCAR Fila FIFO
                desalocaFilaFIFO(&ponteiroFilaFIFO);
                break;
            case 0:
                //Sair menu quatro
                break;

            default:
                printf("Opção inválida\n");
                break;
            }
            }while(escolhaMenuQuatro != 0);

            break;
        case 5:;
            //Inserção e remoção em pilha (LIFO)
            struct pilhaLIFO* ponteiroPilhaLIFO = NULL;

            do{
            printf("DIGITE O NÚMERO DE ACORDO COM A SUA ESCOLHA DAS OPÇÕES ABAIXO: \n\n");
            printf("(1) - Fazer INSERÇÃO em pilha (LIFO)\n");
            printf("(2) - Fazer REMOÇÃO em pilha (LIFO)\n");
            printf("(3) - MOSTRAR Pilha LIFO\n");
            printf("(4) - DESALOCAR Pilha LIFO\n");
            printf("(0) - SAIR\n\n");

            scanf("%d", &escolhaMenuCinco);

            switch (escolhaMenuCinco)
            {
            case 1:
                //Fazer INSERÇÃO em pilha (LIFO)
                printf("DIGITE O VALOR A SER INSERIDO\n");
                scanf("%d", &valor);

                inserePilhaLIFO(&ponteiroPilhaLIFO, valor);
                mostraPilhaLIFO(ponteiroPilhaLIFO);

                break;
            case 2:
                //Fazer REMOÇÃO em pilha (LIFO)
                removeElementosPilhaLIFO(&ponteiroPilhaLIFO);
                mostraPilhaLIFO(ponteiroPilhaLIFO);
                break;
            case 3:
                //MOSTRAR Pilha LIFO
                mostraPilhaLIFO(ponteiroPilhaLIFO);
                break;
            case 4:
                //DESALOCAR Pilha LIFO
                desalocaPilhaLIFO(&ponteiroPilhaLIFO);
                break;
            case 0:
                //Sair menu cinco
                break;

            default:
                printf("Opção inválida\n");
                break;
            }
            }while(escolhaMenuCinco != 0);
            break;
             case 0:
            //Sair menu principal
            break;

            default:
            printf("Opção inválida!\n");
            break;
            }
        
        
    }while (escolha != 0);
}

int main()
{
    menu();
    return 0;
}
