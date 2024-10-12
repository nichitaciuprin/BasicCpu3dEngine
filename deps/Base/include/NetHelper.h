void NetUseAnyPort();
void NetUsePort(int port);
void NetSend(uint64_t* addr, char* buffer, int* messageSize);
void NetRecv(uint64_t* addr, char* buffer, int* messageSize);
void NetPrintAddr(uint64_t addr);
uint64_t NetCreateAddr(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t port);

struct NetInput
{
    bool w;
    bool a;
    bool s;
    bool d;
};

void NetSendFrame(uint64_t* addr, char* frame);
void NetSendInput(NetInput* input);

bool NetRecvFrame(char* frame);
bool NetRecvInput(uint64_t* addr, NetInput* input);
