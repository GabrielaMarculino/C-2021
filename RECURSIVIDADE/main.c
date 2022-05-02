#include <stdio.h>	//entrada e saída
#include <unistd.h>     //necessária para utilizar a função sleep
#include "gfx.h"	//chama a biblioteca do GFX

/*
Feito por: Gabriela Marculino
Comando para compilar no Terminal:
gcc -o main main.c gfx.h gfx.c -lSDL -lSDL_gfx -lSDL_ttf -Wall -Wextra -pedantic  
*/

/*
Observações:
neste programa foi utilizado caracteres especiais, pois o terminal suporta estas condições, entretanto se for executado 
em outro lugar, recomendo utilizar a função <locale.h> para haver o reconhecimento dos caracteres
*/

int altura = 0;
int recursao = 0; //variáveis globais (que precisa em mais de uma função)


/*
    Objetivo: montar a fígura do triângulo equilátero
    Retorno: Não possui nenhum retorno
*/
void montaTriangulo(unsigned int x, unsigned int y, unsigned int altura)
{

	gfx_set_color(255, 255, 255);	//define a cor das arestas, no caso a cor branca
	gfx_line(x, y, x + ((((2 * altura * 2) / 3) + 1) / 2), y - altura); //realiza o calculo para definir o tamanho de uma aresta
	gfx_line(x, y, x - ((((2 * altura * 2) / 3) + 1) / 2), y - altura);

	gfx_line(x + ((((2 * altura * 2) / 3) + 1) / 2), y - altura, x - ((((2 * altura * 2) / 3) + 1) / 2), y - altura); //define a figura do triangulo equilátero
}

/*
    Objetivo: fazer a função recursiva e assim, montar o Triângulo de Siepinski
    Retorno: Não possui nenhum retorno
*/
void sierpinski(unsigned int x, unsigned int y, int altura, unsigned int recursao, unsigned int controleRecursividade)
{

	montaTriangulo(x, y, altura); //chama a função que monta a fígura do triângulo

	if (controleRecursividade < recursao) // faz o controle da recursividade, para manter o triângulo de acordo com a forma desejada
	{
		sierpinski(x + ((((2 * altura * 2) / 3) + 1) / 2), y, (altura / 2), recursao, controleRecursividade + 1);
		sierpinski(x - ((((2 * altura * 2) / 3) + 1) / 2), y, (altura / 2), recursao, controleRecursividade + 1); //faz o calculo das arestas de acordo com o nível de recursão
		sierpinski(x, y - altura, (altura / 2), recursao, controleRecursividade + 1);
	}
}

/*
    Objetivo: chamar a fígura do triângulo para exibir na tela
    Retorno: Não possui nenhum retorno
*/
void chamaTriangulo()
{

	gfx_init(950, 1000, "Triangulo de Sierpinski"); //define o título da aba do GFX e o tamanho da tela (em pixels)
	sierpinski(500, 500, altura, recursao, 1);      //define a centralização do triângulo e mostra o desenho de Sierpinski

	gfx_paint(); //função pra exibir o desenho desejado
	sleep(7);    //a tela fecha automaticamente depois de 7 segundos

	gfx_quit(); //função que fecha/desliga a tela do GFX
}

/*
    Objetivo: fazer os tratamentos de possíveis erros que possa acontecer com o usuário,durante o teste deste programa
    Retorno: Não possui nenhum retorno
*/
void tratamentoErroSierpinski()
{

	int decisao = 0;

	printf("Digite a altura do Triangulo Sierpinski (EM PIXELS): \n");
	scanf("%d", &altura);
	//o usuário define os valores
	printf("Digite o nivel de recursão do triangulo: \n");
	scanf("%d", &recursao);

	if ((altura <= 0) || (recursao <= 0))
	{
		//a condição não aceita nenhum valor <= 0
		printf("Impossível realizar o desenho. Valores inválidos.\n\n");
	}
	else if ((recursao > 10) || (altura > 350))
	{

		do
		{
			printf("Certeza que deseja fazer um nível de recursão MAIOR que 10?\n");
			printf("Ou a altura do Triangulo ser maior que 350?\n");
			printf("Pois, pode demorar para aparecer a fígura, travar seu computador ou até mesmo não aparecer!\n\n");
			printf("Digite (1) - Para CONTINUAR\n");
			printf("Digite (2) - Para PARAR e voltar ao MENU PRINCIPAL.\n\n");

			scanf("%d", &decisao);

			switch (decisao)
			{
			case 1:
				chamaTriangulo(); //se o usuário quiser continuar, o programa chama a função para mostrar a fígura
				break;

			case 2:        				//deve-se levar em consideração que, em meu computador
				//sair                          //que possui um Celeron como processador, não conseguiu suportar
				break; 				//recursões acima do nível 12

			default:

				printf("Opção inválida!\n");
				break;
			}
		} while (decisao != 2);
	}
	else
	{
		chamaTriangulo();
	}
}

