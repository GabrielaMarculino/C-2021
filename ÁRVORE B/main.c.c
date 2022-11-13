#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "gfx/gfx.h"

struct no {
    int qnt_chaves;
    int *chaves;
    struct no **filhos;
};

void desenha_pagina(int x, int y, int *valor, int tamanho, int qnt_chaves) {
    gfx_set_color(0, 0, 128);
    gfx_rectangle(x, y, x + 20, y+tamanho);
    char numero[20];
    for(int i = 0; i < qnt_chaves; i++) {
        numero[0]='\0';
        sprintf(numero, "%d", valor[i]);
        gfx_set_color(255, 255, 255);
        gfx_text(x + 2, y + 20 * i , numero);
    }
}

void desenha_linha(int x, int y, int tamanho, int qnt_chaves, int d, int recursao) {
    for(int i = 0; i <= qnt_chaves; i++) {
        gfx_line(x, y, x + 20,y + (tamanho + 20) * i + recursao * (tamanho + 20) * (2 * d + 1));
        gfx_line(x - 20, y + 20 * i, x, y + 20 * i);
    }
}

void desenha(struct no *arvore, int x, int y, int tamanho, int d, int recursao) {
    if (arvore==NULL){
        return;
    }
    for(int indice = 0; indice <= arvore->qnt_chaves; indice++){
        desenha(arvore->filhos[indice], x + 20 * 2, y + (tamanho + 20) * indice + recursao * (tamanho + 20) * (2 * d + 1),
              tamanho, d, indice + recursao);
    }
    desenha_pagina(x, y, &arvore->chaves[0], tamanho, arvore->qnt_chaves);
    if (arvore->filhos[0] != NULL) {
        desenha_linha(x + 20, y, tamanho, arvore->qnt_chaves, d, recursao);
    }
}

void desenha_arvore(struct no *arvore, int y, int d, int x) {
    gfx_clear();
    desenha(arvore, x, y, 2 * d * 20, d, 0);
    gfx_paint();
}

// aloca uma nova pagina
void alocar_pagina(struct no **nova_pagina, int d) {
    (*nova_pagina) = (struct no*)malloc(sizeof (struct no));
    (*nova_pagina)->chaves = (int*)malloc(sizeof (int)*(d*2)+1);
    (*nova_pagina)->filhos = (struct no**)malloc(sizeof (struct no*)*((2*d)+2));
    (*nova_pagina)->qnt_chaves = d;
    for(int i = 0; i <= d * 2; i++){
        (*nova_pagina)->filhos[i] = NULL;
    }
}

// apaga uma pagina da memoria
void apagar_pagina(struct no **ptr_pagina) {
    free((*ptr_pagina)->chaves);
    free((*ptr_pagina)->filhos);
    free((*ptr_pagina));
}

// desaloca a arvore
void desalocar(struct no **ptr) {
    if ((*ptr)->filhos[0] != NULL) {
        for (int j = 0; j < (*ptr)->qnt_chaves; ++j) {
            desalocar(&(*ptr)->filhos[j]);
        }
    }
    apagar_pagina(ptr);
}

