#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetUsePort(27016);

    char buffer[1024];
    int messageSize = 0;
    uint64_t addr = NetCreateAddr(127, 0, 0, 1, 27015);

    while (true)
    {
        const char* message = "client";
        strcpy(buffer, message);
        messageSize = strlen(message);
        NetSend(&addr, buffer, &messageSize);

        Halt(1000);

        NetRecv(&addr, buffer, &messageSize);
        if (messageSize >= 0)
            printf("got message \"%.*s\"\n", messageSize, buffer);

        Halt(1000);
    }

    return 0;
}
