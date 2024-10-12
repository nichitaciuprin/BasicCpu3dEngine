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
#include "Server.h"

int main()
{
    printf("Process PID:%ld\n", (long)getpid());

    InitGame();
    // InitGameWindow();

    while (true)
    {
        FixedTimeStart();

        // if (GameWindowClosed()) break;

        // UpdateGameWindow();

        RenderGame();
        UpdateGame(0.010f);

        FixedTimeEnd();
    }

    return 0;
}
