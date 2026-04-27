#ifndef PANORAMIX_H
#define PANORAMIX_H

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 84

typedef struct {
  int nb_villagers;
  int pot_size;
  int nb_fights;
  int nb_refills;
  int servings_left;
  int waiters;
  int all_done;
  pthread_mutex_t mutex;
  sem_t druid_wakeup;
  sem_t pot_refilled;
} shared_data_t;

typedef struct {
  int id;
  shared_data_t* data;
} villager_t;

void* druid_routine(void* arg);
void* villager_routine(void* arg);

#endif /* PANORAMIX_H */
