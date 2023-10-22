#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 4

int request = 0;
int respond = 0;
int SOMA = 0;

int CONTROLE_SECAO_CRITICA;

void soma(int i) {
  int local = SOMA;
  sleep(rand() % 2);
  SOMA = local + 1;
  printf("Thread cliente (%d) incrementa, valor atual da SOMA: %d\n", i, SOMA);
}

void *client(void *arg) {
  int i = *(int *)arg;
  while (1) {
    /* Se a condicional for verdadeira, ocorre o controle da Seção Crítica. Caso
     * contrário não haverá controle Seção Crítica e a função soma() é
     * executada.*/
    if (CONTROLE_SECAO_CRITICA) {
      while (respond != i) { // cliente espera pela sua resposta
        sleep(1);
        request = i;
      }
    }
    soma(i); // seçao critica
    respond = 0;
  }
  pthread_exit(NULL);
}

void *server() {
  while (1) {
    while (request == 0) {
      sleep(1);
    } // espera uma requisicao
    respond = request;
    // printf("Resposta do server: %d\n", respond);
    while (respond != 0) {
      sleep(1);
    } // espera cliente sair da seçao critica
    request = 0;
  }
  pthread_exit(NULL);
}

int main(void) {
  printf("Digite 0 ou 1 para definir o controle da secao crítica:\n");
  scanf("%d", &CONTROLE_SECAO_CRITICA);
  if (CONTROLE_SECAO_CRITICA < 0 || CONTROLE_SECAO_CRITICA > 1) {
    return -1;
  }

  pthread_t thr[NUM_THREADS + 1];
  pthread_create(&thr[0], NULL, server, NULL);

  for (int i = 1; i <= NUM_THREADS; i++) {
    int *arg = (int *)malloc(sizeof(int));
    *arg = i;
    pthread_create(&thr[i], NULL, client, (void *)arg);
  }

  for (int i = 0; i <= NUM_THREADS; i++) {
    pthread_join(thr[i], NULL);
  }

  return 0;
}