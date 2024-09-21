#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetHelper_InitNetHelper();
    SOCKET sock = NetHelper_CreateSocketNoBind();
    SOCKADDR addr = NetHelper_CreateSocketAddressEmpty();
    SOCKADDR addrServer = NetHelper_CreateSocketAddress("127.0.0.1", 27015);

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        const char* message = "sendtome";
        messageLength = strlen(message);
        strcpy(buffer, message);
        NetHelper_SendMessage(&sock, &addrServer, buffer, messageLength);

        Halt(1000);

        NetHelper_RecvMessage(&sock, &addr, buffer, &messageLength);
        if (messageLength > 0)
            printf("%.*s\n", messageLength, buffer);

        Halt(1000);
    }

    return 0;
}