// redistribui as chaves de um no
void redistribuicao(int d, struct no **p1, struct no **p2, int *chave) {
    int g = ((*p1)->qnt_chaves + (*p2)->qnt_chaves) / 2;

    if ((*p1)->qnt_chaves < d) {
        (*p1)->chaves[(*p1)->qnt_chaves] = *chave;
        *chave = ((*p2)->chaves[g-(*p1)->qnt_chaves - 1]);
        memcpy(&(*p1)->filhos[(*p1)->qnt_chaves + 1], (*p2)->filhos, sizeof(struct no*) * (g - (*p1)->qnt_chaves));
        memcpy(&(*p1)->chaves[(*p1)->qnt_chaves+1], (*p2)->chaves, sizeof(int) * (g - (*p1)->qnt_chaves - 1));
        memcpy((*p2)->chaves, &(*p2)->chaves[g - (*p1)->qnt_chaves], sizeof (int) * ((*p2)->qnt_chaves - g + (*p1)->qnt_chaves));
        memcpy(&(*p2)->filhos[0], &(*p2)->filhos[g], sizeof (struct no*) * ((*p2)->qnt_chaves - g + (*p1)->qnt_chaves));
        (*p2)->qnt_chaves -= g - (*p1)->qnt_chaves;
        (*p1)->qnt_chaves = g;

    } else {
        memcpy(&(*p2)->chaves[g - (*p2)->qnt_chaves], &(*p2)->chaves[0],sizeof (int) * (*p2)->qnt_chaves);
        memcpy(&(*p2)->filhos[g - (*p2)->qnt_chaves], &(*p2)->filhos[0], sizeof (struct no*) * (*p2)->qnt_chaves + 1);
        (*p2)->chaves[g - (*p2)->qnt_chaves - 1] = *chave;
        *chave = ((*p1)->chaves[(*p1)->qnt_chaves - g + (*p2)->qnt_chaves]);
        memcpy(&(*p2)->filhos[0], &(*p1)->filhos[((*p1)->qnt_chaves - g + (*p2)->qnt_chaves)], sizeof(struct no*) * ( g - (*p2)->qnt_chaves));
        memcpy(&(*p2)->chaves[0], &(*p1)->chaves[(*p1)->qnt_chaves - g + (*p2)->qnt_chaves + 1], sizeof(int) * (g - (*p2)->qnt_chaves - 1));
        (*p1)->qnt_chaves -= g - (*p2)->qnt_chaves;
        (*p2)->qnt_chaves = g;
    }
}

// trocar as chaves de um no
void troca_chaves_mesma_pagina(struct no *ptr_pagina, int i, int j){
    int chaves_aux;
    struct no *filhos_aux;

    chaves_aux = ptr_pagina->chaves[i];
    filhos_aux = ptr_pagina->filhos[i + 1];
    ptr_pagina->chaves[i] = (ptr_pagina)->chaves[j];
    ptr_pagina->filhos[i + 1] = (ptr_pagina)->filhos[j + 1];
    ptr_pagina->chaves[j] = chaves_aux;
    ptr_pagina->filhos[j + 1] = filhos_aux;
}

// ordena as chaves de um no
void ordena_chaves(struct no **ptr_pagina) {
    int j = (*ptr_pagina)->qnt_chaves - 1;
    while(j > 0) {
        if ((*ptr_pagina)->chaves[j] < (*ptr_pagina)->chaves[j - 1]){
            troca_chaves_mesma_pagina((*ptr_pagina), j, j - 1);
        }else{
            break;
        }
        --j;
    }
}

// concatena duas paginas
void concatenar(struct no **p1, struct no **p2, int chave) {
    (*p1)->chaves[(*p1)->qnt_chaves] = chave;
    memcpy(&(*p1)->chaves[(*p1)->qnt_chaves + 1], &(*p2)->chaves[0], sizeof (int) * (*p2)->qnt_chaves);
    memcpy(&(*p1)->filhos[(*p1)->qnt_chaves + 1], (*p2)->filhos, sizeof (struct no*) * ((*p2)->qnt_chaves + 1));
    (*p1)->qnt_chaves += (*p2)->qnt_chaves + 1;
}

