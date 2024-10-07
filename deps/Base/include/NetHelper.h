void NetListen(int port);
void NetSetTarget(const char* ip, int port);
void NetSend(char* buffer, int messageSize);
void NetRecv(char* buffer, int* messageSize);
void NetResp(char* buffer, int messageSize);
void NetServerProcess(char* frame, bool* w, bool* a, bool* s, bool* d);
void NetGetFrame(char* frame);
void NetSendInput(bool w, bool a, bool s, bool d);
