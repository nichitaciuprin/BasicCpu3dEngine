#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetListen(27015);

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        NetRecv(buffer, &messageLength);
        if (messageLength > 0)
            printf("%.*s\n", messageLength, buffer);

        // Halt(1000);

        const char* message = "server";
        strcpy(buffer, message);
        messageLength = strlen(message);
        NetResp(buffer, messageLength);

        // Halt(1000);
    }

    return 0;
}
