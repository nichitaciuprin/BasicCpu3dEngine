#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"
#include "SysWindow.h"

int main()
{
    auto window = BitmapWindow_Create(0, 0, 512, 512);

    while (true)
    {
        BitmapWindow_Update(window);
        cout << (BitmapWindow_Exists(window)) << endl;
        Halt(1000);
    }

    return 0;
}
