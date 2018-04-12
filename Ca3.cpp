
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include<string.h>

long circle = 0;
long number_per_thread;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *run()
{
    long circle_thread = 0;

    unsigned int rand_state = rand();
    long i;
    for (i = 0; i < number_per_thread; i++) 
	{
        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

        if (x * x + y * y < 1)
		{
            circle_thread++;
        }
    }
    
    pthread_mutex_lock(&mutex);
    circle += circle_thread;
    pthread_mutex_unlock(&mutex);
}

int main(int num, const char *argc[])
{
    if (num != 3) 
	{
        fprintf(stderr, "usage:./pi <total points> <threads>\n");
        exit(1);
    }

    long totalpoints = atol(argc[1]);
    int thread_count = atoi(argc[2]);
    number_per_thread = totalpoints / thread_count;

    
    time_t start = time(NULL);

    srand((unsigned)time(NULL));

    pthread_t *threads = malloc(thread_count * sizeof(pthread_t)
        pthread_attr_t attr;

    pthread_attr_init(&attr);

    int i;
    for (i = 0; i < thread_count; i++)
	{
        pthread_create(&threads[i], &attr, run, (void *) NULL);
    }

    for (i = 0; i < thread_count; i++) 
	{
        pthread_join(threads[i], NULL);
    }	
    pthread_mutex_destroy(&mutex);
    free(threads);

    printf("Pi: %f\n", (4. * (double)circle) / ((double)number_per_thread * thread_count));
    printf("Time: %d sec\n", (unsigned int)(time(NULL) - start));

    return 0;
}