// troca de chaves com o sucessor
int troca_chave_sucessor(int *chave, struct no **ptr_pagina, int d) {
    int verificador;

    if ((*ptr_pagina)->filhos[0] != NULL) {
        verificador = troca_chave_sucessor(chave, &(*ptr_pagina)->filhos[0], d);
    } else {
        int aux;
        aux = (*ptr_pagina)->chaves[0];
        (*ptr_pagina)->chaves[0] =* chave;
        *chave = aux;
        verificador = 0;
    }

    if (verificador == 0){
        troca_chaves_mesma_pagina((*ptr_pagina), 0, (*ptr_pagina)->qnt_chaves - 1);
        (*ptr_pagina)->qnt_chaves -= 1;
        ordena_chaves(ptr_pagina);
        if ((*ptr_pagina)->qnt_chaves < d) {
            return 2;
        } else {
            return -1;
        }

    }

    if (verificador == 2) {

        if ((*ptr_pagina)->filhos[0]->qnt_chaves+(*ptr_pagina)->filhos[1]->qnt_chaves<= 2 * d) {
            concatenar(&(*ptr_pagina)->filhos[0], &(*ptr_pagina)->filhos[1], (*ptr_pagina)->chaves[0]);
            troca_chaves_mesma_pagina((*ptr_pagina), 0, (*ptr_pagina)->qnt_chaves - 1);
            apagar_pagina(&(*ptr_pagina)->filhos[(*ptr_pagina)->qnt_chaves]);
            (*ptr_pagina)->qnt_chaves -= 1;
            ordena_chaves(ptr_pagina);
            if ((*ptr_pagina)->qnt_chaves < d) {
                return 2;
            } else {
                return -1;
            }
        } else {
            redistribuicao(d, &(*ptr_pagina)->filhos[0], &(*ptr_pagina)->filhos[1],&(*ptr_pagina)->chaves[0]);
        }
    }

    return -1;
}

// buscar um no na arvore
int busca(struct no *arvore, int valor) {
    int i, j, verificador;

    if (arvore == NULL){
        return 0;
    }

    j = arvore->qnt_chaves / 2;
    verificador = arvore->qnt_chaves;
    i = 0;
    while (i < verificador){
        if (arvore->chaves[j] == valor){
            return 1; //valor encontrado
        }
        if (arvore->chaves[j] > valor){
            verificador = j;
        }
        if (arvore->chaves[j] < valor){
            i = j + 1;
        }
        j = i + ((verificador - i) / 2);
    }
    if (busca(arvore->filhos[j], valor) == 1){
        return 1;
    } else {
        return 0;
    }
}

