#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 

#define N 20

// Alunas:
//  Mariana Medeiros Ruddy Santos   - 1611397
//  Camilla Santos Celles           - XXXXXXX

char *modo_debug = " ";

int main(int argc, char *argv[])
{
    // parametros
    char *tipo_algo = argv[1];
    char *nome_arquivo = argv[2];
    int tam_pagina = atoi(argv[3]);
    int tam_mem_fisica = atoi(argv[4]);

    int num_paginas = tam_mem_fisica / tam_pagina; 
    int contador = 0;
    int s = 13;

    if(strcmp(tipo_algo, "LRU") == 0)
    {
        // LRU
    }
    else if(strcmp(tipo_algo, "NRU") == 0)
    {
        // NRU
    }
    else if(strcmp(tipo_algo, "NOVO") == 0)
    {
        // novo  algoritmo  de substituição  de  páginas ótimo imaginado  por  você
    }
    else
    {
        printf("algoritmo de Substituição de Páginas %s é inválido\n", tipo_algo);
        exit(1);
    }

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
    
    FILE *pnt_arquivo;
    char buff[N];
    char rw;
    unsigned end;
    int end_pagina;
    int pags_escritas = 0;
    int pags_faltantes = 0;

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
            // write
        }

        if(strcmp(&rw, "R") == 0)
        {
            // read
        }
        contador++;
    }

    // finalizacao do programa
    printf("Numero de Faltas de Páginas: %d\n", pags_faltantes);
    printf("Numero de Paginas escritas: %d\n", pags_escritas);
    fclose(pnt_arquivo);
    printf("a execução demorou %d de unidades de tempo\n", contador);
    return 0;
};

// gcc sim-virtual.c -o sim-virtual && ./sim-virtual LRU Todos-Arquivos/compilador.log 8 16