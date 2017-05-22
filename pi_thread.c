#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const unsigned long int NT = 8; //Nro de threads
//const unsigned long int NI = 125000000; //Nro de interacoes por thread
const unsigned long int NI = 1000000000; //Nro de interacoes por thread

typedef struct param
{
    int id_thread;
    long double partial_pi;
} param;

void *calc_pi(void *arg)
{
    register param *parameter = ((param *)arg);

    unsigned long int start = parameter->id_thread * NI;
    unsigned long int end = parameter->id_thread * NI + NI , i;

    printf("Start da %d: %ld\tEnd: %ld\n", parameter->id_thread, start, end);

    for (i = start; i < end; i += 2) // 4x posit
        parameter->partial_pi += (long double)4 / (2 * (i) + 1);

    for (i = start + 1; i < end; i += 2) // 4x negat
        parameter->partial_pi -= (long double)4 / (2 * (i) + 1);

    return NULL;
}

int main()
{
    int i;
    long double pi = 0;

    pthread_t *threads = (pthread_t *)malloc(NT * sizeof(pthread_t));
    register param *parameters = (param *)malloc(NT * sizeof(param));

    for (i = 0; i < NT; i++)
    {
        parameters[i].partial_pi = 0;
        parameters[i].id_thread = i;
        pthread_create(&threads[i], NULL, &calc_pi, &parameters[i]);
    }

    for (i = 0; i < NT; i++)
    {
        pthread_join(threads[i], NULL);
        pi += parameters[i].partial_pi;
    }
    printf("%.50Lf\n", pi);
    return 0;
}
