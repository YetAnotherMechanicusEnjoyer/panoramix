#include <stdio.h>
#include <stdlib.h>
#include "panoramix.h"

void run(shared_data_t* data) {
  pthread_t druid_thread;
  pthread_create(&druid_thread, NULL, druid_routine, data);

  pthread_t* villager_threads = malloc(sizeof(pthread_t) * data->nb_villagers);
  villager_t* villagers = malloc(sizeof(villager_t) * data->nb_villagers);

  for (int i = 0; i < data->nb_villagers; i++) {
    villagers[i].id = i;
    villagers[i].data = data;
    pthread_create(&villager_threads[i], NULL, villager_routine, &villagers[i]);
  }

  for (int i = 0; i < data->nb_villagers; i++) {
    pthread_join(villager_threads[i], NULL);
  }

  pthread_mutex_lock(&data->mutex);
  data->all_done = 1;
  sem_post(&data->druid_wakeup);
  pthread_mutex_unlock(&data->mutex);

  pthread_join(druid_thread, NULL);

  free(villager_threads);
  free(villagers);
  sem_destroy(&data->druid_wakeup);
  sem_destroy(&data->pot_refilled);
  pthread_mutex_destroy(&data->mutex);
}

int main(const int argc, const char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, "USAGE: ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n");
    return ERROR;
  }

  shared_data_t data = {
    .nb_villagers = atoi(argv[1]),
    .pot_size = atoi(argv[2]),
    .nb_fights = atoi(argv[3]),
    .nb_refills = atoi(argv[4]),
    .servings_left = data.pot_size,
    .waiters = 0,
    .all_done = 0,
  };

  if (data.nb_villagers <= 0 || data.pot_size <= 0 || data.nb_fights <= 0 || data.nb_refills <= 0) {
    fprintf(stderr, "USAGE: ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n");
    fprintf(stderr, "Values must be >0.\n");
    return ERROR;
  }

  pthread_mutex_init(&data.mutex, NULL);
  sem_init(&data.druid_wakeup, 0, 0);
  sem_init(&data.pot_refilled, 0, 0);

  run(&data);

  return SUCCESS;
}
