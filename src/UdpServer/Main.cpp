#include "Std.h"
#include "SysHelper.h"
#include "NetHelper.h"

int main()
{
    NetUsePort(27015);

    char buffer[1024];
    int messageSize = 0;
    uint64_t addr = 0;

    while (true)
    {
        NetRecv(&addr, buffer, &messageSize);
        if (messageSize >= 0)
            printf("got message \"%.*s\"\n", messageSize, buffer);

        Halt(1000);

        if (addr == 0) continue;

        const char* message = "server";
        strcpy(buffer, message);
        messageSize = strlen(message);
        NetSend(&addr, buffer, &messageSize);

        Halt(1000);
    }

    return 0;
}
