// Alunas:
//  Mariana Medeiros Ruddy Santos   - 1611397
//  Camila Santos Celes             - 1810336

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct pagina {
  int endereco;
  char R;
  char M;
  int ultimo_acesso;
}; typedef struct pagina Pagina;

Pagina* emQuadro[500]; // armazena vetor de paginas que estao ja mapeadas 

char* algoritmo;
FILE *arquivo_log;
int s;
int num_paginas;
int num_paginas_usadas = 0;
int page_faults = 0;
int paginas_sujas = 0;
int conta_tempo = 0;

int procuraPagina(int end) {
  for (int i=0; i<num_paginas_usadas; i++) {
    if (emQuadro[i]->endereco == end)
      return i;
  }
  return -1;
}

void adicionaPagina(int end, char rw) {
  page_faults++;

  Pagina *p = (Pagina *)malloc(sizeof(Pagina));
  p->endereco = end;
  p->R = (rw == 'R');
  p->M = (rw == 'M');
  p->ultimo_acesso = conta_tempo;

  emQuadro[num_paginas_usadas++] = p;
}

int descartaLRU() {
  int ind = 0;
  for (int i=1; i<num_paginas; i++) { // procurando indice da menos recente
    if (emQuadro[i]->ultimo_acesso < emQuadro[ind]->ultimo_acesso) {
      ind = i;
    }
  }
  return ind;
}

int descartaNRU() {
  // procura por M=0, R=0
  for (int i=1; i<num_paginas; i++) {
    if (!emQuadro[i]->M && !emQuadro[i]->R) {
      return i;
    }
  }
  // procura por M=1, R=0
  for (int i=1; i<num_paginas; i++) {
    if (emQuadro[i]->M && !emQuadro[i]->R) {
      return i;
    }
  }
  // procura por M=0, R=1
  for (int i=1; i<num_paginas; i++) {
    if (!emQuadro[i]->M && emQuadro[i]->R) {
      return i;
    }
  }
  return 0; // pega a primeira M=1, R=1
}

int descartaNOVO() {
  // iremos "olhar para o futuro" e ver qual das paginas em quadro esta mais 
  // longe de ser usada de novo e substituir essa (entre as 10 proximas)

  // armazenando posicao do cursor na leitura do arquivo pela main para depois retornar a esse ponto
  fpos_t cursor;
  fgetpos(arquivo_log, &cursor);

  char flags[500]; for(int i=0; i<num_paginas; i++) flags[i] = 0;
  unsigned addrs; char rw;
  int count = 0;
  int count_linhas = 0;
  while (count_linhas < 10 && fscanf(arquivo_log, "%x %c\n", &addrs, &rw) != -1) {
    int end_pagina = addrs >> s;
    for (int i=0; i<num_paginas && count<num_paginas-1; i++) {
      if (emQuadro[i]->endereco == end_pagina) { // uma das paginas em quadro foi encontrada "no futuro"
        if (!flags[i]) {
          flags[i] = 1;
          count++;
        }
      }
    }
    count_linhas++;
  }
  int ind;
  for (int i=0; i<num_paginas; i++) {
    if (!flags[i]) {
      ind = i; // vamos retornar a pagina que nao foi encontrada no futuro proximo (flag = 0)
      break;
    }
  }

  fsetpos(arquivo_log, &cursor); // voltando com o cursor da main para onde estava
  return ind;
}

void descartaPagina() {
  int ind;
  // procurando qual descartar segundo algoritmo de substituicao
  if (strcmp(algoritmo, "LRU") == 0) {
    ind = descartaLRU();
  }
  else if (strcmp(algoritmo, "NRU") == 0) {
    ind = descartaNRU();
  }
  else { // algoritmo == NOVO
    ind = descartaNOVO();
  }

  // descartando a pagina do vetor de em quadro, substituindo pela ultima posicao do vetor
  if (emQuadro[ind]->M)
    paginas_sujas++;
  free(emQuadro[ind]);
  emQuadro[ind] = emQuadro[--num_paginas_usadas];
}

void paginaLida(int end, char rw) {
  int indPagina = procuraPagina(end);
  if (indPagina >= 0) { // pagina ja em quadro, atualiza seus dados
    if (rw == 'R') {
      emQuadro[indPagina]->R = 1;
   } else {
      emQuadro[indPagina]->M = 1;
    }
    emQuadro[indPagina]->ultimo_acesso = conta_tempo;
  } 
  else { // pagina nao em quadro, a adiciona
    if (num_paginas_usadas == num_paginas)
      descartaPagina();
    adicionaPagina(end, rw);
  }
}


int main(int argc, char *argv[]) {
  // lendo parametros da chamada do simulador
  algoritmo = argv[1];
  char *filename = argv[2];
  int tam_pagina = atoi(argv[3]);
  int tam_mem_fisica = atoi(argv[4]);

  num_paginas = tam_mem_fisica / tam_pagina;

  s = 13;
  if(tam_pagina == 8) 
    s += 0;
  else if(tam_pagina == 16)
    s += 1;
  else if(tam_pagina == 32)
    s += 2;
  else {
    printf("Tamanho de Página %d é inválido\n", tam_pagina);
    exit(1);
  }

  arquivo_log = fopen(filename, "r");
  if(arquivo_log == NULL) {
    printf("Erro ao abrir arquivo %s\n", filename);
    exit(1);
  }

  printf("\nExecutando o simulador...\n");
  printf("Arquivo de entrada: %s\n", filename);
  printf("Tamanho da memoria fisica: %d\n", tam_mem_fisica);
  printf("Tamanho das páginas: %d\n", tam_pagina);
  printf("Alg de substituição: %s\n", algoritmo);

  unsigned addrs; char rw;
  while (fscanf(arquivo_log, "%x %c\n", &addrs, &rw) != -1) {
    int end_pagina = addrs >> s;
    paginaLida(end_pagina, rw);
    conta_tempo++;
  }

  printf("Numero de Faltas de Páginas: %d\n", page_faults);
  printf("Numero de Paginas escritas: %d\n", paginas_sujas);
}
