#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

// Funkcja sprawdzająca czy liczba jest pierwsza
int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Funkcja wykonywana przez pod-procesy
void* find_primes(void* arg) {
    int* range = (int*)arg;
    int min = range[0];
    int max = range[1];
    int* primes = (int*)malloc((max - min + 1) * sizeof(int));
    int prime_count = 0;
    for (int i = min; i <= max; i++) {
        if (is_prime(i)) {
            primes[prime_count++] = i;
        }
    }
    int* result = (int*)malloc((prime_count + 1) * sizeof(int));
    result[0] = prime_count;
    for (int i = 0; i < prime_count; i++) {
        result[i + 1] = primes[i];
    }
    free(primes);
    return (void*)result;
}

int main() {
    int fd;
    pid_t pid;
    char* fifo = "potok_WEJSCIOWY";

    mkfifo(fifo, 0666);

    fd = open(fifo, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int task[3];
        ssize_t bytes_read = read(fd, task, sizeof(task));
        if (bytes_read == 0) {
            printf("No more tasks, exiting...\n");
            break;
        }
        if (bytes_read < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (task[0] == -1) {
            printf("Received termination command, exiting...\n");
            break;
        }

        int min = task[0];
        int max = task[1];
        int num_processes = task[2];

        int range_size = (max - min + 1) / num_processes;
        int remainder = (max - min + 1) % num_processes;

        pid_t pids[num_processes];

        for (int i = 0; i < num_processes; i++) {
            int sub_range[2];
            sub_range[0] = min + i * range_size;
            sub_range[1] = min + (i + 1) * range_size - 1;
            if (i == num_processes - 1) {
                sub_range[1] += remainder;
            }

            pid = fork();
            if (pid == 0) { // child process
                int* result = (int*)find_primes((void*)sub_range);
                write(fd, result, (result[0] + 1) * sizeof(int));
                free(result);
                exit(EXIT_SUCCESS);
            }
            else if (pid > 0) { // parent process
                pids[i] = pid;
            }
            else { // fork failed
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < num_processes; i++) {
            waitpid(pids[i], NULL, 0);
        }
    }

    close(fd);
    unlink(fifo);
    return 0;
}
