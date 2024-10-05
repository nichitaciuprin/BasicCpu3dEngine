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

void main2()
{
    NetSetTarget("127.0.0.1", 27015);

    auto width = 32;
    auto height = 32;

    char pixels[1024];

    auto window = make_unique<BitmapWindow2>(700, 100, width*16, height*16);

    Camera camera = { 0, 1, 95 };

    while (window->Exists())
    {
        int messageLength = 0;

        char tempBuffer[1024];
        const char* message = "sendtome";
        strcpy(tempBuffer, message);
        messageLength = strlen(message);
        NetSend(tempBuffer, messageLength);

        while (true)
        {
            NetRecv(pixels, &messageLength);
            if (messageLength <= 0) break;
        }

        window->SetPixelsScaled2((uint8_t*)pixels, 32, 32, 16);

        window->Update();
    }
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
