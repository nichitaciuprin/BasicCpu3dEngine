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
#include "Client.h"

int main()
{
    printf("Process PID:%ld\n", (long)getpid());

    InitClientWindow();

    while (true)
    {
        FixedTimeStart();

        if (ClientWindowClosed()) break;

        UpdateClientWindow();

        RecvFrame();
        SendInput();

        FixedTimeEnd();
    }

    return 0;
}

// int main()
// {
//     InitClientWindow();

//     while (true)
//     {
//         FixedTimeStart();

//         if (ClientWindowClosed()) break;

//         UpdateClientWindow();

//         RenderClientWindow();
//         SendInput();

//         FixedTimeEnd();
//     }

//     return 0;
// }