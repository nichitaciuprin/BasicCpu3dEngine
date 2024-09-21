#include "NetHelper.h"

int main()
{
    InitNetHelper();

    SOCKET sock = CreateSocket();
    SOCKADDR source = CreateSocketAddressEmpty();
    SOCKADDR target = CreateSocketAddress("127.0.0.1", 27015);

    char buffer[1024];

    while (true)
    {
        const char* message = "sendtome";
        int messageLen = strlen(message);
        strcpy(buffer, message);
        SendMessage(&sock, &target, buffer, messageLen);

        Sleep(1000);

        int messageSize = 0;

        RecvMessage(&sock, &source, buffer, &messageSize);
        if (messageSize > 0)
            printf("%.*s\n", messageSize, buffer);

        Sleep(1000);
    }

    return 0;
}
