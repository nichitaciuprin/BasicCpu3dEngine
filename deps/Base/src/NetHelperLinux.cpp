#include <Std.h>
#include <StdExt.h>

#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef int SOCKET;
typedef struct sockaddr SOCKADDR;

static SOCKADDR CreateSocketAddress(const char* ip, short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton(ip, &addr.sin_addr);
    return *((SOCKADDR*)&addr);
}
static SOCKET CreateSocketNoBind()
{
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
        abort();

    // makes socket non-blocking
    fcntl(sock, F_SETFL, O_NONBLOCK);

    return sock;
}
static SOCKET CreateSocket(int port)
{
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
        abort();

    // makes socket non-blocking
    fcntl(sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // socklen_t socklen = sizeof(addr);
    auto bindResult = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
    if (bindResult == -1)
        abort();

    return sock;
}
static void SendMessage(SOCKET sock, SOCKADDR* addr, char* buffer, int messageSize)
{
    socklen_t addrSize = sizeof(*addr);
    sendto(sock, buffer, messageSize, 0, addr, addrSize);
}
static void RecvMessage(SOCKET sock, SOCKADDR* addr, char* buffer, int* messageSize)
{
    socklen_t addrSize = sizeof(*addr);
    int byteCount = recvfrom(sock, buffer, 1024, 0, addr, &addrSize);

    *messageSize = byteCount;
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

static SOCKET netsock;

void NetUseAnyPort()
{
    netsock = CreateSocketNoBind();
}
void NetUsePort(int port)
{
    netsock = CreateSocket(port);
}
void NetSend(uint64_t* addr, char* buffer, int* messageSize)
{
    assert(addr != NULL);
    assert(buffer != NULL);
    assert(messageSize != NULL);
    assert(*addr != 0);
    assert(*messageSize >= 0);

    struct sockaddr_in sockAddrIn;

    uint64_t id = *addr;

    uint16_t port  = id;  id = id >> 16;
    uint8_t b4     = id;  id = id >>  8;
    uint8_t b3     = id;  id = id >>  8;
    uint8_t b2     = id;  id = id >>  8;
    uint8_t b1     = id;

    sockAddrIn.sin_family = AF_INET;
    struct in_addr inAddr = {};
    inAddr.s_addr += b4; inAddr.s_addr <<= 8;
    inAddr.s_addr += b3; inAddr.s_addr <<= 8;
    inAddr.s_addr += b2; inAddr.s_addr <<= 8;
    inAddr.s_addr += b1;
    sockAddrIn.sin_addr = inAddr;
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

    SendMessage(netsock, sockAddr, buffer, *messageSize);
}
void NetRecv(uint64_t* addr, char* buffer, int* messageSize)
{
    struct sockaddr sockAddr = {};

    RecvMessage(netsock, &sockAddr, buffer, messageSize);

    if (*messageSize < 0) return;

    struct sockaddr_in* sockAddrIn = (sockaddr_in*)&sockAddr;

    // char* ip = inet_ntoa(sockAddrIn->sin_addr);
    // cout << ip << endl;

    // cout << "===RECV===" << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b1) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b2) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b3) << endl;
    // cout << ((int)sockAddrIn->sin_addr.S_un.S_un_b.s_b4) << endl;

    uint8_t b1 = sockAddrIn->sin_addr.s_addr;
    uint8_t b2 = sockAddrIn->sin_addr.s_addr >>  8;
    uint8_t b3 = sockAddrIn->sin_addr.s_addr >> 16;
    uint8_t b4 = sockAddrIn->sin_addr.s_addr >> 24;

    uint64_t id = 0;
    id += b1; id = id <<  8;
    id += b2; id = id <<  8;
    id += b3; id = id <<  8;
    id += b4; id = id << 16;
    id += ntohs(sockAddrIn->sin_port);

    *addr = id;
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
