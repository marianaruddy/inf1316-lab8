#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 

#define N 20

// Alunas:
//  Mariana Medeiros Ruddy Santos   - 1611397
//  Camilla Santos Celles           - XXXXXXX

struct pagina {
	char endereco;
    char flag; // M (pagina modificada) ou R (pagina refernciada)
    int ultimo_acesso;
	struct pagina *prox;
};

char *tipo_algo;

int page_faults = 0;
int paginas_usadas = 0;
int num_paginas;
char *modo_debug = " ";

struct pagina *primeira, *ultima;


void adiciona_pagina(unsigned end, int i)
{
    if(strcmp(modo_debug, "debug")== 0 || strcmp(modo_debug, "-D")== 0) printf("\n-----adiciona pagina----\n");
	struct pagina *atual = (struct pagina*)malloc(sizeof(struct pagina));
    atual->endereco = end;
    atual->prox = NULL;
    atual->ultimo_acesso = i;

    if(paginas_usadas == 0)
    {
        primeira = atual;
        ultima = atual;
    }
    else
    {
        atual->prox = atual;
        atual = atual;
    }

    if(paginas_usadas < num_paginas)
    {
        paginas_usadas++;
    }
}

void NOVO(unsigned end)
{
    //TODO
}

void NRU(unsigned end)
{
    //TODO
}

void LRU(unsigned end, int i)
{
    adiciona_pagina(end, i);
    if(paginas_usadas == num_paginas)
    {
        primeira = primeira->prox;
        primeira->ultimo_acesso = i;
    }
}

void substitui(unsigned end, int i)
{
    if(strcmp(modo_debug, "debug")== 0 || strcmp(modo_debug, "-D")== 0) printf("\n-----substitui----\n");
    if(strcmp(tipo_algo, "LRU") == 0)
    {
        LRU(end, i);
    }
    else if(strcmp(tipo_algo, "NRU") == 0)
    {
        NRU(end);
    }
    else if(strcmp(tipo_algo, "NOVO") == 0)
    {
        // novo  algoritmo  de substituição  de  páginas ótimo imaginado  por  você
        NOVO(end);
    }
    else
    {
        printf("algoritmo de Substituição de Páginas %s é inválido\n", tipo_algo);
        exit(1);
    }
    page_faults++;
}

void escreve(unsigned end, int i)
{
    if(strcmp(modo_debug, "debug")== 0 || strcmp(modo_debug, "-D")== 0) printf("\n-----escreve----\n");
    if(paginas_usadas < num_paginas)
    {
        adiciona_pagina(end, i);
    }
    else
    {
        substitui(end, i);
    }
}

int procura(unsigned end){
	struct pagina *temp = primeira, *ant = NULL;
	while(temp != NULL){
		if(temp->endereco == end){
			if(strcmp(tipo_algo, "LRU") == 0){
				if(ant != NULL){
					if(temp->prox != NULL)
						ant->prox = temp->prox;							
				}
				else {
					primeira = primeira->prox;
				}
				ultima->prox = temp;
				ultima = temp;
				temp->prox = NULL;
			}
			return 0;
		}
		ant = temp;	
		temp = temp->prox;
	}
	return 1;
}


int main(int argc, char *argv[])
{
    // parametros de chamada do programa
    tipo_algo = argv[1];
    char *nome_arquivo = argv[2];
    int tam_pagina = atoi(argv[3]);
    int tam_mem_fisica = atoi(argv[4]);
    if(argv[5])
    {
        modo_debug = argv[5];
    }
    
    num_paginas = tam_mem_fisica / tam_pagina; 

    int i = 0;
    int s = 13;

    FILE *pnt_arquivo;
    char rw;
    unsigned end;
    int end_pagina;
    int pags_escritas = 0;
    int pags_faltantes = 0;

    if(tam_pagina == 8)
    {
        s += 0;
    }
    else if(tam_pagina == 16)
    {
        s += 1;
    }
    else if(tam_pagina == 32)
    {
        s += 2;
    }
    else
    {
        printf("Tamanho de Página %d é inválido\n", tam_pagina);
        exit(1);
    }

    pnt_arquivo = fopen(nome_arquivo, "r");
    if(pnt_arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        exit(1);
    }

    if(strcmp(modo_debug, "debug")== 0 || strcmp(modo_debug, "-D")== 0) printf("\n-----MODO DEBIG ATIVADO----\n");
    printf("\nExecutando o simulador...\n");
    printf("Arquivo de entrada: %s\n", nome_arquivo);
    printf("Tamanho da memoria fisica: %d\n", tam_mem_fisica);
    printf("Tamanho das páginas: %d\n", tam_pagina);
    printf("Alg de substituição: %s\n", tipo_algo);

    while(fscanf(pnt_arquivo, "%x %c\n", &end, &rw) != -1)
    {
        // printf("%d: %x %c\n", i, end, rw);
        end_pagina = end >> s;
        if(strcmp(&rw, "W") == 0)
        {
            escreve(end, i);
        }

        if(strcmp(&rw, "R") == 0)
        {
            if(procura(end) != 0)
            {
                escreve(end, i);
            }
        }
        i++;
    }

    // finalizacao do programa
    printf("Numero de Faltas de Páginas: %d\n", pags_faltantes);
    printf("Numero de Paginas escritas: %d\n", pags_escritas);
    fclose(pnt_arquivo);
	struct pagina *tmp = primeira;
	while(tmp != NULL){
		free(tmp);
		tmp = tmp->prox;
	}
    printf("a execução demorou %d de unidades de tempo\n", i);
    return 0;
};

// gcc sim-virtual.c -o sim-virtual && ./sim-virtual LRU Todos-Arquivos/compilador.log 8 16