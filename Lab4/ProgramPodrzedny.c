#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_THREADS 10

typedef struct {
    HANDLE handle;
    int priority;
} ThreadInfo;

ThreadInfo threads[MAX_THREADS];
int num_threads = 0;

DWORD WINAPI new_thread(LPVOID lpParam) {
    int n = *(int*)lpParam;
    int result = 1;
    
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    Sleep(60000);
	printf("Factorial of %d is %d\n", n, result);
    return 0;
}

int createThread(int priority) {
    if (num_threads >= MAX_THREADS) {
        printf("Maximum number of threads reached.\n");
        return 1;
    }
	int n;
    printf("Enter a number to calculate its factorial: ");
    scanf("%d", &n);
    HANDLE threadHandle;
    threadHandle = CreateThread(NULL, 0, new_thread, &n, 0, NULL);
    if (threadHandle == NULL) {
        printf("Failed to create thread. Error code: %d\n", GetLastError());
        return 1;
    }

    SetThreadPriority(threadHandle, priority);
    threads[num_threads].handle = threadHandle;
    threads[num_threads].priority = priority;
    num_threads++;
    printf("New thread created successfully.\n");
    return 0;
}

void removeThread(int threadIndex) {
    if (threadIndex < 0 || threadIndex >= num_threads) {
        printf("Invalid thread index.\n");
        return;
    }

    CloseHandle(threads[threadIndex].handle);
    for (int i = threadIndex; i < num_threads - 1; i++) {
        threads[i] = threads[i + 1];
    }
    num_threads--;
    printf("Thread removed successfully.\n");
}

void changePriority(int threadIndex, int newPriority) {
    if (threadIndex < 0 || threadIndex >= num_threads) {
        printf("Invalid thread index.\n");
        return;
    }

    if (!SetThreadPriority(threads[threadIndex].handle, newPriority)) {
        printf("Failed to change thread priority. Error code: %d\n", GetLastError());
        return;
    }

    threads[threadIndex].priority = newPriority;
    printf("Thread priority changed successfully.\n");
}

void listThreads() {
    printf("List of running threads:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("%d. Priority: %d\n", i + 1, threads[i].priority);
    }
}

void closeAllThreads() {
    for (int i = 0; i < num_threads; i++) {
        CloseHandle(threads[i].handle);
    }
    num_threads = 0;
    printf("All threads closed successfully.\n");
}

int main() {
    printf("ProgramPodrzedny - Start\n");

    while (1) {
        char command[50];
        printf("Enter command (a: Create thread, b: Remove thread, c: Change priority, d: List threads, e: Exit): ");
        fgets(command, sizeof(command), stdin);

        switch (command[0]) {
        case 'a': {
            int priority;
            printf("Enter thread priority (1-15): ");
            fgets(command, sizeof(command), stdin);
            priority = atoi(command);
            if (priority < 1 || priority > 15) {
                printf("Invalid priority.\n");
                break;
            }
            createThread(priority);
            break;
        }
        case 'b': {
            listThreads();
            printf("Enter the index of the thread to remove: ");
            fgets(command, sizeof(command), stdin);
            int threadIndex = atoi(command) - 1;
            removeThread(threadIndex);
            break;
        }
        case 'c': {
            listThreads();
            printf("Enter the index of the thread to change priority: ");
            fgets(command, sizeof(command), stdin);
            int threadIndex = atoi(command) - 1;
            printf("Enter new priority (1-15): ");
            fgets(command, sizeof(command), stdin);
            int newPriority = atoi(command);
            if (newPriority < 1 || newPriority > 15) {
                printf("Invalid priority.\n");
                break;
            }
            changePriority(threadIndex, newPriority);
            break;
        }
        case 'd': {
            listThreads();
            break;
        }
        case 'e': {
            closeAllThreads();
            printf("ProgramPodrzedny - Finish\n");
            return 0;
        }
        default:
            printf("Invalid command.\n");
        }
    }

    return 0;
}
