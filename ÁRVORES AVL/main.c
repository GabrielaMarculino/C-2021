#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "gfx.h"

/*
Feito por: Gabriela Marculino

Comando para compilar no Terminal:
gcc -o main main.c gfx.h gfx.c -lSDL -lSDL_gfx -lSDL_ttf -Wall -Wextra -pedantic

OBS: este código foi feito no Linux, logo a acentuação funciona no terminal. Entretanto se for rodar este código no Windows
utilize o <locale.h> para que ele reconheça os caracteres especiais.
*/

//construindo estrutura
struct s_no{

    int32_t chave: 28;
    int32_t bal: 2;
    int32_t reservado: 2;

    struct s_no *dir;
    struct s_no *esq;

};

struct s_arq_no{

    int32_t chave: 28;
    int32_t bal: 2;
    uint32_t esq: 1;
    uint32_t dir: 1;
};

//alocando o nó
void novoNo(struct s_no **ponto, int32_t valor){

    (*ponto) = (struct s_no*) malloc(sizeof(struct s_no));
    (*ponto) -> esq = NULL, (*ponto) -> dir = NULL;
    (*ponto) -> chave = valor, (*ponto) -> bal = 0, (*ponto) -> reservado = 0;

}

//busca
void busca(struct s_no* raizArvore, int chave){

    if(raizArvore -> chave == chave){

        printf("ELEMENTO %d ENCONTRADO \n\n",chave);
        return;
    }

    if(raizArvore -> chave < chave){

        if(raizArvore -> dir != NULL){

            busca(raizArvore, chave);
            return;

        }else{

            printf("ELEMENTO %d NÃO ENCONTRADO \n\n",chave);
            return;
        }
    }
    if(raizArvore -> esq != NULL){

        busca(raizArvore -> esq, chave);
        return;

    }else{

        printf("ELEMENTO %d NÃO ENCONTRADO \n", chave);
        return;
    }
}

//mostra a arvore no utilizando GFX
void printArvore(struct s_no *raizArvore, unsigned short xi, unsigned short xf, unsigned short y){

    char string[64];
    unsigned short xm;

    if (raizArvore == NULL){

        return;
    }

    gfx_set_color(255,255,255);
    xm = (xi+xf) / 2;

    snprintf(string,64,"%i",raizArvore -> chave);
    gfx_text(xm, y, string);
    gfx_set_color(0,255,0);
    snprintf(string,64,"%i",raizArvore -> bal);
    gfx_text(xm + 10, y - 30, string);

    if(raizArvore -> esq != NULL){

        gfx_set_color(0,0,255);
        gfx_line(xm, y, (xi + xm) / 2, y + 70);
    }

    if(raizArvore->dir != NULL){

        gfx_set_color(0,0,255);
        gfx_line(xm, y, (xf+xm)/2, y+70);
    }

    //funções recursivas para atualizar a árvore
    printArvore(raizArvore->esq, xi, xm, y + 70);
    printArvore(raizArvore->dir, xm, xf, y + 70);
}

//com base nos algoritmos do slide, fazer o rebalanceamento (CASO 1)
void rebalanceamentoUm(struct s_no** ponto, bool* altura){

    struct s_no* pontoUm = (*ponto) -> esq;

    if(pontoUm -> bal == -1){

        (*ponto) -> esq = pontoUm -> dir;
        pontoUm -> dir = (*ponto);
        (*ponto) -> bal = 0;
        (*ponto) = pontoUm;

    }else{

        struct s_no* pontoV = pontoUm -> dir;

        pontoUm -> dir = pontoV -> esq;
        pontoV -> esq = pontoUm;
        (*ponto) -> esq = pontoV -> dir;
        pontoV -> dir = (*ponto);

        if(pontoV -> bal == -1) (*ponto) -> bal = 1;
        else (*ponto) -> bal = 0;

        if(pontoV -> bal == 1) pontoUm -> bal = -1;
        else pontoUm -> bal = 0;
        (*ponto) = pontoV;
    }

    (*ponto) -> bal = 0;
    *altura = false;
}

