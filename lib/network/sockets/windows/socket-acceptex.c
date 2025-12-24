#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define SERVER_PORT 12345
#define ACCEPT_TIMEOUT 5000 // Timeout de 5000 ms (5 segundos)
#define ADDRESS_BUFFER_SIZE (sizeof(SOCKADDR_IN) + 16)

typedef struct {
    OVERLAPPED overlapped;
    SOCKET acceptSocket;
    char addressBuffer[2 * ADDRESS_BUFFER_SIZE];
} ACCEPT_CONTEXT;

// Função para inicializar o Winsock e criar um socket de escuta
SOCKET CreateListenSocket() {
    WSADATA wsaData;
    SOCKET listenSocket = INVALID_SOCKET;
    struct sockaddr_in serverAddr;

    // Inicializa o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup falhou.\n");
        return INVALID_SOCKET;
    }

    // Cria um socket overlapped
    listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket == INVALID_SOCKET) {
        printf("Falha ao criar o socket de escuta.\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Configura o endereço do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Associa o socket ao endereço e porta
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind falhou.\n");
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Coloca o socket em modo de escuta
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen falhou.\n");
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return listenSocket;
}

// Função para obter o ponteiro para a função AcceptEx
LPFN_ACCEPTEX GetAcceptExFunction(SOCKET listenSocket) {
    GUID guidAcceptEx = WSAID_ACCEPTEX;
    LPFN_ACCEPTEX lpAcceptEx = NULL;
    DWORD bytes = 0;

    if (WSAIoctl(listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
                 &guidAcceptEx, sizeof(guidAcceptEx),
                 &lpAcceptEx, sizeof(lpAcceptEx),
                 &bytes, NULL, NULL) == SOCKET_ERROR) {
        printf("WSAIoctl falhou ao obter o ponteiro para AcceptEx.\n");
        closesocket(listenSocket);
        WSACleanup();
        return NULL;
    }

    return lpAcceptEx;
}

int main() {
    SOCKET listenSocket;
    LPFN_ACCEPTEX lpAcceptEx;
    HANDLE hIOCP;
    DWORD bytesTransferred;
    ULONG_PTR completionKey;
    LPOVERLAPPED pOverlapped;
    BOOL result;
    ACCEPT_CONTEXT acceptContext;

    // Cria o socket de escuta
    listenSocket = CreateListenSocket();
    if (listenSocket == INVALID_SOCKET) {
        return 1;
    }

    // Obtém o ponteiro para a função AcceptEx
    lpAcceptEx = GetAcceptExFunction(listenSocket);
    if (lpAcceptEx == NULL) {
        return 1;
    }

    // Cria um IOCP e associa o socket de escuta a ele
    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hIOCP == NULL) {
        printf("Falha ao criar o IOCP.\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (CreateIoCompletionPort((HANDLE)listenSocket, hIOCP, 0, 0) == NULL) {
        printf("Falha ao associar o socket de escuta ao IOCP.\n");
        CloseHandle(hIOCP);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Inicializa o contexto de aceitação
    ZeroMemory(&acceptContext, sizeof(ACCEPT_CONTEXT));
    acceptContext.acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (acceptContext.acceptSocket == INVALID_SOCKET) {
        printf("Falha ao criar o socket de aceitação.\n");
        CloseHandle(hIOCP);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Inicia a aceitação assíncrona
    result = lpAcceptEx(listenSocket, acceptContext.acceptSocket,
                        acceptContext.addressBuffer, 0,
                        ADDRESS_BUFFER_SIZE, ADDRESS_BUFFER_SIZE,
                        NULL, &acceptContext.overlapped);
    if (result == FALSE) {
        if (WSAGetLastError() != WSA_IO_PENDING) {
            printf("AcceptEx falhou.\n");
            closesocket(acceptContext.acceptSocket);
            CloseHandle(hIOCP);
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
    }

    // Aguarda a conclusão da aceitação ou timeout
    result = GetQueuedCompletionStatus(hIOCP, &bytesTransferred, &completionKey, &pOverlapped, ACCEPT_TIMEOUT);
    if (result == FALSE) {
        if (pOverlapped == NULL) {
            printf("Tempo de aceitação esgotado.\n");
            // Cancela a operação de aceitação
            CancelIo((HANDLE)listenSocket);
            closesocket(acceptContext.acceptSocket);
            CloseHandle(hIOCP);
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        } else {
            printf("AcceptEx falhou durante a conclusão.\n");
            closesocket(acceptContext.acceptSocket);
            CloseHandle(hIOCP);
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
    }

    printf("Conexão aceita com sucesso.\n");

    int iResult = 0;

    iResult = setsockopt(acceptContext.acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&listenSocket, sizeof(listenSocket));

    // Aqui você pode continuar com operações assíncronas, como WSASend e WSARecv

    // Limpa os recursos
    closesocket(accept);
    // ...

    return 0;
}
