#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#define PHILOSOPHERS_NUM 5

sem_t chopstick[PHILOSOPHERS_NUM];
pthread_t thread_id[PHILOSOPHERS_NUM];
int philosopher_id[PHILOSOPHERS_NUM]; 
sem_t gate;



/* turn the action of picking chopsticks to an atomic opeartion */
void* philosopher(void* philosopher_index) 
{
    int philosopher_index0 = *(int*)philosopher_index;
    printf("Philosopher %d seat in.\n", philosopher_index0);
    while (1) { 
//      if (philosopher_index0 % 2 == 0) /* philosophers at even number position */
  //    {
       printf("before enter gate = %d\n", gate);
       sem_wait(&gate); /* close the gate : only one philosopher is allowed to get chopsticks at the moment. */
       printf("Philosopher %d enter the gate = %d.\n", philosopher_index0, gate);

       sem_wait(&chopstick[ (philosopher_index0+1) % 5] );  /* pick the right hand side chopstick */
       sem_wait(&chopstick[philosopher_index0]);            /* pick the left hand side chopstick */
        

       sem_post(&chopstick[philosopher_index0]);  
       sem_post(&chopstick[ (philosopher_index0+1) % 5] );

       printf("Philosopher %d have picked chopsticks and got the rice.\n", philosopher_index0);
       sem_post(&gate);  /* open the gate */
       printf("Philosopher %d leave the gate.\n", philosopher_index0);
//      }
     // else  /* philosophers at odd number position */ 
     // {
     //  sem_wait(&gate); /* close the gate : only one philosopher is allowed to get chopsticks at the moment. */

//       sem_wait(&chopstick[philosopher_index0]);             /* pick the left hand side chopstick */
//       sem_wait(&chopstick[ (philosopher_index0+1) % 5] );   /* pick the right hand side chopstick */

//       sem_post(&chopstick[ (philosopher_index0+1) % 5] );
 //      sem_post(&chopstick[philosopher_index0]);  

//       sem_post(&gate);  /* open the gate */
//       printf("Philosopher %d have picked chopsticks and got the rice.\n", philosopher_index0);

//      }
    }
    printf("Philosopher %d is leaving\n", philosopher_index0);
}


void init_semaphore() {

    if (-1 == sem_init(&gate, 0, 1)) 
    {
        printf("semaphore init error\n");
        return;
    }
    printf("gate initialized, currently gate = %d\n", gate);
    for (int i=0; i<PHILOSOPHERS_NUM; i++) 
    {

      if (-1 == sem_init(&chopstick[i], 0, 1)) 
      {
          printf("semaphore init error\n");
          return;
      }

    }
}

/************************************************* 
  structure description : 
    
    Philosphers will be placed at odd number positions.
    The even number posistions are for chopsticks.
    
    In the following, 1 = a philospher, 0 = chopstick

    arr is a circular array represented a dinner table.
    array. 

           chopstick 
               |
  arr : [0, 1, 0, 1, 0, 1, 0, 1, 0, 1]
         ^              |           ^
         |______________|___________|
                        |
                    philosohper
            a     b     c     d     e  

  return : None
  Exception handling : None
**************************************************/

void philosophers_sit_down() {


  for (int i=0; i<PHILOSOPHERS_NUM; i++) 
  {
    philosopher_id[i] = i;
    pthread_create(&thread_id[i],NULL, philosopher, (void*)&philosopher_id[i]);
  }
  printf("5 philosophers created !\n\n");

}


int main() {

  init_semaphore();
  printf("semaphore init finished\n");
  philosophers_sit_down();
  for (int i=0; i<PHILOSOPHERS_NUM; i++) 
  {
    pthread_join(&thread_id[i],NULL);
  }
    
}