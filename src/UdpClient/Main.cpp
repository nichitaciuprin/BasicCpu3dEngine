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
    printf("Process PID:%ld\n", (long)getpid());

    char frame[1024];

    auto window = make_unique<BitmapWindow2>(700, 100, 512, 512);

    while (true)
    {
        FixedTimeStart();

        if (!window->Exists()) break;
        window->Update();
        window->SetPixelsScaled2((uint8_t*)frame, 32, 32, 16);
        NetRecvFrame(frame);

        NetInput netInput = {};
        netInput.w = window->KeyDown_W();
        netInput.a = window->KeyDown_A();
        netInput.s = window->KeyDown_S();
        netInput.d = window->KeyDown_D();
        NetSendInput(&netInput);

        FixedTimeEnd();
    }

    return 0;
}
