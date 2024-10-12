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

static bool _NetInited = false;
static SOCKET _NetSock;
static SOCKADDR _NetTarget;
static SOCKADDR _NetSource;

static void NetHelper_InitNetHelper()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
        printf("WSAStartup failed with error %d\n", result);
}

static SOCKADDR NetHelper_CreateSocketAddressEmpty()
{
    struct sockaddr_in addr1;
    return *((SOCKADDR*)&addr1);
}
static SOCKADDR NetHelper_CreateSocketAddress(const char* ip, short port)
{
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(port);
    addr1.sin_addr.s_addr = inet_addr(ip);
    return *((SOCKADDR*)&addr1);
}
static SOCKET NetHelper_CreateSocketNoBind()
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
static SOCKET NetHelper_CreateSocket(int port)
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
static void NetHelper_SendMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int messageSize)
{
    int addrSize = (sizeof(*addr));
    sendto(*sock, buffer, messageSize, 0, addr, addrSize);
}
static void NetHelper_RecvMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int* messageSize)
{
    int addrSize = (sizeof(*addr));
    int byteCount = recvfrom(*sock, buffer, 1024, 0, addr, &addrSize);

    // if (byteCount > 0)
    // {
    //     char* ip = inet_ntoa(((sockaddr_in*)addr)->sin_addr);
    //     // cout << ip << endl;
    // }

    *messageSize = byteCount;
}


uint64_t NetHelper_CreateId(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t port)
{
    uint64_t ip = 0;

    ip += b1; ip = ip <<  8;
    ip += b2; ip = ip <<  8;
    ip += b3; ip = ip <<  8;
    ip += b4; ip = ip << 16;
    ip += port;

    return ip;
}
static uint64_t NetHelper_ntohs(SOCKADDR addr)
{
    uint64_t ip = 0;

    sockaddr_in* sockAddrIn = (sockaddr_in*)&addr;

    ip += sockAddrIn->sin_addr.S_un.S_un_b.s_b1; ip = ip <<  8;
    ip += sockAddrIn->sin_addr.S_un.S_un_b.s_b2; ip = ip <<  8;
    ip += sockAddrIn->sin_addr.S_un.S_un_b.s_b3; ip = ip <<  8;
    ip += sockAddrIn->sin_addr.S_un.S_un_b.s_b4; ip = ip << 16;
    ip += ntohs(sockAddrIn->sin_port);

    return ip;
}
static SOCKADDR NetHelper_htons(uint64_t addr)
{
    sockaddr_in sockAddrIn;

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_port = htons(addr);           addr = addr >> 16;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b1 = addr; addr = addr >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b2 = addr; addr = addr >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b3 = addr; addr = addr >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b4 = addr;

    return *((SOCKADDR*)&sockAddrIn);
}
bool dontPrint = false;
void NetHelper_PrintId(uint64_t id)
{
    // if (dontPrint) return;
    // dontPrint = true;

    for (size_t i = 0; i < 6; i++)
    {
        uint8_t byte = id >> 8 * (5 - i);
        printf("%02x", byte);
    }

    printf("\n");
}

void NetInit()
{
    if (_NetInited) return;
        _NetInited = true;

    NetHelper_InitNetHelper();
    _NetSource = NetHelper_CreateSocketAddressEmpty();
}
void NetUseAnyPort()
{
    NetInit();
    _NetSock = NetHelper_CreateSocketNoBind();
}
void NetUsePort(int port)
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

    // if (*messageSize >= 0)
    // {
    //     // struct in_addr addr = ((sockaddr_in*)&_NetSource)->sin_addr;
    //     // char* ip = inet_ntoa(addr);
    //     // cout << ip << endl;
    //     // NetRecvId = NetHelper_ToId(&_NetSource);
    //     // NetHelper_PrintId(id);
    // }
}
void NetResp(char* buffer, int messageSize)
{
    NetHelper_SendMessage(&_NetSock, &_NetSource, buffer, messageSize);
}


