#include <stdio.h>
#include <windows.h>

#define MAX_PROCESSES 10

typedef struct {
    DWORD pid;
    HANDLE handle;
} ProcessInfo;

ProcessInfo processes[MAX_PROCESSES];
int num_processes = 0;

void addProcess(DWORD pid, HANDLE handle) {
    processes[num_processes].pid = pid;
    processes[num_processes].handle = handle;
    num_processes++;
}

void stopProcess(int processIndex) {
    if (processIndex < 0 || processIndex >= num_processes) {
        printf("Invalid process index.\n");
        return;
    }

    if (!TerminateProcess(processes[processIndex].handle, 0)) {
        printf("Failed to terminate process. Error code: %d\n", GetLastError());
        return;
    }

    CloseHandle(processes[processIndex].handle);
    printf("Process terminated successfully.\n");

    for (int i = processIndex; i < num_processes - 1; i++) {
        processes[i] = processes[i + 1];
    }
    num_processes--;
}


int main() {
    printf("ProgramGlowny - Start\n");

    while (1) {
        for (int i = 0; i < num_processes; i++) {
            for (int i = 0; i < num_processes; i++) {
                DWORD exitCode;
                if (!GetExitCodeProcess(processes[i].handle, &exitCode)) {
                    printf("Failed to get exit code for process %d. Error code: %d\n", processes[i].pid, GetLastError());
                    continue;
                }
                if (exitCode != STILL_ACTIVE) {
                    // Proces jest już zakończony, usuwamy go z tablicy
                    CloseHandle(processes[i].handle);
                    for (int j = i; j < num_processes - 1; j++) {
                        processes[j] = processes[j + 1];
                    }
                    num_processes--;
                    i--; // Spowoduje to ponowne sprawdzenie procesu na tej samej pozycji
                }
            }
        }
        char command[50];
        printf("Enter command (1: Create process, 2: Stop process, 3: List processes, 4: Exit): ");
        fgets(command, sizeof(command), stdin);

        int choice = atoi(command);

        switch (choice) {
        case 1: {
            char exe_path[] = "ProgramPodrzedny.exe";
            STARTUPINFO structStartupInfo;
            PROCESS_INFORMATION structProcInfo;

            GetStartupInfo(&structStartupInfo);

            if (!CreateProcess(NULL, exe_path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &structStartupInfo, &structProcInfo)) {
                printf("CreateProcess failed (%d).\n", GetLastError());
                break;
            }

            addProcess(structProcInfo.dwProcessId, structProcInfo.hProcess);
            break;
        }
        case 2: {
            printf("Enter the index of the process to stop: ");
            fgets(command, sizeof(command), stdin);
            int processIndex = atoi(command) - 1;
            stopProcess(processIndex);
            break;
        }
        case 3: {
            printf("List of running processes:\n");
            for (int i = 0; i < num_processes; i++) {
                printf("%d. PID: %d\n", i + 1, processes[i].pid);
            }
            break;
        }
        case 4: {
            for (int i = 0; i < num_processes; i++) {
                stopProcess(i);
            }
            printf("ProgramGlowny - Finish\n");
            return 0;
        }
        default:
            printf("Invalid command.\n");
        }
    }

    return 0;
}
