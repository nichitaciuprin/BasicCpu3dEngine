#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetSetTarget("127.0.0.1", 27015);

    char buffer[1024];
    int messageLength = 0;

    while (true)
    {
        const char* message = "sendtome";
        messageLength = strlen(message);
        strcpy(buffer, message);
        NetSend(buffer, messageLength);

        Halt(1000);

        NetRecv(buffer, &messageLength);
        while (messageLength > 0)
        {
            NetRecv(buffer, &messageLength);
            printf("%.*s\n", messageLength, buffer);
        }

        Halt(1000);
    }

    return 0;
}
