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
    int x, y;
    GetConsolePosition(&x, &y);

    printf("PID:%ld\n", (long)GetPid());

    // missing memset to render stack, for fun
    char frame[1024];

    auto window = make_unique<BitmapWindow2>(x, y, 512, 512);

    while (true)
    {
        FixedTimeStart();

        if (!window->Exists())
            break;

        window->Update();
        window->SetPixelsScaled2((uint8_t *)frame, 32, 32, 16);

        while (NetRecvFrame(frame))
        {
        }

        NetInput netInput = {};

        netInput.w = window->KeyDown_W();
        netInput.a = window->KeyDown_A();
        netInput.s = window->KeyDown_S();
        netInput.d = window->KeyDown_D();

        netInput.up = window->KeyDown_UP();
        netInput.left = window->KeyDown_LEFT();
        netInput.down = window->KeyDown_DOWN();
        netInput.right = window->KeyDown_RIGHT();

        netInput.q = window->KeyDown_Q();
        netInput.e = window->KeyDown_E();

        NetSendInput(&netInput);

        FixedTimeEnd();
    }

    return 0;
}
