/*---BIBLIOTECAS----*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
    int quantityProds;
    Product products[1000];
} Sell;

/*------VARIAVEIS GLOBAIS-------*/
int registeredSells = 0;
Sell sells[1000];
int contProd;

/*FUNCOES AUXILIARES*/

// Funcao para retornar a data/hora atual
char *getCurrentDate() {
    static char todayDateStr[100];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY HH:MM:SS") + 1, "%d-%b-%Y %H:%M:%S", timeinfo);

    return todayDateStr;
}

// Funcao para gerar o id e controlar a quantidade de vendas
int generateId() {
    registeredSells++;
    return registeredSells;
}

// Funcao para inserir a venda no array
void registerSell(Sell s) {
    sells[registeredSells - 1] = s;
}

/*FUNCOES DO MENU*/
// Funcao para criar uma venda
void insert() {
    char continuar;
    Sell s;
    contProd = 0;
    s.value = 0;

    s.id = generateId();
    strcpy(s.date, getCurrentDate());
    do {
        system("@cls||clear");
        printf("\nRegistrar venda");

        printf("\nDigite o nome do produto: ");
        setbuf(stdin, NULL);
        scanf("%49[^\n]", s.products[contProd].name);

        while (1) {
            setbuf(stdin, NULL);
            printf("\nDigite a quantidade: ");
            scanf("%d", &s.products[contProd].quantity);

            if (s.products[contProd].quantity > 0) {
                break;
            } else {
                printf("Valor invalido\n");
            }
        }

        while (1) {
            setbuf(stdin, NULL);
            printf("\nDigite o preco: ");
            scanf("%f", &s.products[contProd].value);

            if (s.products[contProd].value > 0.0) {
                break;
            } else {
                printf("Valor invalido\n");
            }
        }

        s.value += (s.products[contProd].quantity * s.products[contProd].value);

        contProd++;

        while (1) {
            printf("\nDeseja adicionar mais produtos?[s/n] ");
            scanf(" %c", &continuar);

            continuar = tolower(continuar);

            if (continuar == 's' || continuar == 'n') break;
            else printf("Valor invalido\n");
        }

    } while (continuar == 's');

    s.quantityProds = contProd;
    registerSell(s);
}

// Funcao para imprimir uma lista de Sell em JSON
void jsonfy(Sell *s, int quantitySells) {
    printf("\n\n[\n");
    for (int i = 0; i < quantitySells; i++) {
        printf("\t{\n");
        printf("\t\t'Id': %d,\n", s[i].id);
        printf("\t\t'Data da venda': %s', \n", s[i].date);
        printf("\t\t'Valor total': %.2f, \n", s[i].value);
        printf("\t\t'Produtos': [ \n");

        for (int j = 0; j < s[i].quantityProds; j++) {
            printf("\t\t\t{\n");
            printf("\t\t\t\t'Nome': '%s',\n", s[i].products[j].name);
            printf("\t\t\t\t'Quantidade': %d,\n", s[i].products[j].quantity);
            printf("\t\t\t\t'Valor': %.2f\n", s[i].products[j].value);

            if ((j + 1) == s[i].quantityProds) {
                printf("\t\t\t}\n");
            } else {
                printf("\t\t\t},\n");
            }
        }

        printf("\t\t]\n");

        if ((i + 1) == quantitySells) {
            printf("\t}\n\n");
        } else {
            printf("\t},\n\n");
        }
    }

    printf("]\n\n");
}

// Funcao para retornar todas as vendas
void find() {
    system("@cls||clear");

    if (registeredSells > 0) {
        jsonfy(sells, registeredSells);
    } else {
        printf("\nNao foram encontradas vendas registradas\n\n");
    }
}

void advancedSearch() {
    int choosenParam;
    Sell searchSell;

    system("@cls||clear");

    if (registeredSells > 0) {
        system("@cls||clear");
        printf("1. Id\n2. Valor total");

        while (1) {
            setbuf(stdin, NULL);
            printf("\nPesquisar por: ");
            scanf("%d", &choosenParam);

            if (choosenParam >= 1 && choosenParam <= 2) {
                break;
            } else {
                printf("Valor invalido\n");
            }
        }

        int isValid = -1;
        switch (choosenParam) {
            case 1:
                do {
                    int idDigitado;
                    setbuf(stdin, NULL);
                    printf("Pesquisar pelo id: ");
                    isValid = scanf("%d", &idDigitado);

                    if (idDigitado <= 0 || idDigitado > registeredSells) {
                        isValid = -1;
                    }

                    if (isValid != 1) {
                        printf("Valor invalido\n");
                    } else {
                        searchSell.id = idDigitado;
                    }
                } while (isValid != 1);

                Sell foundMatch[1];

                for (int i = 0; i < registeredSells; i++) {
                    if (sells[i].id == searchSell.id) {
                        foundMatch[0] = sells[i];
                    }
                }

                jsonfy(foundMatch, 1);
                break;

            case 2:
                do {
                    float valorDigitado;
                    setbuf(stdin, NULL);
                    printf("Pesquisar valores a partir de: ");
                    isValid = scanf("%f", &valorDigitado);

                    if (valorDigitado <= 0) {
                        isValid = -1;
                    }
                    if (isValid != 1) {
                        printf("Valor invalido\n");
                    } else {
                        searchSell.value = valorDigitado;
                    }
                } while (isValid != 1);


                Sell foundMatches[30];
                int matchesCounter = 0;
                for (int i = 0; i < registeredSells; i++) {
                    if (sells[i].value >= searchSell.value) {
                        foundMatches[matchesCounter] = sells[i];
                        matchesCounter++;
                    }
                }

                if (matchesCounter > 0) {
                    jsonfy(foundMatches, matchesCounter);
                } else {
                    printf("Nao foram encontradas vendas para o parametro informado");
                }
                break;

        }


    } else {
        printf("\nNao foram encontradas vendas registradas\n\n");
    }
}

void initMenu() {
    char continuar;
    int option;

    do {

        system("@cls||clear");

        printf("\n1. Registrar venda\n2. Consultar vendas\n3. Consulta avancada\n4. Sair");

        while (1) {
            setbuf(stdin, NULL);
            printf("\n\nEscolha uma opcao: ");
            scanf("%d", &option);

            if (option >= 1 && option <= 4) {
                break;
            } else {
                printf("Valor invalido\n");
            }
        }

        switch (option) {
            case 1:
                insert();
                break;
            case 2:
                find();
                break;
            case 3:
                advancedSearch();
                break;
        }

        if (option == 4)
            break;

        while (1) {
            printf("\n\nDeseja realizar outra operacao?[s/n] ");
            scanf(" %c", &continuar);

            continuar = tolower(continuar);

            if (continuar == 's' || continuar == 'n') break;
            else printf("Valor invalido");
        }

    } while (continuar == 's');
}

int main() {
    initMenu();
}