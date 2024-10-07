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
    auto width = 32;
    auto height = 32;
    auto scale = 16;

    auto window = make_unique<BitmapWindow2>(700, 100, width*scale, height*scale);

    while (window->Exists())
    {
        char pixels[1024];
        NetClientProcess(pixels, window->KeyDown_W(), window->KeyDown_A(), window->KeyDown_S(), window->KeyDown_D());
        window->SetPixelsScaled2((uint8_t*)pixels, width, height, scale);
        window->Update();
    }

    return 0;
}

// int main()
// {
//     char frame[1024];

//     do
//     {
//         GetNetFrame(frame);
//         DrawNetFrame(frame);
//         SendWindowInput();
//     }
//     while (NetWindowExists())

//     return 0;
// }
