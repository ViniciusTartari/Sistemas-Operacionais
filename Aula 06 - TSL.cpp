#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


void enter_region() {
    asm(
        ".data\n\t"
        "lock:\n\t"
        ".byte 0\n\t"
        ".text\n\t"

        "_enter_region:\n\t"
        "movb $1, %al\n\t" /* move 1 to AL */
        "xchgb (lock),%al\n\t" 
        "cmp $0, %al\n\t"
        "jne _enter_region\n\t"

    );
}

void leave_region() {
    asm("movb $0, (lock)");
}


void critical_region() {

}

void noncritical_region() {

}

static void* f1(void* p) {
    while(1) {
        puts("wait for f2");
        enter_region();
        printf("f1 can start its critical section\n");
        critical_region();
        leave_region();
        noncritical_region();
        sleep(1);
    }
    return NULL;
}

static void* f2(void* p) {
    while(1) {
        puts("wait for f1");
        enter_region();
        printf("f2 can start its critical section\n");
        critical_region();
        leave_region();

        /* if you call sleep, you can see that the non-critical section of this thread won't
        *  block the other thread from running its critical section as many times as it wants
        */
        sleep(4);
        noncritical_region();
    }
    return NULL;
}

int main() {
    int rc;

    pthread_t t1, t2;

    rc = pthread_create(&t1, NULL, f1, NULL);
    if(rc != 0) {
        fprintf(stderr, "pthread f1 failed\n");
        return EXIT_FAILURE;
    }

    rc = pthread_create(&t2, NULL, f2, NULL);
    if(rc != 0) {
        fprintf(stderr, "pthread f2 failed\n");
        return EXIT_FAILURE;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    puts("All threads finished.");
    return 0;
}
