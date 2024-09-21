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

void InitNetHelper()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
        printf("WSAStartup failed with error %d\n", result);
}
SOCKET CreateSocket()
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // makes socket non-blocking
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    if (sock == INVALID_SOCKET)
        printf("socket failed with error %d\n", WSAGetLastError());

    return sock;
}
SOCKADDR CreateSocketAddress(const char* ip, short port)
{
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(port);
    addr1.sin_addr.s_addr = inet_addr(ip);
    return *((SOCKADDR*)&addr1);
}
SOCKADDR CreateSocketAddressEmpty()
{
    struct sockaddr_in addr1;
    return *((SOCKADDR*)&addr1);
}
void SendMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int messageSize)
{
    int addrSize = (sizeof(*addr));
    sendto(*sock, buffer, messageSize, 0, addr, addrSize);
}
void RecvMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int* messageSize)
{
    int addrSize = (sizeof(*addr));
    int byteCount = recvfrom(*sock, buffer, 1024, 0, addr, &addrSize);
    *messageSize = byteCount;
}

// bool _NetHelper_inited = false;
// SOCKET _NetHelper_socket;
// SOCKADDR _NetHelper_socketAddrEmpty;

// void _NetHelper_Init()
// {
//     if (_NetHelper_inited) return;
//     InitNetHelper();
//     _NetHelper_socket = CreateSocket();
//     _NetHelper_socketAddrEmpty = CreateSocketAddressEmpty();
// }
// void NetHelper_SendMessage(SOCKADDR* addr, char* buffer, int messageSize)
// {
//     _NetHelper_Init();
//     SendMessage(&_NetHelper_socket, addr, buffer, messageSize);
// }
// void NetHelper_RecvMessage(SOCKADDR* addr, char* buffer, int* messageSize)
// {
//     _NetHelper_Init();
//     RecvMessage(&_NetHelper_socket, addr, buffer, messageSize);
// }
// void NetHelper_RecvMessage(char* buffer, int* messageSize)
// {
//     _NetHelper_Init();
//     RecvMessage(&_NetHelper_socket, &_NetHelper_socketAddrEmpty, buffer, messageSize);
// }
