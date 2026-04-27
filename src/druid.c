#include <stdio.h>
#include "panoramix.h"

void* druid_routine(void* arg) {
  shared_data_t* data = (shared_data_t*)arg;

  printf("Druid: I'm ready... but sleepy...\n");

  while(1) {
    sem_wait(&data->druid_wakeup);

    pthread_mutex_lock(&data->mutex);

    if (data->all_done) {
      pthread_mutex_unlock(&data->mutex);
      break;
    }

    data->nb_refills--;
    data->servings_left = data->pot_size;
    printf("Druid: Ah! Yes, yes, I'm awake! Working on it! Beware I can only make %d more refills after this one.\n", data->nb_refills);

    for (int i = 0; i < data->waiters + 1; i++) {
      sem_post(&data->pot_refilled);
    }

    int out_of_viscum = (data->nb_refills == 0);
    pthread_mutex_unlock(&data->mutex);

    if (out_of_viscum) {
      printf("Druid: I'm out of viscum. I'm going back to... zZz\n");
      break;
    }
  }

  return NULL;
}
