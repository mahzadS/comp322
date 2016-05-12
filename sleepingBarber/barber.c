#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sem.h"

Semaphore customers; // 0
Semaphore barber; // 0
// mutex
Semaphore access_seats; // 1
int number_of_free_seats = 25;

int total_customers = 5;

// Prototypes
void barber_process();
void customer_process();

int main()
{
  sem_init(&customers, 0);
  sem_init(&barber, 0);
  sem_init(&access_seats, 1);

  // Create the customers
  int i;
  for (i = 0; i < total_customers; i++) {
    if(fork() == 0) {
      printf("Fork!\n");
      customer_process(i);
      exit(0);
    }
  }
  barber_process();
  return 0;
}

void barber_process()
{
  // runs in an infinite loop
  for (;;) {
    // tries to acquire a customer
    printf("Barber is ready for customer.\n");
    sem_wait(&customers);
    // modify the number of available seats
    printf("Barder is trying to access seats\n");
    sem_wait(&access_seats);
    // one chair gets free
    number_of_free_seats++;
    // the barber is ready to cut
    printf("Barber is ready to cut.\n");
    sem_signal(&barber);
    // we don't need the lock on the chairs
    sem_signal(&access_seats);
    // here the barber is cutting hair
    printf("Barber is cutting hair");
  }
}

void customer_process(int number)
{
  // runs in an infinite loop
  for (;;) {
    // tries to get access to the chairs
    printf("[PID %d] Customer %d: Trying to access seat.\n",
	   getpid(), number);
    sem_wait(&access_seats);
    printf("[PID %d] Customer %d: Checking if any free seats.\n",
	   getpid(), number);
    // if there are any free seats
    if (number_of_free_seats > 0) {
      // sitting down on a chair
      printf("Customer %d sat down in waiting room. [PID: %d]\n",
	     number, getpid());
      number_of_free_seats--;
      // notifiy the barber, about a customer
      sem_signal(&customers);
      // don't need to lock the chairs anymore
      sem_signal(&access_seats);
      // customer wait if the barber is busy
      sem_wait(&barber);
    }
    // there are no free seats
    else {
      // but don't forget to release the seats
      sem_signal(&access_seats);
      // customer leaves without haircut
      printf("Customer %d left (no free seats).\n", number);
    }
  }
}
