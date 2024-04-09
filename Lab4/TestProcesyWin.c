#include <stdio.h>
#include <windows.h>

#define MAX_PROCESSES 10

typedef struct {
    DWORD pid;
    HANDLE handle;
    int priority;
} ProcessInfo;

ProcessInfo processes[MAX_PROCESSES];
int num_processes = 0;

void addProcess(DWORD pid, HANDLE handle, int priority) {
    processes[num_processes].pid = pid;
    processes[num_processes].handle = handle;
    processes[num_processes].priority = priority;
    num_processes++;
}

void stopProcess(int processIndex) {
    if (processIndex < 0 || processIndex >= num_processes) {
        printf("Invalid process index.\n");
        return;
    }

    if (!TerminateProcess(processes[processIndex].handle, 0)) {
        printf("Failed to terminate process. Error code: %d\n", (int)GetLastError());
        return;
    }

    CloseHandle(processes[processIndex].handle);
    printf("Process PID:%d terminated successfully.\n", (int)processes[processIndex].pid);

    for (int i = processIndex; i < num_processes - 1; i++) {
        processes[i] = processes[i + 1];
    }
    num_processes--;
}

void checkIfStillAlive() {
	for (int i = 0; i < num_processes; i++) {
            DWORD exitCode;
            if (!GetExitCodeProcess(processes[i].handle, &exitCode)) {
                printf("Failed to get exit code for process %d. Error code: %d\n", (int)processes[i].pid, (int)GetLastError());
                continue;
            }
            if (exitCode != STILL_ACTIVE) {
                CloseHandle(processes[i].handle);
				printf("Process PID:%d terminated successfully.\n", (int)processes[i].pid);
                for (int j = i; j < num_processes - 1; j++) {
                    processes[j] = processes[j + 1];
                }
                num_processes--;
                i--; 
            }
        }
}

int main() {
    printf("ProgramGlowny - Start\n");

    while (1) {
        char command[50];
        printf("Enter command (1: Create process, 2: Stop process, 3: List processes, 4: Exit): ");
        fgets(command, sizeof(command), stdin);

        int choice = atoi(command);

        switch (choice) {
            case 1: {
				checkIfStillAlive();
                char exe_path[] = "ProgramPodrzedny.exe";
                STARTUPINFO structStartupInfo;
                PROCESS_INFORMATION structProcInfo;

                GetStartupInfo(&structStartupInfo);

                printf("Enter priority for the new process (1-4): ");
                fgets(command, sizeof(command), stdin);
                int priority = atoi(command);

                if (priority < 1 || priority > 4) {
                    printf("Invalid priority. Please enter a value between 1 and 4.\n");
                    break;
                }

                DWORD priorityClass;
                switch (priority) {
                    case 1:
                        priorityClass = IDLE_PRIORITY_CLASS;
                        break;
                    case 2:
                        priorityClass = BELOW_NORMAL_PRIORITY_CLASS;
                        break;
                    case 3:
                        priorityClass = NORMAL_PRIORITY_CLASS;
                        break;
                    case 4:
                        priorityClass = ABOVE_NORMAL_PRIORITY_CLASS;
                        break;
                }

                if (!CreateProcess(NULL, exe_path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &structStartupInfo, &structProcInfo)) {
                    printf("CreateProcess failed (%d).\n", (int)GetLastError());
                    break;
                }

                if (!SetPriorityClass(structProcInfo.hProcess, priorityClass)) {
                    printf("Failed to set priority class for the process. Error code: %d\n", (int)GetLastError());
                    break;
                }

                addProcess(structProcInfo.dwProcessId, structProcInfo.hProcess, priority);
                break;
            }
            case 2: {
				checkIfStillAlive();
                printf("Enter the index of the process to stop: ");
                fgets(command, sizeof(command), stdin);
                int processIndex = atoi(command) - 1;
                stopProcess(processIndex);
                break;
            }
            case 3: {
				checkIfStillAlive();
                printf("List of running processes:\n");
                for (int i = 0; i < num_processes; i++) {
                    printf("%d. PID: %d, Priority: %d\n", i + 1, (int)processes[i].pid, processes[i].priority);
                }
                break;
            }
            case 4: {
				checkIfStillAlive();
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
