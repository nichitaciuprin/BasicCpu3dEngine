struct RecvInput
{
    uint64_t id;
    bool w;
    bool a;
    bool s;
    bool d;
};

void NetListen(int port);
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