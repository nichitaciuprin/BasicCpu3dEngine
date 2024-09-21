#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetHelper_InitNetHelper();
    SOCKET sock = NetHelper_CreateSocket(27015);
    SOCKADDR addr = NetHelper_CreateSocketAddressEmpty();

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        NetHelper_RecvMessage(&sock, &addr, buffer, &messageLength);
        if (messageLength > 0)
            printf("%.*s\n", messageLength, buffer);

        Halt(1000);

        const char* message = "server";
        strcpy(buffer, message);
        messageLength = strlen(message);
        NetHelper_SendMessage(&sock, &addr, buffer, messageLength);

        Halt(1000);
    }

    return 0;
}
