#include "NetHelper.h"

int main()
{
    InitNetHelper();
    SOCKET sock = CreateSocket(27015);
    SOCKADDR addr = CreateSocketAddressEmpty();

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        RecvMessage(&sock, &addr, buffer, &messageLength);
        if (messageLength > 0)
            printf("%.*s\n", messageLength, buffer);

        Sleep(1000);

        const char* message = "server";
        strcpy(buffer, message);
        messageLength = strlen(message);
        SendMessage(&sock, &addr, buffer, messageLength);

        Sleep(1000);
    }

    return 0;
}
