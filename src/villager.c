#include <stdio.h>
#include "panoramix.h"

void* villager_routine(void* arg) {
  villager_t* v = (villager_t*)arg;
  shared_data_t* data = v->data;

  printf("Villager %d: Going into battle!\n", v->id);

  for (int i = data->nb_fights; i > 0; i--) {
    pthread_mutex_lock(&data->mutex);

    printf("Villager %d: I need a drink... I see %d servings left.\n", v->id, data->servings_left);

    if (data->servings_left == 0) {
      if (data->waiters == 0) {
        printf("Villager %d: Hey Pano wake up! We need more potion.\n", v->id);
        sem_post(&data->druid_wakeup);
      }

      while(data->servings_left == 0) {
        if (data->nb_refills == 0) {
          sem_post(&data->pot_refilled);
          pthread_mutex_unlock(&data->mutex);
          printf("Villager %d: I'm going to sleep now.\n", v->id);
          return NULL;
        }

        data->waiters++;
        pthread_mutex_unlock(&data->mutex);

        sem_wait(&data->pot_refilled);

        pthread_mutex_lock(&data->mutex);
        data->waiters--;
      }
    }

    data->servings_left--;
    pthread_mutex_unlock(&data->mutex);

    printf("Villager %d: Take that roman scum! Only %d left.\n", v->id, i - 1);
  }

  printf("Villager %d: I'm going to sleep now.\n", v->id);
  return NULL;
}
