// Server.c

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 7777
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

CRITICAL_SECTION cs;
FILE* sharedFile;
BOOL serverRunning = TRUE;


void closeServer(SOCKET client) {
    fprintf(sharedFile, "Shutdown request received from client %d.\n", (int)client);
    fflush(sharedFile);

    const char* response = "Server shutdown initiated.\n";
    printf(response);
    Sleep(2000);
    exit(0);
}


void handleClient(void* socket) {
    SOCKET clientSocket = *(SOCKET*)socket;
    char buffer[BUFFER_SIZE];

    while (serverRunning) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR) {
            fprintf(stderr, "recv() failed for client %d: %d\n", (int)clientSocket, WSAGetLastError());
            closesocket(clientSocket);
            return;
        }

        if (bytesReceived == 0) {
            printf("Client %d disconnected\n", (int)clientSocket);
            closesocket(clientSocket);
            return;
        }

        buffer[bytesReceived] = '\0';
        if (!strncmp(buffer, "close_server", 12)) {
            closeServer(clientSocket);
            return;
        }
        int n = atoi(buffer);

        int factorial = 1;
        for (int i = 1; i <= n; i++) {
            factorial *= i;

            EnterCriticalSection(&cs);
            fprintf(sharedFile, "Client %d: Partial result for n=%d: %d\n", (int)clientSocket, i, factorial);
            Sleep(1000);
            fflush(sharedFile);
            LeaveCriticalSection(&cs);
        }

        sprintf(buffer, "%d", factorial);
        send(clientSocket, buffer, strlen(buffer), 0);
    }
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    DWORD threadId;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    sharedFile = fopen("shared.txt", "w");
    if (sharedFile == NULL) {
        fprintf(stderr, "Failed to open shared file\n");
        return 1;
    }

    InitializeCriticalSection(&cs);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed: %d\n", WSAGetLastError());
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind() failed: %d\n", WSAGetLastError());
        return 1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR) {
        fprintf(stderr, "listen() failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (serverRunning) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            fprintf(stderr, "accept() failed: %d\n", WSAGetLastError());
            continue;
        }

        printf("Client %d connected\n", (int)clientSocket);

        HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, &clientSocket, 0, &threadId);
        if (threadHandle == NULL) {
            fprintf(stderr, "CreateThread() failed: %d\n", (int)GetLastError());
            closesocket(clientSocket);
        }
        else {
            CloseHandle(threadHandle);
        }
    }

    DeleteCriticalSection(&cs);
    fclose(sharedFile);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