static void NetHelper_SendMessage2(SOCKET* sock, uint64_t id, char* buffer, int messageSize)
{
    sockaddr_in sockAddrIn;

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_port = htons(id);           id = id >> 16;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b1 = id; id = id >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b2 = id; id = id >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b3 = id; id = id >>  8;
    sockAddrIn.sin_addr.S_un.S_un_b.s_b4 = id;

    SOCKADDR* sockAddr = (SOCKADDR*)&sockAddrIn;
    int sockAddrSize = (sizeof(*sockAddr));

    sendto(*sock, buffer, messageSize, 0, sockAddr, sockAddrSize);
}
static void NetHelper_RecvMessage2(SOCKET* sock, uint64_t* id, char* buffer, int* messageSize)
{
    SOCKADDR sockAddr;
    int sockAddrSize = (sizeof(sockAddr));
    int byteCount = recvfrom(*sock, buffer, 1024, 0, &sockAddr, &sockAddrSize);

    *messageSize = byteCount;

    if (byteCount < 0) return;

    uint64_t id2 = 0;

    sockaddr_in* sockAddrIn = (sockaddr_in*)&sockAddr;

    id2 += sockAddrIn->sin_addr.S_un.S_un_b.s_b1; id2 = id2 <<  8;
    id2 += sockAddrIn->sin_addr.S_un.S_un_b.s_b2; id2 = id2 <<  8;
    id2 += sockAddrIn->sin_addr.S_un.S_un_b.s_b3; id2 = id2 <<  8;
    id2 += sockAddrIn->sin_addr.S_un.S_un_b.s_b4; id2 = id2 << 16;
    id2 += ntohs(sockAddrIn->sin_port);

    *id = id2;
}

void NetSend2(uint64_t* addr, char* buffer, int* messageSize)
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
    int sockAddrSize = (sizeof(*sockAddr));

    NetHelper_SendMessage(&_NetSock, sockAddr, buffer, *messageSize);
}
void NetRecv2(uint64_t* addr, char* buffer, int* messageSize)
{
    SOCKADDR sockAddr;

    NetHelper_RecvMessage(&_NetSock, &sockAddr, buffer, messageSize);

    // cout << (*messageSize) << endl;

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

void NetSend2(char* buffer, int messageSize, uint64_t id)
{
    auto addr = NetHelper_htons(id);

    NetHelper_SendMessage(&_NetSock, &addr, buffer, messageSize);
}

struct RecvInput
{
    uint64_t id;
    bool w;
    bool a;
    bool s;
    bool d;
};

bool NetServerProcessCalled = false;
void NetSendFrame(char* frame)
{
    if (!NetServerProcessCalled)
    {
        NetServerProcessCalled = true;
        NetUsePort(27015);
    }

    NetResp(frame, 1024);
}
void NetRecvInput(bool* w, bool* a, bool* s, bool* d)
{
    if (!NetServerProcessCalled)
    {
        NetServerProcessCalled = true;
        NetUsePort(27015);
    }

    char recvBuffer[1024];

    while (true)
    {
        int messageLength = 0;

        *w = false;
        *a = false;
        *s = false;
        *d = false;

        NetRecv(recvBuffer, &messageLength);

        uint8_t temp2 = *(uint8_t*)recvBuffer;

        *w = (1 << 3 & temp2) != 0;
        *a = (1 << 2 & temp2) != 0;
        *s = (1 << 1 & temp2) != 0;
        *d = (1 << 0 & temp2) != 0;

        if (messageLength <= 0) break;
    }
}

bool NetRecvInput2(RecvInput* recvInputOut)
{
    if (!NetServerProcessCalled)
    {
        NetServerProcessCalled = true;
        NetUsePort(27015);
    }

    char recvBuffer[1024];

    int messageLength = 0;

    // *recvInputOut = {};

    NetHelper_RecvMessage(&_NetSock, &_NetSource, recvBuffer, &messageLength);

    if (messageLength < 0)
        return false;

    recvInputOut->id = NetHelper_ntohs(_NetSource);

    uint8_t temp2 = *(uint8_t*)recvBuffer;

    recvInputOut->w = (1 << 3 & temp2) != 0;
    recvInputOut->a = (1 << 2 & temp2) != 0;
    recvInputOut->s = (1 << 1 & temp2) != 0;
    recvInputOut->d = (1 << 0 & temp2) != 0;

    return true;
}
void NetSendFrame2(uint64_t id, char* frame)
{
    if (!NetServerProcessCalled)
    {
        NetServerProcessCalled = true;
        NetUsePort(27015);
    }

    NetHelper_PrintId(id);

    auto addr = NetHelper_htons(id);

    NetHelper_SendMessage(&_NetSock, &addr, frame, 1024);
}

bool NetClientProcessCalled = false;
void NetRecvFrame(char* frame)
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
        if (messageLength > 0)
        {
            cout << "YES" << endl;
        }
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

    auto id = NetHelper_CreateId(127, 0, 0, 1, 27015);

    NetHelper_SendMessage2(&_NetSock, id, (char*)&message, 1);
}