//com base nos algoritmos do slide, fazer o rebalanceamento (CASO 2)
void rebalanceamentoDois(struct s_no** ponto, bool *altura){

    struct s_no* pontoUm = (*ponto) -> dir;

    if(pontoUm -> bal == 1){

        (*ponto) -> dir = pontoUm -> esq;
        pontoUm -> esq = (*ponto);
        (*ponto) -> bal = 0;
        (*ponto) = pontoUm;

    }else{

        struct s_no* pontoV = pontoUm -> esq;

        pontoUm -> esq = pontoV -> dir;
        pontoV -> dir = pontoUm;
        (*ponto) -> dir = pontoV -> esq;
        pontoV -> esq = (*ponto);

        if(pontoV -> bal == 1) (*ponto) -> bal = -1;
        else (*ponto) -> bal = 0;

        if(pontoV -> bal == -1) pontoUm -> bal = 1;
        else pontoUm -> bal = 0;
        (*ponto) = pontoV;
    }

    (*ponto) -> bal = 0;
    *altura = false;
}

//com base nos algoritmos do slide, fazer a INSERÇÃO
void insercao(int32_t valor, struct s_no** ponto, bool* altura){

    if(*ponto == NULL){

        novoNo(&(*ponto), valor);
        *altura = true;

    }else if(valor == (*ponto) -> chave){

        printf("A CHAVE JÁ EXISTE \n");
        return;

    }else{
        if(valor < (*ponto) -> chave){

            insercao(valor, &(*ponto) -> esq, altura);

            if(*altura){

                switch((*ponto) -> bal){

                    case 1:;

                        (*ponto) -> bal = -1;
                        *altura = false;

                        break;

                    case 0:;

                        (*ponto) -> bal = -1;

                        break;

                    case -1:;
                        rebalanceamentoUm(&(*ponto), altura);
                        break;
                }
            }
        }else{

            insercao(valor, &(*ponto) -> dir, altura);

            if(*altura){

                switch((*ponto) -> bal){

                    case -1:;

                        (*ponto) -> bal = 0;
                        *altura = false;

                        break;

                    case 0:;

                        (*ponto) -> bal = 1;

                        break;

                    case 1:;

                        rebalanceamentoDois(&(*ponto), altura);

                        break;
                }
            }
        }
    }
}

//com base nos algoritmos do slide, fazer o rebalanceamento (CASO 1)
void rebalanceamentoCasoUm(struct s_no** ponto, bool* altura){

    struct s_no* pontoUm = (*ponto) -> esq;

    if(pontoUm -> bal < 1){

        (*ponto) -> esq = pontoUm -> dir;
        pontoUm -> dir = (*ponto);

        if(pontoUm -> bal == 0){

            pontoUm -> bal = 1;
            *altura = false;

        }else{

            pontoUm -> bal = 0;
        }

        (*ponto) = pontoUm;

    }else{

        struct s_no* pontoV = pontoUm -> dir;

        pontoUm -> dir = pontoV -> esq;
        pontoV -> esq = pontoUm;
        (*ponto) -> esq = pontoV -> dir;
        pontoV -> dir = (*ponto);

        if(pontoV -> bal == -1){

            (*ponto) -> bal = 1;

        }else{

            (*ponto) -> bal = 0;
        }

        if(pontoV -> bal == 1){

            pontoUm -> bal = -1;

        }else{

            pontoUm -> bal = 0;
        }

        (*ponto) = pontoV;
        pontoV -> bal = 0;
    }
}

//com base nos algoritmos do slide, fazer o rebalanceamento (CASO 2)
void rebalancemanentoCasoDois(struct s_no** ponto, bool *altura){

    struct s_no* pontoUm = (*ponto) -> dir;

    if(pontoUm -> bal > -1){

        (*ponto) -> dir = pontoUm -> esq;
        pontoUm -> esq = (*ponto);

        if(pontoUm -> bal == 0){

            pontoUm -> bal = -1;
            *altura = false;

        }else{

            pontoUm -> bal = 0;
        }

        (*ponto) -> bal = 0;
        (*ponto) = pontoUm;

    }else{

        struct s_no* pontoV = pontoUm -> esq;

        pontoUm -> esq = pontoV -> dir;
        pontoV -> dir = pontoUm;
        (*ponto) -> dir = pontoV -> esq;
        pontoV -> esq = (*ponto);

        if(pontoV -> bal == 1){

            (*ponto) -> bal = -1;

        }else{

            (*ponto) -> bal = 0;
        }

        if(pontoV -> bal == -1){

            pontoUm -> bal = 1;

        }else{

            pontoUm -> bal = 0;
        }

        (*ponto) = pontoV;
        pontoV -> bal = 0;
    }
}

