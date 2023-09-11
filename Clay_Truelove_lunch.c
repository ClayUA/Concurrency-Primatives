#include <stdio.h>
#include <pthread.h>
#include "mytime.h"
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
//Clay Truelove
//11695253



int i = 0;
int k = 0;
int j = 0;
int c = 0;


#define max 50




int loops;
int tick_count = 0;
int tb[max];
int add = 0;
int use = 1;


sem_t empty;
sem_t fill;
sem_t mutex;
sem_t skempty;
sem_t trill;

// create struct for each customer

typedef struct lunch{
    long long int TID;
    int Ticket_Num;
    pthread_mutex_t lock;

} lunch;


struct lunch *buffer;

void NowServing(struct lunch *lunch){
    printf("Now Serving %d" ,lunch->Ticket_Num);
   
}
void lunch_init(struct lunch *lunch){
    lunch->TID = 0;
    lunch->Ticket_Num = 0;
    pthread_mutex_init(&lunch->lock,NULL);
}

int lunch_get_ticket(struct lunch *lunch){
    
    //use semaphores so threads do not run at the same time

    sem_wait(&empty);
    sem_wait(&mutex);
    
    long long int hello = pthread_self();
    lunch->TID = hello;
    printf("\n%llu enters lunch_get_ticket\n",hello);
    tick_count++;
    lunch->Ticket_Num = tick_count;

    printf("%llu gets ticket %d\n",hello,tick_count);
    
    printf("%llu leave function lunch_get_ticket\n\n",hello);
    pthread_mutex_unlock(&lunch->lock);
    sem_post(&mutex);
    sem_post(&fill);
    int sleep_t = mytime(0,8);
    sleep(sleep_t);
     printf("\n%llu sleeps for %d seconds\n",lunch->TID,sleep_t);

    return lunch->Ticket_Num;
    
    
}


void lunch_wait_turn(struct lunch *lunch){
    sem_wait(&empty);
    sem_wait(&mutex);
    NowServing(lunch);
    printf("\n%llu enter lunch_wait_turn with ticket %d\n",lunch->TID,lunch->Ticket_Num);
    printf("%llu leave lunch_wait_turn after ticket %d served\n\n",lunch->TID,lunch->Ticket_Num);
    pthread_mutex_lock(&lunch->lock);
    sem_post(&mutex);
    sem_post(&fill);
    return;
}
void lunch_wait_customers(struct lunch *lunch){
    sem_wait(&fill);
    sem_wait(&mutex);
    long long int sir = pthread_self();
    printf("\n%llu enter lunch_waiting_customers\n",sir);
    printf("%llu after served ticket %d\n",sir,lunch->Ticket_Num);
    printf("%llu leave lunch_wait_customers\n\n",sir);
    use++;
    pthread_mutex_unlock(&lunch->lock);
    sem_post(&mutex);
    sem_post(&empty);

}
void *server(void *args){
    lunch *hee = (lunch *)args;
    lunch_wait_customers(hee);
}
void *customer(void *args){
    lunch * buffer = (lunch * )args;
    lunch_init(buffer);
    lunch_get_ticket(buffer);
    lunch_wait_turn(buffer);

}







int main(int argc, char *argv[]){
    if(atoi(argv[1]) != atoi(argv[2]))
    {
        printf("incorrect input %d %d",atoi(argv[1]),atoi(argv[2]));
        exit(0);
    }
    int total = atoi(argv[2]);
    loops = atoi(argv[2]);
    int iterable = 0;
    int iterable_2 = 0;
    pthread_t consumer[total];
    pthread_t producer[total];
    buffer = (lunch *)malloc((total * 2) * sizeof(struct lunch));
   
    printf("main begin and total = %d\n",total);
    sem_init(&empty,0,total); //wait decrements
    sem_init(&fill,0,0);    //post increases
    sem_init(&mutex,0,1);
   
   
    //create threads fro customers and servers

    for(i = 0; i < total; i++){
        pthread_create(&consumer[i], NULL ,customer, buffer + iterable);
        iterable++;
    }
    for(k = 0; k < total; k++){
        pthread_create(&producer[k],NULL,server, buffer + iterable_2);
        iterable_2++;
    }




    for(j = 0; j < total; j++){
        pthread_join(consumer[j],NULL);
    }
     for(c = 0; c < total; c++){
        pthread_join(producer[c],NULL);
    }
    int m;
    printf("\n");
    
    printf("\nmain end");


    return 0;
}