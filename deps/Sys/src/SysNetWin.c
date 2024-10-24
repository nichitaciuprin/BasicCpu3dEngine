#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <winsock.h>

static void InitNetHelper()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
        printf("WSAStartup failed with error %d\n", result);
}

// static SOCKADDR CreateSocketAddress(const char* ip, short port)
// {
//     struct sockaddr_in addr;
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(port);
//     addr.sin_addr.s_addr = inet_addr(ip);
//     return *((SOCKADDR*)&addr);
// }
static SOCKET CreateSocketNoBind()
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
static SOCKET CreateSocket(int port)
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // makes socket non-blocking
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    if (sock == INVALID_SOCKET)
        printf("socket failed with error %d\n", WSAGetLastError());

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)))
    {
        printf("bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    return sock;
}
static void SendMessage(SOCKET sock, SOCKADDR* addr, char* buffer, int messageSize)
{
    int addrSize = (sizeof(*addr));
    sendto(sock, buffer, messageSize, 0, addr, addrSize);
}
static void RecvMessage(SOCKET sock, SOCKADDR* addr, char* buffer, int* messageSize)
{
    int addrSize = (sizeof(*addr));
    int byteCount = recvfrom(sock, buffer, 1024, 0, addr, &addrSize);

    // if (byteCount > 0)
    // {
    //     char* ip = inet_ntoa(((sockaddr_in*)addr)->sin_addr);
    //     // cout << ip << endl;
    // }

    *messageSize = byteCount;
}

static SOCKET netsock;
static bool NetInitCalled = false;
static void NetInit()
{
    if (NetInitCalled) return;
        NetInitCalled = true;

    InitNetHelper();
}

void NetPrintAddrAsHex(uint64_t addr)
{
    for (size_t i = 0; i < 6; i++)
    {
        uint8_t byte = addr >> 8 * (5 - i);
        printf("%02x", byte);
    }

    printf("\n");
}
void NetPrintAddr(uint64_t addr)
{
    { uint8_t byte = addr >> 8 * (5 - 0); int byte2 = byte; printf("%i", byte2); } printf(".");
    { uint8_t byte = addr >> 8 * (5 - 1); int byte2 = byte; printf("%i", byte2); } printf(".");
    { uint8_t byte = addr >> 8 * (5 - 2); int byte2 = byte; printf("%i", byte2); } printf(".");
    { uint8_t byte = addr >> 8 * (5 - 3); int byte2 = byte; printf("%i", byte2); } printf(":");

    { uint16_t port = addr; printf("%i", port); }
}
uint64_t NetCreateAddr(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t port)
{
    uint64_t ip = 0;

    ip += b1; ip = ip <<  8;
    ip += b2; ip = ip <<  8;
    ip += b3; ip = ip <<  8;
    ip += b4; ip = ip << 16;
    ip += port;

    return ip;
}

void NetUseAnyPort()
{
    NetInit();
    netsock = CreateSocketNoBind();
}
void NetUsePort(int port)
{
    NetInit();
    netsock = CreateSocket(port);
}
void NetSend(uint64_t* addr, char* buffer, int* messageSize)
{
    assert(addr != NULL);
    assert(buffer != NULL);
    assert(messageSize != NULL);
    assert(*addr != 0);
    assert(*messageSize >= 0);

    sockaddr_in sockAddrIn;

    uint64_t id = *addr;

    // NetHelper_PrintId(id);

    uint16_t port  = id;  id = id >> 16;
    uint8_t b4     = id;  id = id >>  8;
    uint8_t b3     = id;  id = id >>  8;
    uint8_t b2     = id;  id = id >>  8;
    uint8_t b1     = id;

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b1 = b1;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b2 = b2;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b3 = b3;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b4 = b4;
    sockAddrIn.sin_port = htons(port);

    // cout << "===SEND===" << endl;
    // cout << ((int)sockAddrIn.sin_addr.S_un.S_un_b.s_b1) << endl;
    // cout << ((int)sockAddrIn.sin_addr.S_un.S_un_b.s_b2) << endl;
    // cout << ((int)sockAddrIn.sin_addr.S_un.S_un_b.s_b3) << endl;
    // cout << ((int)sockAddrIn.sin_addr.S_un.S_un_b.s_b4) << endl;

    // char* ip = inet_ntoa(sockAddrIn.sin_addr);
    // cout << ip << endl;

    SOCKADDR* sockAddr = (SOCKADDR*)&sockAddrIn;

    SendMessage(netsock, sockAddr, buffer, *messageSize);
}
void NetRecv(uint64_t* addr, char* buffer, int* messageSize)
{
    SOCKADDR sockAddr;

    RecvMessage(netsock, &sockAddr, buffer, messageSize);

    if (*messageSize < 0) return;

    sockaddr_in* sockAddrIn = (sockaddr_in*)&sockAddr;

    // char* ip = inet_ntoa(sockAddrIn->sin_addr);
    // cout << ip << endl;

    uint64_t id = 0;

    // cout << "===RECV===" << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b1) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b2) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b3) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b4) << endl;

    id += sockAddrIn->sin_addr.S_un.S_un_b.s_b1; id = id <<  8;
    id += sockAddrIn->sin_addr.S_un.S_un_b.s_b2; id = id <<  8;
    id += sockAddrIn->sin_addr.S_un.S_un_b.s_b3; id = id <<  8;
    id += sockAddrIn->sin_addr.S_un.S_un_b.s_b4; id = id << 16;
    id += ntohs(sockAddrIn->sin_port);

    *addr = id;
}