//função auxiliar para realizar a remoção dos elementos da árvore binária
void auxiliarRemocao(struct s_no **ponto, struct s_no **valorTemporario, bool *altura){

    if((*valorTemporario) -> esq == NULL){

        (*ponto) -> chave = (*valorTemporario) -> chave;
        *ponto = *valorTemporario;
        *valorTemporario = (*valorTemporario) -> dir;
        *altura = true;

    }else{

        auxiliarRemocao(ponto, &(*valorTemporario) -> esq, altura);

        if(*altura){

            switch((*valorTemporario) -> bal){

                case -1:;

                    (*valorTemporario) -> bal = 0;

                    break;

                case 0:;

                    (*valorTemporario) -> bal = 1;
                    *altura = false;

                    break;

                case 1:;

                    rebalancemanentoCasoDois(valorTemporario, altura);

                    break;
            }
        }
    }
}

//remover elemento árvore binária, de acordo com os slides apresentados
void removerElemento(struct s_no** ponto, int32_t valor, bool *altura){

    struct s_no *valorTemporario = NULL;

    if((*ponto) != NULL){

        if(valor == (*ponto) -> chave){

            valorTemporario = *ponto;

            if(((*ponto) -> esq != NULL) && ((*ponto) -> dir != NULL)){
                auxiliarRemocao(&valorTemporario, &valorTemporario -> dir, altura);

                if(*altura){

                    switch ((*ponto) -> bal){

                        case 1:;

                            (*ponto) -> bal = 0;

                            break;

                        case 0:;

                            (*ponto) -> bal = -1;
                            *altura = false;

                            break;

                        case -1:;

                            rebalanceamentoCasoUm(ponto, altura);

                            break;
                    }
                }
            }else{

                if(valorTemporario -> esq == NULL){

                    *ponto = valorTemporario -> esq;
                    *altura = true;
                }
            }
            free(valorTemporario);

        }else{

            if(valor < (*ponto) -> chave){

                removerElemento(&((*ponto) -> esq), valor, altura);

                if(*altura){

                    switch((*ponto) -> bal){

                        case 1:;

                            rebalanceamentoDois(ponto, altura);

                            break;

                        case 0:;

                            (*ponto) -> bal = 1;
                            *altura = false;

                            break;

                        case -1:;

                            (*ponto) -> bal = 0;

                            break;
                    }
                }

            }else{

                removerElemento(&((*ponto) -> dir), valor, altura);

                if(*altura){

                    switch((*ponto) -> bal){

                        case 1:;

                            (*ponto) -> bal =0;

                            break;

                        case 0:;

                            (*ponto) -> bal = -1;
                            *altura = false;

                            break;

                        case -1:;

                            rebalanceamentoCasoUm(ponto, altura);

                            break;
                    }
                }
            }
        }
    }
}

void escreveArquivo(struct s_no* raizArvore, struct s_arq_no *arquivo, FILE* fp_bin){

    if(raizArvore == NULL){

        return;
    }

    arquivo = (struct s_arq_no *)malloc(sizeof(struct s_arq_no));
    arquivo -> chave = raizArvore -> chave;
    arquivo -> bal = raizArvore -> bal;

    if(raizArvore -> esq == NULL){

        arquivo -> esq = 0;

    }else{

        arquivo -> esq = 1;
    }
    if(raizArvore -> dir == NULL){

        arquivo -> dir = 0;

    }else{

        arquivo -> dir = 1;
    }

    fwrite(arquivo, sizeof(struct s_arq_no), 1, fp_bin);
    escreveArquivo(raizArvore -> esq, arquivo, fp_bin);
    escreveArquivo(raizArvore -> dir, arquivo, fp_bin);
}

struct s_no* leArquivo(struct s_no* no, FILE* fl_bin){

    struct s_arq_no arquivo;

    fread(&arquivo, sizeof(arquivo), 1, fl_bin);

    if(!feof(fl_bin)){

        no = (struct s_no*)malloc(sizeof(struct s_no));
        no -> chave = arquivo.chave;
        no -> bal = arquivo.bal;
        no -> reservado = 0;

        if(arquivo.esq == 1){

            no -> esq = leArquivo(no -> esq, fl_bin);

        }else{

            no -> esq = NULL;
        }
        if(arquivo.dir == 1){

            no->dir = leArquivo(no -> dir, fl_bin);

        }else{

            no->dir = NULL;
        }
    }
    return no;
}

//desalocar árvore binária
void desalocaArvore(struct s_no* raizArvore){

    if(raizArvore == NULL){

        return;
    }

    desalocaArvore(raizArvore -> esq);
    desalocaArvore(raizArvore -> dir);

    free(raizArvore);
}