/*
    Objetivo: montar a fígura do quadrado
    Retorno: Não possui nenhum retorno
*/
void montaQuadrado(unsigned int x, unsigned int y, int altura){

	gfx_set_color(0, 0, 0);
	gfx_rectangle(x + altura / 2, y - altura / 2, x - altura / 2, y + altura / 2);
	gfx_set_color(255, 250, 250);
	gfx_filled_rectangle(x + (altura / 2) - 1, y - (altura / 2) + 1, x - (altura / 2) + 1, y + (altura / 2) - 1);
}

/*
    Objetivo: fazer a função recursiva e assim, montar os quadrados sobrepostos
    Retorno: Não possui nenhum retorno
*/
void quadradosSobrepostos(int x, int y, int altura, int recursao, int controleRecursividade){

	montaQuadrado(x, y, altura); 

	if(recursao > controleRecursividade){

		quadradosSobrepostos(x + altura / 2, y - altura / 2, altura / 2, recursao, controleRecursividade + 1);
	    quadradosSobrepostos(x + altura / 2, y + altura / 2, altura / 2, recursao, controleRecursividade + 1);
		quadradosSobrepostos(x - altura / 2, y + altura / 2, altura / 2, recursao, controleRecursividade + 1);
		quadradosSobrepostos(x - altura / 2, y - altura / 2, altura / 2, recursao, controleRecursividade + 1);
	}
}

/*
    Objetivo: chamar a fígura do quadrado sobreposto para exibir na tela
    Retorno: Não possui nenhum retorno
*/
void chamaQuadrado(){
	
	gfx_init(950, 1000, "Quadrados Sobrepostos");
	quadradosSobrepostos(500, 500, altura, recursao, 1);
	gfx_paint();
	sleep(7);
	gfx_quit();

}

/*
    Objetivo: fazer os tratamentos de possíveis erros que possa acontecer com o usuário,durante o teste deste programa
    Retorno: Não possui nenhum retorno
*/
void tratamentoErroQuadrado(){


	int decisao = 0;

	printf("Digite a altura do quadrado (EM PIXELS): \n");
	scanf("%d", &altura);
	//o usuário define os valores
	printf("Digite o nivel de recursão do quadrado: \n");
	scanf("%d", &recursao);

	if ((altura <= 0) || (recursao <= 0))
	{
		//a condição não aceita nenhum valor <= 0
		printf("Impossível realizar o desenho. Valores inválidos.\n\n");
	}
	else if ((recursao > 10) || (altura > 350))
	{

		do
		{
			printf("Certeza que deseja fazer um nível de recursão MAIOR que 10?\n");
			printf("Ou a altura do quadrado ser maior que 350?\n");
			printf("Pois, pode demorar para aparecer a fígura, travar seu computador ou até mesmo não aparecer!\n\n");
			printf("Digite (1) - Para CONTINUAR\n");
			printf("Digite (2) - Para PARAR e voltar ao MENU PRINCIPAL.\n\n");

			scanf("%d", &decisao);

			switch (decisao)
			{
			case 1:
				chamaQuadrado(); //se o usuário quiser continuar, o programa chama a função para mostrar a fígura
				break;

			case 2:        				
				//sair                          
				break; 				

			default:

				printf("Opção inválida!\n");
				break;
			}
		} while (decisao != 2);
	}
	else
	{
		chamaQuadrado();
	}
}
/*
    Objetivo: mostrar minha "assinatura"
    Retorno: Não possui nenhum retorno
*/
void creditos()
{

	printf("Feito por: Gabriela Marculino\n");
	printf("RGM: 41431\n");
	printf("Curso: Ciência da Computaçao\n");
	printf("Matéria: Algoritmos e Estrutura de Dados II\n\n");
}

/*
    Objetivo: mostrar o menu de escolhas que usuário pode fazer
    Retorno: Não possui nenhum retorno
*/
void menu()
{

	int escolha = 0;

	do
	{
		printf("FEITO POR: Gabriela Marculino RGM: 41431\n\n");
		printf("Digite o numero da opcao que deseja: \n\n");
		printf("1 - Triangulo de Sierpinski\n");
		printf("2 - Quadrados Sobrepostos\n");
		printf("3 - Créditos\n\n");
		printf("0 - SAIR\n\n");

		scanf("%d", &escolha);

		switch (escolha)
		{
		case 1:
			tratamentoErroSierpinski();
			break;

		case 2:
			tratamentoErroQuadrado();
			break;

		case 3:
			creditos();
			break;

		case 0:
			//sair
			break;

		default:
			printf("Opção inválida.\n\n");
			break;
		}
	} while (escolha != 0);
}

int main()
{
	menu();
	return 0;
}
