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

static void NetHelper_InitNetHelper()
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != NO_ERROR)
        printf("WSAStartup failed with error %d\n", result);
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

static SOCKET _NetSock;
bool NetInitCalled = false;
void NetInit()
{
    if (NetInitCalled) return;
        NetInitCalled = true;

    NetHelper_InitNetHelper();
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
    int sockAddrSize = (sizeof(*sockAddr));

    NetHelper_SendMessage(&_NetSock, sockAddr, buffer, *messageSize);
}
void NetRecv(uint64_t* addr, char* buffer, int* messageSize)
{
    SOCKADDR sockAddr;

    NetHelper_RecvMessage(&_NetSock, &sockAddr, buffer, messageSize);

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

struct NetInput
{
    bool w, a, s, d;
    bool up, left, down, right;
    bool q, e;
};

bool NetInitServerCalled = false;
void NetInitServer()
{
    if (NetInitServerCalled) return;
        NetInitServerCalled = true;

    NetUsePort(27015);
}
void NetSendFrame(uint64_t* addr, char* frame)
{
    NetInitServer();
    int messageSize = 1024;
    NetSend(addr, frame, &messageSize);
}
bool NetRecvInput(uint64_t* addr, NetInput* input)
{
    NetInitServer();

    char buffer[1024];
    int messageSize = 0;

    NetRecv(addr, buffer, &messageSize);

    if (messageSize < 0) return false;

    *input = {};

    uint8_t byte1 = ((uint8_t*)buffer)[0];
    uint8_t byte2 = ((uint8_t*)buffer)[1];

    input->w     = (1 << 7 & byte1) != 0;
    input->a     = (1 << 6 & byte1) != 0;
    input->s     = (1 << 5 & byte1) != 0;
    input->d     = (1 << 4 & byte1) != 0;
    input->up    = (1 << 3 & byte1) != 0;
    input->left  = (1 << 2 & byte1) != 0;
    input->down  = (1 << 1 & byte1) != 0;
    input->right = (1 << 0 & byte1) != 0;

    input->q  = (1 << 7 & byte2) != 0;
    input->e  = (1 << 6 & byte2) != 0;

    return true;
}

bool NetInitClientCalled = false;
void NetInitClient()
{
    if (NetInitClientCalled) return;
        NetInitClientCalled = true;

    NetUseAnyPort();
    // NetUsePort(27016);
}
bool NetRecvFrame(char* frame)
{
    NetInitClient();

    int messageSize = 0;

    uint64_t addr;

    NetRecv(&addr, frame, &messageSize);

    if (messageSize < 0) return false;

    return true;
}
void NetSendInput(NetInput* input)
{
    NetInitClient();

    uint8_t bytes[2] = {};
    // uint8_t message2 = 0;

    if (input->w)     bytes[0] += 128;
    if (input->a)     bytes[0] +=  64;
    if (input->s)     bytes[0] +=  32;
    if (input->d)     bytes[0] +=  16;
    if (input->up)    bytes[0] +=   8;
    if (input->left)  bytes[0] +=   4;
    if (input->down)  bytes[0] +=   2;
    if (input->right) bytes[0] +=   1;

    if (input->q)     bytes[1] +=  128;
    if (input->e)     bytes[1] +=   64;

    // if (input->w) message += 2;
    // if (input->a) message += 1;

    uint64_t addr = NetCreateAddr(127, 0, 0, 1, 27015);

    int messageSize = 2;

    NetSend(&addr, (char*)&bytes, &messageSize);
}
