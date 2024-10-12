struct RecvInput
{
    uint64_t id;
    bool w;
    bool a;
    bool s;
    bool d;
};

void NetHelper_PrintId(uint64_t id);
uint64_t NetHelper_CreateId(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t port);
void NetUsePort(int port);
void NetUseAnyPort();
void NetSetTarget(const char* ip, int port);
void NetSend(char* buffer, int messageSize);
void NetRecv(char* buffer, int* messageSize);
void NetResp(char* buffer, int messageSize);
void NetSendFrame(char* frame);
void NetSendInput(bool w, bool a, bool s, bool d);
void NetRecvInput(bool* w, bool* a, bool* s, bool* d);
bool NetRecvInput2(RecvInput* recvInputOut);
void NetRecvFrame(char* frame);
void NetSendFrame2(uint64_t id, char* frame);
void NetSend2(uint64_t* addr, char* buffer, int* messageSize);
void NetRecv2(uint64_t* addr, char* buffer, int* messageSize);