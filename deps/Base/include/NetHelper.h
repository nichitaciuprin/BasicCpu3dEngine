void NetListen(int port);
void NetSetTarget(const char* ip, int port);
void NetSend(char* buffer, int messageSize);
void NetRecv(char* buffer, int* messageSize);
void NetResp(char* buffer, int messageSize);
void NetSendFrame(char* frame);
void NetSendInput(bool w, bool a, bool s, bool d);
void NetRecvInput(bool* w, bool* a, bool* s, bool* d);
void NetRecvFrame(char* frame);
