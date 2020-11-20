#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 

#define N 20

// Alunas:
//  Mariana Medeiros Ruddy Santos   - 1611397
//  Camilla Santos Celles           - XXXXXXX

int main(int argc, char *argv[])
{
    // parametros
    char *tipo_algo = argv[1];
    char *nome_arquivo = argv[2];
    int tam_pagina = atoi(argv[3]);
    int tam_mem_fisica = atoi(argv[4]);

    int num_paginas = tam_mem_fisica / tam_pagina; 
    int contador = 0;

    clock_t tempo_ini = clock(); 

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
    
    FILE *pnt_arquivo;
    char buff[N];
    char rw;
    unsigned end;

    pnt_arquivo = fopen(nome_arquivo, "r");
    if(pnt_arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        exit(1);
    }
    printf("\nExecutando o simulador...\n");
    printf("Arquivo de entrada: %s\n", nome_arquivo);
    printf("Tamanho da memoria fisica: %d\n", tam_mem_fisica);
    printf("Tamanho das páginas: %d\n", tam_pagina);
    printf("Alg de substituição: %s\n", tipo_algo);
    printf("Numero de Faltas de Páginas: \n");
    printf("Numero de Paginas escritas: \n");

    while(fscanf(pnt_arquivo, "%x %c\n", &end, &rw) != -1)
    {
        printf("%d: %x %c\n", i, end, rw);

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
    fclose(pnt_arquivo);
    clock_t tempo_fim = clock(); 
    printf("a execução demorou %f milisegundos\n", (tempo_fim - tempo_ini) / (double)CLOCKS_PER_SEC);
    printf("a execução demorou %d de unidades de tempo\n", contador);   // nao entendi muito bem qual dessas formas é pra contar o tempo
    return 0;
};

// gcc sim-virtual.c -o sim-virtual && ./sim-virtual LRU Todos-Arquivos/compilador.log 8 16