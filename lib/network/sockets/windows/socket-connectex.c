#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 12345
#define CONNECT_TIMEOUT 5000 // Timeout de 5000 ms (5 segundos)

// Função para inicializar o Winsock e criar um socket
SOCKET CreateSocket() {
    WSADATA wsaData;
    SOCKET  sock = INVALID_SOCKET;

    // Inicializa o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup falhou.\n");
        return INVALID_SOCKET;
    }

    // Cria um socket overlapped
    sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET) {
        printf("Falha ao criar o socket.\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    return sock;
}

// Função para obter o ponteiro para a função ConnectEx
LPFN_CONNECTEX GetConnectExFunction(SOCKET sock) {
    GUID guidConnectEx = WSAID_CONNECTEX;
    LPFN_CONNECTEX lpConnectEx = NULL;
    DWORD bytes = 0;

    if (WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
                 &guidConnectEx, sizeof(guidConnectEx),
                 &lpConnectEx, sizeof(lpConnectEx),
                 &bytes, NULL, NULL) == SOCKET_ERROR) {
        printf("WSAIoctl falhou ao obter o ponteiro para ConnectEx.\n");
        closesocket(sock);
        WSACleanup();
        return NULL;
    }

    return lpConnectEx;
}

int main() {
    SOCKET  clientSocket;
    struct sockaddr_in serverAddr;
    LPFN_CONNECTEX lpConnectEx;
    OVERLAPPED ol = { 0 };
    HANDLE hIOCP;
    DWORD bytesTransferred;
    ULONG_PTR completionKey;
    LPOVERLAPPED pOverlapped;
    BOOL result;
    DWORD flags = 0;

    // Cria o socket
    clientSocket = CreateSocket();
    if (clientSocket == INVALID_SOCKET) {
        return 1;
    }

    // Obtém o ponteiro para a função ConnectEx
    lpConnectEx = GetConnectExFunction(clientSocket);
    if (lpConnectEx == NULL) {
        return 1;
    }

    // Cria um IOCP e associa o socket a ele
    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hIOCP == NULL) {
        printf("Falha ao criar o IOCP.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, 0, 0) == NULL) {
        printf("Falha ao associar o socket ao IOCP.\n");
        CloseHandle(hIOCP);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Define o endereço do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    // Inicia a conexão assíncrona
    result = lpConnectEx(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr), NULL, 0, NULL, &ol);
    if (result == FALSE) {
        if (WSAGetLastError() != WSA_IO_PENDING) {
            printf("ConnectEx falhou.\n");
            CloseHandle(hIOCP);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    // Aguarda a conclusão da conexão ou timeout
    result = GetQueuedCompletionStatus(hIOCP, &bytesTransferred, &completionKey, &pOverlapped, CONNECT_TIMEOUT);
    if (result == FALSE) {
        if (pOverlapped == NULL) {
            printf("Tempo de conexão esgotado.\n");
            // Cancela a operação de conexão
            CancelIo((HANDLE)clientSocket);
            CloseHandle(hIOCP);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        } else {
            printf("ConnectEx falhou durante a conclusão.\n");
            CloseHandle(hIOCP);
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    printf("Conexão estabelecida com sucesso.\n");

    // Aqui você pode continuar com operações assíncronas, como WSASend e WSARecv

    // Limpa os recursos
    CloseHandle(hIOCP);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
