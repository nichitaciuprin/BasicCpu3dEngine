#include <Std.h>
#include <StdExt.h>
#include <SysHelper.h>
#include <NetHelper.h>

int main()
{
    NetUseAnyPort();

    auto addr = NetCreateAddr(127, 0, 0, 1, 8888);

    char buffer[1024] = {};
    const char* message = "sendtome\0";
    int messageSize = strlen(message);
    strcpy(buffer, message);

    NetSend(&addr, buffer, &messageSize);
    NetRecv(&addr, buffer, &messageSize);
    printf("%.*s\n", messageSize, buffer);

    // while (true)
    // {
    //     FixedTimeStart();
    //     FixedTimeEnd();
    // }

    return 0;
}
