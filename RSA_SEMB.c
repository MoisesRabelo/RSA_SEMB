/*
C�digo RSA
Serve para criptografar uma mensagem simples de at� 2000 caracteres pelo modelo RSA
Entrada: 2 n�meros primos, e uma mensagem
Sa�da:n,e,d,mensagem encriptada
C�digo feito para disciplina de Sistemas Embarcados
Alunos: Moises Moura Rabelo, Francisco Gleyson de Braga Vieira
Data:10/12/2020 
Licen�a para software livre
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <conio.h>
#include <math.h>

#define TAM 2001 //o tamanho m�ximo da mensagem

//vari�veis Globais

long int e[TAM],d[TAM]; //vetor que guarda os poss�veis valores de e,d
long int en[TAM]; //a mensagem encriptada

long int m[TAM]; //a mensagem descriptada
long int temp[TAM]; //um vetor auxiliar para ajudar na descriptografia, que ser� usada apenas para mostrar o funcionamento da criptografia

long int first,second; //os dois primos
long int totient,n; //totient e a multiplica��o entre os dois primos
long int i,j,flag; //vari�veis para me auxiliar
char msg[TAM]; //a mensagem

//fun��es Gr�ficas

void menu();
void falsePrimo();

//fun��es de Processamento

void recebePrimo();
void encrypt();
void decrypt();
long int constructorD(long int);
void constructorE();
int verificaPrimo(long int);

//______________________________//

void menu(){
	system("cls");
    printf("\n _____________________________________________________________________________");
    printf("\n|                          Codifica��o de Mensagem                            |");
    printf("\n|_____________________________________________________________________________|\n\n");
}
void falsePrimo() {
	printf("\n _____________________________________________________________________________");
	printf("\n|                       PRIMO INV�LIDO! TENTE NOVAMENTE                       |");
	printf("\n|                   PRESSIONE QUALQUER TECLA PARA CONTINUAR                   |");
	printf("\n|_____________________________________________________________________________|\n");
	getch();
}
// recebe dois n�meros, e s� termina quando os dois forem Primos
void recebePrimo() {
	
	while(1)
	{
		printf("\nPrimeiro N�mero Primo:\n"); 
		scanf("%d",&first);	 
		flag = verificaPrimo(first);
	 
		if(flag == 0)
		{ 
			falsePrimo();
			menu();
		}else break;
	}
	
 	
 	while(1)
	{
		printf("\nSegundo N�mero Primo\n"); 
		scanf("%d",&second);	 
		flag = verificaPrimo(second);
	 
		if(flag == 0 || first == second)
		{ 
			falsePrimo();
			menu();
		}else break;
	}
	
}
/*
-Recebe um n�mero
-Guarda o valor de sua raiz em uma vari�vel
-Dentro de um loop, fica dividindo o numero recebido por i(onde o menor � 2),
at� o i ser maior que a raiz do numero recebido
-Se em algum momento o modulo for 0, significa que n�o � um n�mero primo, retornado 0,
caso contr�rio, retorna 1, pois � primo
*/
int verificaPrimo(long int pr) {
	
	long int i;
	j = sqrt(pr); //fun��o que retorna a raiz quadrada
 
	for (i = 2;  i<=j; i++) {
 		if(pr%i == 0) return 0; 
	} 
	return 1; 
}
/*
Entramos em um la�o de repeti��o com a dura��o sendo o tamanho da mensagem recebida
Na vari�vel p colocamos a mensagem com um decr�scimo de 96 para n�o haver caracteres inv�lidos durante a mensagem encriptada
Mudamos o valor de k na quantia de vezes do e, para que depois darmos esse valor para a respectiva parte encriptada, sem esquecer dos 96 tirados antes
No fim do while, colocamos em sua ultima posi��o um -1, para sabermos quando acaba em si, a mensagem encriptada
*/
void encrypt() {
 
	long int k,length;
 	long int key = e[0]; //o e da chave publica
 	
	length = strlen(msg); //tamanho da mensagem
	i = 0;
	
	while(i != length) //enquanto n�o tiver rodado por toda a mensagem
	{
		k = 1;
 
		for (j = 0; j < key; j++) //roda a quantia do e, para codificar a parte da mensagem em quest�o
		{ 
			k = k*msg[i]; //multiplica o k pela parte da mensagem 
 			k = k%n; //faz que o valor seja o modulo entre a multiplica��o da linha anterior pelo n
 		}
 		temp[i] = k;
 		en[i] = k; //devolve os 96 tirados anteriormente
 		i++;
 	}
	en[i] = -1; //para saber o final da mensagem encriptada
 
 	//Retorno dos dados de sa�da(n,e,d, mensagem encriptada)
 
 	printf("\nn: %ld\ne: %ld\nd: %ld",n,e[0],d[0]); //n,e,d
  	printf("\n");
 	
	printf("\nMensagem Encriptada:\n");
	for (i = 0; en[i]!=-1; i++) printf("%ld ",en[i]); //mensagem encriptada
	printf("\n");
}
/*
-O e tem que ser menor que totient e maior que 1, por isso usamos um la�o que obriga esse setup, mas pode acabar assim que k for igual a 1999, no caso,
2000 la�os com o constructorD retornando um numero positivo
-O e n�o pode ser divis�vel com o totient, se for, pulamos o la�o e n�o adicionamos no vetor a qual guarda todos os poss�veis e
-Necessita ser primo e diferente dos dois primos usados para calcular n, totient
-� chamada a fun��o contructorD, e se o numero nao for negativo, adicionamos essa flag como um poss�vel d no vetor de poss�veis d,
e aumenta um no valor de k
-Se um d n�o passar na valida��o positiva, o k n�o aumentar�, fazendo com que o proximo e substitua o antigo, j� que ele n�o tem um d
*/
void constructorE() {
 
	int k = 0;
 
	for (i = 2; i < totient; i++)
	{
		if(totient % i == 0) continue; //se totient for divisivel por i, pulamos para o prox la�o
 
		flag = verificaPrimo(i);
 
		if(flag == 1 && i != first && i != second) //tem que ser primo diferente dos primos recebidos para o c�lculo de n,totient
		{
			e[k] = i; //adicionado um dos poss�veis valores de e em seu vetor
 
			flag = constructorD(e[k]); //procura o d respectivo do e
 
			if(flag > 0) //se o d calculado for positivo, est� valido
			{
				d[k] = flag; //adicionado um dos poss�veis valores de d em seu vetor
				k++;
			}
			if(k == (TAM-1)) break; //verifica��o de tamanho do vetor de d,e
		}
	}
}
/*
Aumentasse k que inicia com o valor 1, em totient at� que o m�dulo do k com e seja 0, retornando o valor da divis�o entre eles
J� que o d � o inverso multiplicativo de e no mod de totient
*/
long int constructorD(long int e) {
 
	long int k = 1;
 
	while(1)
	{
 		k = k + totient; //o valor de k ser� implementado totient at� o la�o de repeti��o ser quebrado
 		if(k%e == 0) return (k/e);
	}
 
}
int main()
{
	setlocale(LC_ALL, "Portuguese"); //Para aceitar acentua��o
	
	menu();
	recebePrimo();
 	menu();
 	//recebe a mensagem a qual ser� codificada
	printf("\nEscreva a mensagem: \n");
	fflush(stdin); //limpar buffer do teclado
	gets(msg);
  
	n = first*second; //m�dulo da chave privada e p�blica
 
	totient = (first-1)*(second-1); //fun��o totient
 
	constructorE();
	encrypt();
		
	decrypt(); //Parte pra provar que a criptografia est� funcionando

	return 0;
}
void decrypt() {

	long int k,p;
 	long int key = d[0];
	i = 0;
	while(en[i] != -1)
	{ 
		k = 1;
 
		for (j = 0; j < key; j++) {
			k = k*temp[i];
			k = k%n;
		}
		p = k;
 		m[i] = p;
 		i++;
 	}
	m[i] = -1;
 
	printf("\nMensagem Descriptografada:\n"); 	
	for (i = 0; m[i] != -1; i++) printf("%c",m[i]); 
}
