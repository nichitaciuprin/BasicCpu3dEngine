#include "SysHelper.h"
#include "SysWindow.h"
#include "SysNet.h"

#include "Std.h"
#include "StdExt.h"
#include "Subgen.h"
#include "Helper.h"
#include "Models.h"
#include "Clipping.h"
#include "Bitmap.h"
#include "Server.h"

int main()
{
    printf("PID:%ld\n", (long)GetPid());

    InitGame();

    while (true)
    {
        FixedTimeStart();

        TestRender();

        RenderGame();
        UpdateGame(0.010f);

        FixedTimeEnd();
    }

    return 0;
}
