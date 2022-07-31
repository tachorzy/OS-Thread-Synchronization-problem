#include <pthread.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

static pthread_mutex_t bsem;
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;
static int turn;
static int nthreads;

void *print_in_increasing_order_even_odd(void *void_ptr_argv)
{
    int threadID = *((int*) void_ptr_argv); //getting the thread id from the void* arg
    pthread_mutex_lock(&bsem); //the mutex object (our binary semaphore) is locked

    while(threadID != turn) //while it is not a threads turn it will be stuck in this while loop, 
        pthread_cond_wait(&waitTurn, &bsem); //and blocks the calling thread until the condition variable (waitTurn) is signaled
    
    
    pthread_mutex_unlock(&bsem); //end of the first critical section, our binary semaphore is unlocked, all threads that are blocked become available
	std::cout << "I am Thread " << threadID << std::endl; 
    pthread_mutex_lock(&bsem); //start of second critical section, our semaphore is locked again
    turn += 2; 
    if(turn >= nthreads)
        turn = 1;
    pthread_cond_broadcast(&waitTurn); //wakes up all threads that are waiting on the condition variable, this will signal waitTurn
    pthread_mutex_unlock(&bsem); //end of second critical section, our semaphore is unlocked/released
    return NULL;
}

int main()
{
    std::cin >> nthreads;
    pthread_mutex_init(&bsem, NULL); // Initialize access to 1
 	pthread_t *tid= new pthread_t[nthreads];
	int *threadNumber=new int[nthreads];
    turn = 0;

	for(int i=0;i<nthreads;i++)
	{
        threadNumber[i] = i;
        pthread_create(&tid[i], NULL, print_in_increasing_order_even_odd, &threadNumber[i]);
	}
	
    for(int i = 0; i < nthreads; i++)
        pthread_join(tid[i], NULL);

    delete [] threadNumber;
    delete [] tid;
	return 0;
}