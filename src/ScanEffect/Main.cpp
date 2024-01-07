#include "../Base/Base.h"

int main()
{
    auto width = 50;
    auto height = 200;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    int iteration = 0;

    while (window->Exists())
    {
        CheckFPS();

        int i1 = iteration;
        int i2 = Subgen::StaticNext();
        int pixelCount = width * height;
        i1 %= pixelCount;
        i2 %= pixelCount;
        bitmap->pixels[i1] = Subgen::StaticNext();
        bitmap->pixels[i2] = 0;
        iteration++;

        bitmap->DrawBorder(GREEN);
        window->SetPixels(bitmap);
        window->Update();
    }

    return 0;
}