void menu(){

    int escolha = 0;
    int buscaArvore = 0;
    int novoValor = 0;
    unsigned long tamanhoArquivo;
    char nome[25];

    struct s_no *raizArvore = NULL;
    struct s_arq_no *arquivo = NULL;

    FILE* fp_bin;
    FILE* fl_bin;

    do{

        bool altura = false;

        printf("Feito por: Gabriela Marculino\n");
        printf("RGM: 41431\n\n");

        printf("DIGITE ABAIXO O NÚMERO QUE CORRESPONDE A OPÇÃO QUE VOCÊ DESEJA: \n\n");
        printf("1- BUSCA \n");
        printf("2- INSERCAO \n");
        printf("3- REMOCAO \n");
        printf("4- GRAVAÇÃO EM ARQUIVO BINÁRIO \n");
        printf("5- LER ARQUIVO BINÁRIO \n");
        printf("0- SAIR E DESALOCAR \n\n");

        scanf("%d",&escolha);

        switch(escolha){

            case 1:;

                if(raizArvore != NULL){

                    printf("DIGITE O VALOR DO ELEMENTO A SER BUSCADO: \n");
                    scanf("%d", &buscaArvore);

                    busca(raizArvore, buscaArvore);

                }else{

                    printf("ÁRVORE VAZIA. \n\n\n");
                }
                break;

            case 2:;

                altura = 0;

                printf("DIGITE O VALOR DO ELEMENTO A SER INSERIDO: \n");
                scanf("%d", &novoValor);

                insercao(novoValor, &raizArvore, &altura);

                printf("ELEMENTO '%d' INSERIDO OU JÁ PRESENTE NA ÁRVORE. \n\n", novoValor);

                gfx_init(1366, 768, "ÁRVORE");
                gfx_set_font_size(30);
                printArvore(raizArvore, 0, 1366, 70);
                gfx_paint();

                break;

            case 3:;

                if (raizArvore == NULL){

                    printf("ÁRVORE VAZIA \n\n");

                }else{

                    printf("DIGITE O VALOR DO ELEMENTO A SER REMOVIDO:\n");
                    scanf("%d", &buscaArvore);

                    removerElemento(&raizArvore, buscaArvore, &altura);
                    printf("ELEMENTO REMOVIDO OU NAO ENCONTRADO\n\n");
                }

                //mostra a árvore no gfx
                gfx_init(1366, 768, "ÁRVORE");
                gfx_set_font_size(30);
                printArvore(raizArvore, 0, 1366, 70);
                gfx_paint();

                break;

            case 4:;

                if (raizArvore == NULL){

                    printf("ÁRVORE VAZIA \n\n");

                }else{

                    printf("DIGITE O NOME QUE O ARQUIVO BINÁRIO (COM A EXTENSÃO): \n\n");
                    scanf("%s", nome);

                    fp_bin = fopen(nome, "wb");
                    escreveArquivo(raizArvore, arquivo, fp_bin);

                    printf("\nARVORE ESCRITA EM ARQUIVO\n\n");
                    fclose(fp_bin);
                }
                break;

            case 5:;

                desalocaArvore(raizArvore);

                printf("DIGITE O NOME DO ÁRQUIVO (COM A EXTENSAO): \n\n");
                scanf("%s", nome);

                fl_bin = fopen(nome, "rb");

                if (fl_bin == NULL){

                    printf("ÁRQUIVO NÃO ENCONTRADO \n\n");

                }else{

                    raizArvore = (struct s_no*)malloc(sizeof(struct s_no));
                    fseek (fl_bin , 0 , SEEK_END);
                    tamanhoArquivo = ftell (fl_bin);
                    rewind (fl_bin);
                    arquivo = (struct s_arq_no*)malloc(sizeof(struct s_arq_no)*tamanhoArquivo);

                    if (arquivo == NULL){

                        printf("ERRO DE MEMÓRIA\n");

                    }else{

                        raizArvore = leArquivo(raizArvore, fl_bin);
                        printf("ÁRQUIVO LIDO\n\n");
                    }
                }
                fclose(fl_bin);
                break;

            case 0:;

                desalocaArvore(raizArvore);
                printf("ÁRVORE DESALOCADA. \n\n\n");

                break;

            default:;

                printf("OPÇÃO INVÁLIDA \n");

                break;
        }

    }while(escolha != 0);


}

int main(){
    menu();
    return 0;
}