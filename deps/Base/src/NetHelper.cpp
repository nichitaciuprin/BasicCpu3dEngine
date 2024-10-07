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

bool _NetInited = false;
SOCKET _NetSock;
SOCKADDR _NetTarget;
SOCKADDR _NetSource;

void NetHelper_InitNetHelper()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
        printf("WSAStartup failed with error %d\n", result);
}
SOCKADDR NetHelper_CreateSocketAddressEmpty()
{
    struct sockaddr_in addr1;
    return *((SOCKADDR*)&addr1);
}
SOCKADDR NetHelper_CreateSocketAddress(const char* ip, short port)
{
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(port);
    addr1.sin_addr.s_addr = inet_addr(ip);
    return *((SOCKADDR*)&addr1);
}
SOCKET NetHelper_CreateSocketNoBind()
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
SOCKET NetHelper_CreateSocketLocalHost(int port)
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // makes socket non-blocking
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    if (sock == INVALID_SOCKET)
        printf("socket failed with error %d\n", WSAGetLastError());

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)))
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    return sock;
}
SOCKET NetHelper_CreateSocket(int port)
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // makes socket non-blocking
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    if (sock == INVALID_SOCKET)
        printf("socket failed with error %d\n", WSAGetLastError());

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)))
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    return sock;
}
void NetHelper_SendMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int messageSize)
{
    int addrSize = (sizeof(*addr));
    sendto(*sock, buffer, messageSize, 0, addr, addrSize);
}
void NetHelper_RecvMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int* messageSize)
{
    int addrSize = (sizeof(*addr));
    int byteCount = recvfrom(*sock, buffer, 1024, 0, addr, &addrSize);
    *messageSize = byteCount;
}

void NetInit()
{
    if (_NetInited) return;
        _NetInited = true;

    NetHelper_InitNetHelper();
    _NetSource = NetHelper_CreateSocketAddressEmpty();
}

void NetListen(int port)
{
    NetInit();
    _NetSock = NetHelper_CreateSocket(port);
}
void NetSetTarget(const char* ip, int port)
{
    NetInit();
    _NetSock = NetHelper_CreateSocketNoBind();
    _NetTarget = NetHelper_CreateSocketAddress(ip, port);
}
void NetSend(char* buffer, int messageSize)
{
    NetHelper_SendMessage(&_NetSock, &_NetTarget, buffer, messageSize);
}
void NetRecv(char* buffer, int* messageSize)
{
    NetHelper_RecvMessage(&_NetSock, &_NetSource, buffer, messageSize);
}
void NetResp(char* buffer, int messageSize)
{
    NetHelper_SendMessage(&_NetSock, &_NetSource, buffer, messageSize);
}

bool NetServerProcessCalled = false;
void NetServerProcess(char* frame, bool* w, bool* a, bool* s, bool* d)
{
    *w = false;
    *a = false;
    *s = false;
    *d = false;

    if (!NetServerProcessCalled)
    {
        NetServerProcessCalled = true;
        NetListen(27015);
    }

    char recvBuffer[1024];
    int messageLength = 0;
    NetRecv(recvBuffer, &messageLength);

    if (messageLength == 1)
    {
        uint8_t temp2 = *(uint8_t*)recvBuffer;

        *w = (1 << 3 & temp2) != 0;
        *a = (1 << 2 & temp2) != 0;
        *s = (1 << 1 & temp2) != 0;
        *d = (1 << 0 & temp2) != 0;
    }

    while (messageLength > 0)
        NetRecv(recvBuffer, &messageLength);

    NetResp(frame, 1024);
}

bool NetClientProcessCalled = false;
void NetGetFrame(char* frame)
{
    if (!NetClientProcessCalled)
    {
        NetClientProcessCalled = true;
        NetSetTarget("127.0.0.1", 27015);
    }

    while (true)
    {
        int messageLength = 0;
        NetRecv(frame, &messageLength);
        if (messageLength <= 0) break;
    }
}
void NetSendInput(bool w, bool a, bool s, bool d)
{
    if (!NetClientProcessCalled)
    {
        NetClientProcessCalled = true;
        NetSetTarget("127.0.0.1", 27015);
    }

    uint8_t message = 0;
    if (w) message += 8;
    if (a) message += 4;
    if (s) message += 2;
    if (d) message += 1;

    NetSend((char*)&message, 1);
}
