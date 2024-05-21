#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "potok_WEJSCIOWY"
typedef struct {
    int min;
    int max;
} Task;

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void find_primes_range(Task task, int write_pipe) {
    for (int i = task.min; i <= task.max; ++i) {
        if (is_prime(i)) {
            if (write(write_pipe, &i, sizeof(int)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }
    close(write_pipe);
}

void create_processes(Task task, int num_processes) {
    int pipes[num_processes][2];
    for (int i = 0; i < num_processes; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_processes; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            close(pipes[i][0]);

            int range = (task.max - task.min + 1) / num_processes;
            Task subtask;
            subtask.min = task.min + i * range;
            subtask.max = (i == num_processes - 1) ? task.max : (subtask.min + range - 1);

            find_primes_range(subtask, pipes[i][1]);

            exit(EXIT_SUCCESS);
        }
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_processes; ++i) {
        wait(NULL);
    }

    bool* primes_present = calloc(task.max - task.min + 1, sizeof(bool));
    if (primes_present == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    printf("Liczby pierwsze w zakresie [%d;%d]: ", task.min, task.max);
    for (int i = 0; i < num_processes; ++i) {
        int prime;
        while (read(pipes[i][0], &prime, sizeof(int)) > 0) {
            if (!primes_present[prime - task.min]) {
                printf("%d ", prime);
                primes_present[prime - task.min] = true;
            }
        }
        close(pipes[i][0]);
    }
    printf("\n");

    free(primes_present);
}

void process_commands(int fd) {
    char buffer[128];
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        struct timeval timeout = {1, 0};

        int ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
        if (ret == -1) {
            perror("select");
            close(fd);
            exit(EXIT_FAILURE);
        } else if (ret == 0) {
            continue;
        }

        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                perror("read");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == 0) {
            close(fd);
            fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            continue;
        }

        buffer[bytes_read] = '\0';

        if (strncmp(buffer, "koniec", 6) == 0) {
            break;
        }

        if (strncmp(buffer, "szukaj", 6) == 0) {
            Task task;
            int num_processes;
            sscanf(buffer, "szukaj [%d;%d] %d", &task.min, &task.max, &num_processes);
            create_processes(task, num_processes);
        }
    }
}

int main() {
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    int fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    process_commands(fd);
    close(fd);
    unlink(FIFO_NAME);

    return 0;
}
