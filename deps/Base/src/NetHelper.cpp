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

static SOCKET _NetSock;
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
void NetPrintAddr(uint64_t addr)
{
    for (size_t i = 0; i < 6; i++)
    {
        uint8_t byte = addr >> 8 * (5 - i);
        printf("%02x", byte);
    }

    printf("\n");
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
    bool w;
    bool a;
    bool s;
    bool d;
};

bool NetInitServerCalled = false;
void NetInitServer()
{
    if (NetInitServerCalled) return;
        NetInitServerCalled = true;

    NetUsePort(27015);
}

bool NetInitClientCalled = false;
void NetInitClient()
{
    if (NetInitClientCalled) return;
        NetInitClientCalled = true;

    NetUseAnyPort();
}

void NetSendFrame(uint64_t* addr, char* frame)
{
    NetInitServer();
    int messageSize = 1024;
    NetSend(addr, frame, &messageSize);
}
void NetSendInput(NetInput* input)
{
    NetInitClient();

    uint8_t message = 0;
    if (input->w) message += 8;
    if (input->a) message += 4;
    if (input->s) message += 2;
    if (input->d) message += 1;

    uint64_t addr;

    int messageSize = 1;

    NetSend(&addr, (char*)&message, &messageSize);
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
bool NetRecvInput(uint64_t* addr, NetInput* input)
{
    NetInitServer();

    char buffer[1024];
    int messageSize = 0;

    NetRecv(addr, buffer, &messageSize);

    if (messageSize < 0) return false;

    *input = {};

    uint8_t byte = ((uint8_t*)buffer)[0];

    input->w = (1 << 3 & byte) != 0;
    input->a = (1 << 2 & byte) != 0;
    input->s = (1 << 1 & byte) != 0;
    input->d = (1 << 0 & byte) != 0;

    return true;
}
