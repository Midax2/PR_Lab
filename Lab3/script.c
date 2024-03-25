#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void finish(int pid) {
    printf("Process %d over\n", pid);
}

void run_command(char *command, char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(command, args);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // Parent process
        printf("Process %d has started\n", pid);
        waitpid(pid, NULL, 0);
        finish(pid);
    } else {
        // Error
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main() {
    char *commands[] = {"curl", "ping", "sleep", "df"};
    char *args[][3] = {{"curl", "-O", "http://speedtest.tele2.net/1GB.zip"},
                       {"ping", "-c", "10", "example.com"},
                       {"sleep", "30", NULL},
                       {"df", "-h", NULL}};
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < num_commands; ++i) {
        run_command(commands[i], args[i]);
    }

    printf("Finish!\n");

    return 0;
}
