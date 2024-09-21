#include "NetHelper.h"

int main()
{
    int result = 0;

    WSADATA wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
    {
        printf("WSAStartup failed with error %d\n", result);
        return 1;
    }

    SOCKET serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // makes socket non-blocking
    // u_long mode = 1;
    // ioctlsocket(serverSocket, FIONBIO, &mode);

    if (serverSocket == INVALID_SOCKET)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    struct sockaddr_in serverAddr;
    short port = 27015;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    int bytes_received;
    char messageBuffer[1025];
    int serverBufLen = 1024;

    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);

    while (true)
    {
        bytes_received = recvfrom(serverSocket, messageBuffer, serverBufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

        if (bytes_received == SOCKET_ERROR)
            printf("recvfrom failed with error %d\n", WSAGetLastError());

        messageBuffer[bytes_received] = '\0';

        char sendBuf[] = {'h', 'e', 'l', 'l', 'o', '\0'};
        int sendBufLen = (int)(sizeof(sendBuf) - 1);
        printf("echo\n");
        int sendResult = sendto(serverSocket, sendBuf, sendBufLen, 0, (SOCKADDR *)&SenderAddr, SenderAddrSize);

        if (result == SOCKET_ERROR)
            printf("Sending back response got an error: %d\n", WSAGetLastError());
    }


    return 0;
}
