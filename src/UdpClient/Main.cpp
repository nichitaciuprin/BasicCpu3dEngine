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
#include "Temp1.h"

int main()
{
    char frame[1024];

    while (true)
    {
        FixedTimeStart();

        NetRecvFrame(frame);
        Window32Render(frame);

        bool w = Window32_W();
        bool a = Window32_A();
        bool s = Window32_S();
        bool d = Window32_D();
        NetSendInput(w, a, s, d);

        if (Window32Closed()) break;

        FixedTimeEnd();
    }

    return 0;
}
