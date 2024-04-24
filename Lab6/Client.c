// Client.c

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 7777
#define BUFFER_SIZE 1024

void sendRequest(SOCKET clientSocket, const char* request) {
    send(clientSocket, request, strlen(request), 0);
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int calculationsInProgress = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "connect() failed\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    int choice;
    do {
        printf("Menu:\n");
        if (!calculationsInProgress) {
            printf("1. Calculate factorial\n");
        }
        printf("2. Close server\n");
        printf("3. Close client\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (!calculationsInProgress) {
                printf("Enter a number (n) to calculate factorial: ");
                int n;
                scanf("%d", &n);
                sprintf(buffer, "%d", n);
                sendRequest(clientSocket, buffer);
                calculationsInProgress = 1;

                int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                    printf("Server disconnected. Cannot perform a calculations\n");
                }

                buffer[bytesReceived] = '\0';
                printf("Factorial result: %s\n", buffer);

                calculationsInProgress = 0;
            }
            else {
                printf("Calculations are already in progress.\n");
            }
            break;
        case 2:
            sendRequest(clientSocket, "close_server");
            break;
        case 3:
        	printf("Client is disconnecting.\n");
        	Sleep(2000);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 3);
 	
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
