#include "LightWindow.h"

void TestDraw(BitmapWindow* window, int width, int height)
{
    static int iteration = 0;
    iteration++;

    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        uint32_t a = 0;
        uint32_t r = (uint8_t)(iteration / 9);
        uint32_t g = (uint8_t)(iteration / 6);
        uint32_t b = (uint8_t)(iteration / 3);

        uint32_t pixel =
            (a << (8 * 3)) +
            (r << (8 * 2)) +
            (g << (8 * 1)) +
            (b << (8 * 0));

        BitmapWindow_SetPixel(window, x, y, pixel);
    }
}

int main()
{
    int size = 512;

    BitmapWindow* window = BitmapWindow_Create(0, 0, size, size);

    while (BitmapWindow_Exists(window))
    {
        BitmapWindow_Update(window);
        TestDraw(window, size, size);
    }

    BitmapWindow_Destroy(window);

    return 0;
}
