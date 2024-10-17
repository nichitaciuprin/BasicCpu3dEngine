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
    int x, y;
    GetConsolePosition(&x, &y);

    cout << x << "|" << y << endl;

    printf("Process PID:%ld\n", (long)GetPid());

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
