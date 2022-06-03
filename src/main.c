/*---BIBLIOTECAS----*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


/*-------STRUCTS------*/
typedef struct {
	char name[50];
	float value;
	int quantity;
} Product;

typedef struct {
	int id;
	char date[50];
	float value;
	Product products[1000];
} Sell;

/*------VARIAVEIS GLOBAIS-------*/
static int idSell = 0;
static Sell sells[255];
static int contProd;

/*FUNCOES AUXILIARES*/

//Funcao para retornar a data/hora atual
char* getCurrentDate() {
	static char todayDateStr[100];
	time_t rawtime;
	struct tm *timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(todayDateStr, strlen("DD-MMM-YYYY HH:MM")+1,"%d-%b-%Y %H:%M",timeinfo);

	return todayDateStr;
}

//Funcao para gerar o id e controlar a quantidade de vendas
int generateId() {
	idSell++;
	return idSell;
}

//Funcao para inserir a venda no array
void registerSell(Sell s) {
	sells[idSell-1] = s;
}



/*FUNCOES DO MENU*/
//Funcao para criar uma venda
void insert() {
	char continuar;
	Sell s;
	contProd = 0;

	do {
		system("@cls||clear");
		printf("\nRegistrar venda");
		
		printf("\nDigite o nome do produto: ");
		setbuf(stdin,NULL);
		scanf("%49[^\n]", s.products[contProd].name);

		setbuf(stdin,NULL);
		printf("\nDigite a quantidade: ");
		scanf("%d", &s.products[contProd].quantity);

		printf("\nDigite o preco: ");
		scanf("%f", &s.products[contProd].value);

		s.value += (s.products[contProd].quantity * s.products[contProd].value);

		contProd++;

		printf("Adicionar mais produtos?[s/n] ");
		scanf(" %c", &continuar);
	} while (!(continuar != 's') || !(continuar != 'S'));

	s.id = generateId();
	strcpy(s.date,getCurrentDate());
	registerSell(s);
}

//Funcao para retornar todas as vendas no formato de JSON
void find() {

	system("@cls||clear");

	if (idSell == 0) {
		printf("\nNao foram encontradas vendas registradas\n\n");
	} else {
		printf("\n\n[\n");
		for(int i = 0; i < idSell; i++) {
			printf("\t{\n");
			printf("\t\t'Id': %d,\n", sells[i].id);
			printf("\t\t'Data da venda': '%s', \n", sells[i].date);
			printf("\t\t'Valor total': %.2f, \n", sells[i].value);
			printf("\t\t'Produtos': [ \n");

			for(int j = 0; j < contProd; j++) {
				printf("\t\t\t{\n");
				printf("\t\t\t\t'Nome': '%s',\n", sells[i].products[j].name);
				printf("\t\t\t\t'Quantidade': %d,\n", sells[i].products[j].quantity);
				printf("\t\t\t\t'Valor': %.2f\n", sells[i].products[j].value);

				if ((j + 1) == contProd) {
					printf("\t\t\t}\n");
				} else {
					printf("\t\t\t},\n");
				}
			}

			printf("\t\t]\n");
			
			if ((i + 1) == idSell){
				printf("\t}\n\n");
			} else {
				printf("\t},\n\n");
			}
		}
		
		printf("]\n\n");
	}
}

int main() {

	char continuar;
	int option;

	do {

		system("@cls||clear");

		printf("\n1. Registrar venda\n2. Consultar vendas\n3. Sair");
		printf("\n\nEscolha uma opcao: ");
		scanf("%d",&option);

		switch (option) {
			case 1:
				insert();
				break;
			case 2:
				find();
				break;
			case 3:
				printf("Voce escolheu sair");
				break;
			default:
				system("@cls||clear");
				break;
		}

		printf("\n\nDeseja realizar outra operacao?[s/n] ");
		scanf(" %c", &continuar);

	} while (!(continuar != 's') || !(continuar != 'S'));
}