// funcao de comparacao do qsort
int comparador(const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

// separar uma folha em duas
void cisao(int d, struct no **arvore, int no) {
    struct no *aux;
    alocar_pagina(&aux, d);
    (*arvore)->chaves[(*arvore)->qnt_chaves - 1] = (*arvore)->filhos[no]->chaves[d];
    (*arvore)->filhos[(*arvore)->qnt_chaves] = aux;
    if ((*arvore)->filhos[0]->filhos[0] != NULL) {
        memcpy(&aux->filhos[0], &(*arvore)->filhos[no]->filhos[d + 1], sizeof(struct no*) * ( d + 1));
    }

    memcpy(&aux->chaves[0], &(*arvore)->filhos[no]->chaves[d + 1], sizeof (int) * d);
    (*arvore)->filhos[no]->qnt_chaves= d;
    aux->qnt_chaves = d;
}

// inserir um no na arvore
int insercao(struct no **arvore, int chave, int d) {
    int i, j, verificador;

    if ((*arvore) == NULL){
        return 0;
    }

    j = (*arvore)->qnt_chaves / 2;
    verificador = (*arvore)->qnt_chaves;
    i = 0;

    while (i < verificador){
        if ((*arvore)->chaves[j] == chave){
            return 1; //valor encontrado
        }
        if ((*arvore)->chaves[j] > chave){
            verificador = j;
        }
        if ((*arvore)->chaves[j] < chave){
            i = j + 1;
        }
        j = i + ((verificador - i) / 2);
    }
    verificador = insercao(&((*arvore)->filhos[j]), chave, d);
    if (verificador == 1) {
        //valor encontrado
        return 1;
    }
    if (verificador == -1) {
        //insercao concluida
        return -1;
    }
    if (verificador == 0) {
        //fazer a inserção
        (*arvore)->chaves[(*arvore)->qnt_chaves] = chave;
        (*arvore)->filhos[(*arvore)->qnt_chaves + 1] = NULL;
        (*arvore)->qnt_chaves += 1;
        qsort((*arvore)->chaves, (*arvore)->qnt_chaves, sizeof(int), comparador);

        if ((*arvore)->qnt_chaves > 2 * d) {
            //pagina cheia
            return 2;
        } else {
            //nao esta cheia ainda, somente inserir
            return -1;
        }
    }

    if (verificador == 2) {
        //aloca novo espaço para a chave que vai subir
        (*arvore)->qnt_chaves += 1;
        //faz a cisao
        cisao(d, arvore, j);
        //ordena as chaves
        ordena_chaves(arvore);
        if ((*arvore)->qnt_chaves > 2 * d){
            return 2;
        }
        return -1;
    }
    return -1;
}

int remocao(int d, struct no **arvore, int valor) {
    int i , j, verificador;

    if ((*arvore) == NULL){
        return 1;
    }

    j = (*arvore)->qnt_chaves / 2;
    verificador = (*arvore)->qnt_chaves;
    i = 0;
    //busca pelo valor na pagina
    while (i < verificador){
        if ((*arvore)->chaves[j] == valor){
            if ((*arvore)->filhos[0] != NULL){
                verificador = troca_chave_sucessor(&(*arvore)->chaves[j], &(*arvore)->filhos[j + 1], d);
            } else {
                verificador = 0;//valor encontrado
            }
            goto label;
        }
        if ((*arvore)->chaves[j] > valor){
            verificador = j;
        }
        if ((*arvore)->chaves[j] < valor){
            i = j + 1;
        }
        j = i + ((verificador - i) / 2);
    }

    verificador = remocao(d, &(*arvore)->filhos[j], valor);
    label:
    if (verificador == 1) {
        //valor nao encontrado
        return 1;
    }
    if (verificador == -1) {
        //remoção concluida
        return -1;
    }
    if (verificador == 0) {
        //fazer a remoção
        if ((*arvore)->qnt_chaves == 1){
            (*arvore)->qnt_chaves = 0;
            return -1;
        }
        if (j != ((*arvore)->qnt_chaves - 1)) {
            troca_chaves_mesma_pagina((*arvore), j, (*arvore)->qnt_chaves - 1);
        }
        (*arvore)->qnt_chaves -= 1;
        if ((*arvore)->qnt_chaves > 1) {
            ordena_chaves(arvore);
        }
        if ((*arvore)->qnt_chaves < d){
            return 2;

        } else {
            return -1;
        }
    }
    if (verificador == 2) {
        if ((*arvore)->qnt_chaves == 1){
            struct no *aux;
            alocar_pagina(&aux, d);
            memcpy(&aux->filhos[0], &(*arvore)->filhos[0]->filhos[0], sizeof (struct no*) * ((*arvore)->filhos[0]-> qnt_chaves + 1));
            memcpy(&aux->chaves[0], &(*arvore)->filhos[0]->chaves[0], sizeof (int) * (*arvore)->filhos[0]->qnt_chaves);
            aux->chaves[(*arvore)->filhos[0]->qnt_chaves] = (*arvore)->chaves[0];
            memcpy(&aux->filhos[(*arvore)->filhos[0]->qnt_chaves+1], &(*arvore)->filhos[1]->filhos[0], sizeof (struct no*) * ((*arvore)->filhos[1]->qnt_chaves + 1));
            memcpy(&aux->chaves[(*arvore)->filhos[0]->qnt_chaves+1], &(*arvore)->filhos[1]->chaves[0], sizeof (int) * (*arvore)->filhos[1]->qnt_chaves);
            aux->qnt_chaves = 2 * d;
            apagar_pagina(&(*arvore)->filhos[0]);
            apagar_pagina(&(*arvore)->filhos[1]);
            apagar_pagina(arvore);
            (*arvore) = aux;
            return -1;
        }
        if (j == 0){

            if ((*arvore)->filhos[j]->qnt_chaves + (*arvore)->filhos[j + 1]->qnt_chaves<= 2 * d){


                concatenar(&(*arvore)->filhos[j], &(*arvore)->filhos[j + 1], (*arvore)->chaves[j]);
                troca_chaves_mesma_pagina((*arvore),j,(*arvore)->qnt_chaves - 1);
                apagar_pagina(&(*arvore)->filhos[(*arvore)->qnt_chaves]);
                (*arvore)->qnt_chaves -= 1;
                ordena_chaves(arvore);
                if ((*arvore)->qnt_chaves < d){
                    return 2;
                } else {
                    return -1;
                }
            } else {
                redistribuicao(d, &(*arvore)->filhos[j], &(*arvore)->filhos[j + 1], &(*arvore)->chaves[j]);
                return -1;
            }
        }else{
            if ((*arvore)->filhos[j]->qnt_chaves+(*arvore)->filhos[j - 1]->qnt_chaves <= 2 * d){
                concatenar(&(*arvore)->filhos[j - 1], &(*arvore)->filhos[j], (*arvore)->chaves[j - 1]);
                troca_chaves_mesma_pagina((*arvore), j - 1,(*arvore)->qnt_chaves - 1);
                apagar_pagina(&(*arvore)->filhos[(*arvore)->qnt_chaves]);
                (*arvore)->qnt_chaves -= 1;
                ordena_chaves(arvore);
                if ((*arvore)->qnt_chaves < d){
                    return 2;
                } else{
                    return -1;
                }
            }else{
                redistribuicao(d, &(*arvore)->filhos[j - 1], &(*arvore)->filhos[j], &(*arvore)->chaves[j - 1]);
                return -1;
            }
        }
    }
    return -1;
}

int main() {
    gfx_init(800,600,"trabalho");
    gfx_clear();

    int d, escolha, valor, flag;
    struct no *pt;
    struct no *aux;

    do {
        printf("digite o valor de 'd':");
        scanf("%d",&d);

        if (d <= 0){
            printf("valor invalido\n");
        }

    } while (d<=0);

    alocar_pagina(&pt, 1);
    pt->qnt_chaves = 0;

    do {
        desenha_arvore(pt, 15, d, 0);

        printf("\n1-busca\n");
        printf("2-inserir\n");
        printf("3-apagar\n");
        printf("0-sair\n");
        scanf("%d",&escolha);

        switch (escolha) {
            case 1:
                printf("valor da busca: ");

                scanf("%d",&valor);
                if (busca(pt,valor)==1){
                    printf("valor encontrado\n");
                }
                break;
            case 2:
                printf("valor para inserir: ");
                scanf("%d",&valor);
                flag=insercao(&pt,valor,d);
                switch (flag) {
                    case -1:
                        printf("insercao realizada com sucesso\n");
                        break;
                    case 1:
                        printf("valor ja existe na arvore\n");
                        break;
                    case 2:
                        alocar_pagina(&aux, d);
                        aux->qnt_chaves = 1;
                        aux->filhos[0] = pt;
                        cisao(d, &aux, 0);
                        pt = aux;
                        printf("valor inserido com sucesso\n");
                        break;
                }
                break;
            case 3:
                printf("valor para apagar: ");
                scanf("%d",&valor);
                flag = remocao(d,&pt,valor);
                if (flag == -1){
                    printf("removido com sucesso\n");
                }
                if (flag == 1){
                    printf("valor nao encontrado\n");
                }
                break;
            default:
                break;
        }
    } while (escolha != 0);

    if (pt != NULL) {
        desalocar(&pt);
    }
    gfx_quit();

    return 0;
}
