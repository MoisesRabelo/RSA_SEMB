/*
Código RSA
Serve para criptografar uma mensagem simples de até 2000 caracteres pelo modelo RSA
Entrada: 2 números primos, e uma mensagem
Saída:n,e,d,mensagem encriptada
Código feito para disciplina de Sistemas Embarcados
Alunos: Moises Moura Rabelo, Francisco Gleyson de Braga Vieira
Data:10/12/2020 
Licença para software livre
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <conio.h>
#include <math.h>

#define TAM 2001 //o tamanho máximo da mensagem

//variáveis Globais

long int e[TAM],d[TAM]; //vetor que guarda os possíveis valores de e,d
long int en[TAM]; //a mensagem encriptada

long int m[TAM]; //a mensagem descriptada
long int temp[TAM]; //um vetor auxiliar para ajudar na descriptografia, que será usada apenas para mostrar o funcionamento da criptografia

long int first,second; //os dois primos
long int totient,n; //totient e a multiplicação entre os dois primos
long int i,j,flag; //variáveis para me auxiliar
char msg[TAM]; //a mensagem

//funções Gráficas

void menu();
void falsePrimo();

//funções de Processamento

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
    printf("\n|                          Codificação de Mensagem                            |");
    printf("\n|_____________________________________________________________________________|\n\n");
}
void falsePrimo() {
	printf("\n _____________________________________________________________________________");
	printf("\n|                       PRIMO INVÁLIDO! TENTE NOVAMENTE                       |");
	printf("\n|                   PRESSIONE QUALQUER TECLA PARA CONTINUAR                   |");
	printf("\n|_____________________________________________________________________________|\n");
	getch();
}
// recebe dois números, e só termina quando os dois forem Primos
void recebePrimo() {
	
	while(1)
	{
		printf("\nPrimeiro Número Primo:\n"); 
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
		printf("\nSegundo Número Primo\n"); 
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
-Recebe um número
-Guarda o valor de sua raiz em uma variável
-Dentro de um loop, fica dividindo o numero recebido por i(onde o menor é 2),
até o i ser maior que a raiz do numero recebido
-Se em algum momento o modulo for 0, significa que não é um número primo, retornado 0,
caso contrário, retorna 1, pois é primo
*/
int verificaPrimo(long int pr) {
	
	long int i;
	j = sqrt(pr); //função que retorna a raiz quadrada
 
	for (i = 2;  i<=j; i++) {
 		if(pr%i == 0) return 0; 
	} 
	return 1; 
}
/*
Entramos em um laço de repetição com a duração sendo o tamanho da mensagem recebida
Na variável p colocamos a mensagem com um decréscimo de 96 para não haver caracteres inválidos durante a mensagem encriptada
Mudamos o valor de k na quantia de vezes do e, para que depois darmos esse valor para a respectiva parte encriptada, sem esquecer dos 96 tirados antes
No fim do while, colocamos em sua ultima posição um -1, para sabermos quando acaba em si, a mensagem encriptada
*/
void encrypt() {
 
	long int k,length;
 	long int key = e[0]; //o e da chave publica
 	
	length = strlen(msg); //tamanho da mensagem
	i = 0;
	
	while(i != length) //enquanto não tiver rodado por toda a mensagem
	{
		k = 1;
 
		for (j = 0; j < key; j++) //roda a quantia do e, para codificar a parte da mensagem em questão
		{ 
			k = k*msg[i]; //multiplica o k pela parte da mensagem 
 			k = k%n; //faz que o valor seja o modulo entre a multiplicação da linha anterior pelo n
 		}
 		temp[i] = k;
 		en[i] = k; //devolve os 96 tirados anteriormente
 		i++;
 	}
	en[i] = -1; //para saber o final da mensagem encriptada
 
 	//Retorno dos dados de saída(n,e,d, mensagem encriptada)
 
 	printf("\nn: %ld\ne: %ld\nd: %ld",n,e[0],d[0]); //n,e,d
  	printf("\n");
 	
	printf("\nMensagem Encriptada:\n");
	for (i = 0; en[i]!=-1; i++) printf("%ld ",en[i]); //mensagem encriptada
	printf("\n");
}
/*
-O e tem que ser menor que totient e maior que 1, por isso usamos um laço que obriga esse setup, mas pode acabar assim que k for igual a 1999, no caso,
2000 laços com o constructorD retornando um numero positivo
-O e não pode ser divisível com o totient, se for, pulamos o laço e não adicionamos no vetor a qual guarda todos os possíveis e
-Necessita ser primo e diferente dos dois primos usados para calcular n, totient
-É chamada a função contructorD, e se o numero nao for negativo, adicionamos essa flag como um possível d no vetor de possíveis d,
e aumenta um no valor de k
-Se um d não passar na validação positiva, o k não aumentará, fazendo com que o proximo e substitua o antigo, já que ele não tem um d
*/
void constructorE() {
 
	int k = 0;
 
	for (i = 2; i < totient; i++)
	{
		if(totient % i == 0) continue; //se totient for divisivel por i, pulamos para o prox laço
 
		flag = verificaPrimo(i);
 
		if(flag == 1 && i != first && i != second) //tem que ser primo diferente dos primos recebidos para o cálculo de n,totient
		{
			e[k] = i; //adicionado um dos possíveis valores de e em seu vetor
 
			flag = constructorD(e[k]); //procura o d respectivo do e
 
			if(flag > 0) //se o d calculado for positivo, está valido
			{
				d[k] = flag; //adicionado um dos possíveis valores de d em seu vetor
				k++;
			}
			if(k == (TAM-1)) break; //verificação de tamanho do vetor de d,e
		}
	}
}
/*
Aumentasse k que inicia com o valor 1, em totient até que o módulo do k com e seja 0, retornando o valor da divisão entre eles
Já que o d é o inverso multiplicativo de e no mod de totient
*/
long int constructorD(long int e) {
 
	long int k = 1;
 
	while(1)
	{
 		k = k + totient; //o valor de k será implementado totient até o laço de repetição ser quebrado
 		if(k%e == 0) return (k/e);
	}
 
}
int main()
{
	setlocale(LC_ALL, "Portuguese"); //Para aceitar acentuação
	
	menu();
	recebePrimo();
 	menu();
 	//recebe a mensagem a qual será codificada
	printf("\nEscreva a mensagem: \n");
	fflush(stdin); //limpar buffer do teclado
	gets(msg);
  
	n = first*second; //módulo da chave privada e pública
 
	totient = (first-1)*(second-1); //função totient
 
	constructorE();
	encrypt();
		
	decrypt(); //Parte pra provar que a criptografia está funcionando

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
