#include "NetHelper.h"

int main()
{
    SOCKADDR addr = CreateSocketAddressEmpty();
    SOCKADDR addrServer = CreateSocketAddress("127.0.0.1", 27015);

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        const char* message = "sendtome";
        messageLength = strlen(message);
        strcpy(buffer, message);
        NetHelper_SendMessage(&addrServer, buffer, messageLength);

        Sleep(1000);

        NetHelper_RecvMessage(&addr, buffer, &messageLength);
        if (messageLength > 0)
            printf("%.*s\n", messageLength, buffer);

        Sleep(1000);
    }

    return 0;
}
