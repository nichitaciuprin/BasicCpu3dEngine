#include <Std.h>
#include <StdExt.h>
#include <SysHelper.h>
#include <BitmapWindow.h>

int main()
{
    int width = 512;
    int height = 512;

    uint32_t* pixels = (uint32_t*)malloc(4 * width * height);

    auto window1 = BitmapWindow_Create(0, 0, width, height);
    auto window2 = BitmapWindow_Create(200, 0, width, height);

    while (true)
    {
        auto window1Closed = !BitmapWindow_Exists(window1);
        auto window2Closed = !BitmapWindow_Exists(window2);

        if (window1Closed && window2Closed) break;

        long time1 = GetTime();

        for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            uint8_t r = (time1 / 10) % 255;
            uint8_t g = (time1 / 10) % 255;
            uint8_t b = 0;
            uint8_t a = 0;

            uint32_t pixel = 0;

            pixel += a; pixel <<= 8;
            pixel += r; pixel <<= 8;
            pixel += g; pixel <<= 8;
            pixel += b;

            pixels[y * width + x] = pixel;
        }

        // BitmapWindow_SetPixelsScaled(window, pixels, 16, 16, 4);
        // BitmapWindow_SetPixelsScaled2(window, (uint8_t*)pixels, 16, 16, 4);

        BitmapWindow_SetPixels(window1, pixels, width, height);
        BitmapWindow_SetPixels(window2, pixels, width, height);

        BitmapWindow_Update(window1);
        BitmapWindow_Update(window2);
    }

    return 0;
}
