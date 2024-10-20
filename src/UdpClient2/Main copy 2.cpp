#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"
#include "Subgen.h"
#include "Helper.h"
#include "Models.h"
#include "Clipping.h"
#include "Bitmap.h"
#include "BitmapWindow.h"
#include "NetHelper.h"

int main()
{
    auto width = 512;
    auto height = 512;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow2>(700, 100, width, height);

    NetSetTarget("127.0.0.1", 27015);

    char buffer[1024];
    int messageLength = 0;

    while (window->Exists())
    {
        bitmap->Fill(BLACK);

        const char* message = "sendtome";
        messageLength = strlen(message);
        strcpy(buffer, message);
        NetSend(buffer, messageLength);

        // Halt(1000);

        NetRecv(buffer, &messageLength);
        while (messageLength > 0)
        {
            NetRecv(buffer, &messageLength);
            printf("%.*s\n", messageLength, buffer);
        }

        window->Update();
    }

    return 0;
}
