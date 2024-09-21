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
