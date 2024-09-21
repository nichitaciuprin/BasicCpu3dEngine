typedef unsigned long long SOCKET;
typedef unsigned short u_short;
struct sockaddr {
	u_short	sa_family;
	char	sa_data[14];
};
typedef struct sockaddr SOCKADDR;

void NetHelper_InitNetHelper();
SOCKADDR NetHelper_CreateSocketAddressEmpty();
SOCKADDR NetHelper_CreateSocketAddress(const char* ip, short port);
SOCKET NetHelper_CreateSocketNoBind();
SOCKET NetHelper_CreateSocketLocalHost(int port);
SOCKET NetHelper_CreateSocket(int port);
void NetHelper_SendMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int messageSize);
void NetHelper_RecvMessage(SOCKET* sock, SOCKADDR* addr, char* buffer, int* messageSize);

bool _NetInited = false;
SOCKET _NetSock;
SOCKADDR _NetTarget;
SOCKADDR _NetSource;

void _NetInit()
{
    if (_NetInited) return;
        _NetInited = true;
    NetHelper_InitNetHelper();
    _NetSource = NetHelper_CreateSocketAddressEmpty();
}
void NetListen(int port)
{
    _NetInit();
    _NetSock = NetHelper_CreateSocket(port);
}
void NetSetTarget(const char* ip, int port)
{
    _NetInit();
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