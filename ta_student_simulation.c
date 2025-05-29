#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* ------------------- Constants ------------------- */
#define MAX_STUDENTS 25   // Total number of students
#define MAX_CHAIRS   5    // Waiting room capacity

/* ----------- Global synchronization vars ---------- */
sem_t students_sem;           // Signals student arrival
sem_t ta_sem;                 // TA signals session completion
pthread_mutex_t chairs_mutex; // Mutex for chair count

int waiting_chairs = 0;       // Current number of waiting students

/* ------------------- TA Thread -------------------- */
void *ta_thread(void *arg) {
    (void)arg;

    while (1) {
        sem_wait(&students_sem); // Wait for a student

        pthread_mutex_lock(&chairs_mutex);
        waiting_chairs--;
        printf("TA called a student | Free chairs: %d\n", MAX_CHAIRS - waiting_chairs);
        pthread_mutex_unlock(&chairs_mutex);

        printf("TA is helping a student...\n");
        sleep(3); // Simulate helping time

        sem_post(&ta_sem); // Notify student
    }

    return NULL;
}

/* ---------------- Student Thread ------------------ */
void *student_thread(void *arg) {
    int id = *((int *)arg);
    free(arg);

    pthread_mutex_lock(&chairs_mutex);

    if (waiting_chairs < MAX_CHAIRS) {
        waiting_chairs++;
        printf("Student %d sat down | In queue: %d\n", id, waiting_chairs);
        pthread_mutex_unlock(&chairs_mutex);

        sem_post(&students_sem);  // Notify TA
        sem_wait(&ta_sem);        // Wait for help

        printf("Student %d was helped and left.\n", id);
    } else {
        printf("Student %d left – no free chair.\n", id);
        pthread_mutex_unlock(&chairs_mutex);
    }

    return NULL;
}

/* -------------------- Main ------------------------ */
int main(void) {
    pthread_t ta;
    pthread_t students[MAX_STUDENTS];

    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&chairs_mutex, NULL);

    pthread_create(&ta, NULL, ta_thread, NULL);

    for (int i = 0; i < MAX_STUDENTS; ++i) {
        int *id = malloc(sizeof(int));
        *id = i + 1;

        sleep(rand() % 3); // Simulate random arrival
        pthread_create(&students[i], NULL, student_thread, id);
    }

    for (int i = 0; i < MAX_STUDENTS; ++i) {
        pthread_join(students[i], NULL);
    }

    pthread_cancel(ta);
    pthread_join(ta, NULL);

    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&chairs_mutex);

    return 0;
}