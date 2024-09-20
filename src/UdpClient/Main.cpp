#include <Std.h>
#include <StdExt.h>

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <winuser.h>

#include <WinSock2.h>
#include <Ws2tcpip.h>

int main()
{
    int iResult = 0;

    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != NO_ERROR)
    {
        printf("WSAStartup failed with error %d\n", iResult);
        return 1;
    }

    SOCKET soc = INVALID_SOCKET;
    soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (soc == INVALID_SOCKET)
    {
        printf("socket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    struct sockaddr_in serverAddr;
    int SenderAddrSize = sizeof(serverAddr);

    char SendBuf[1024];
    int BufLen = (int)(sizeof(SendBuf) - 1);
    const char* toSend = "foobar";
    strcpy(SendBuf, toSend);

    struct sockaddr_in ClientAddr;
    int clientAddrSize = (int)sizeof(ClientAddr);
    short port = 27015;
    const char* local_host = "127.0.0.1";
    ClientAddr.sin_family = AF_INET;
    ClientAddr.sin_port = htons(port);
    ClientAddr.sin_addr.s_addr = inet_addr(local_host);
    printf("Sending a datagram to the receiver...\n");

    int clientResult = sendto(soc, SendBuf, BufLen, 0, (SOCKADDR*)&ClientAddr, clientAddrSize);

    int bytes_received = recvfrom(soc, SendBuf, BufLen, 0, (SOCKADDR*)&ClientAddr, &SenderAddrSize);

    SendBuf[bytes_received] = '\0';

    printf(SendBuf);
    printf("\n");

    return 0;
}
