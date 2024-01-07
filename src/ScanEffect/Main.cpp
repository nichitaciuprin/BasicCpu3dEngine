#include "../Base/Base.h"

int main()
{
    BitmapWindow1::Create(700, 0, 100, 100);

    auto width = BitmapWindow1::GetClientWidth();
    auto height = BitmapWindow1::GetClientHeight();

    auto bitmap = make_unique<Bitmap>(width,height);

    int iteration = 0;

    while (BitmapWindow1::Exists())
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

        BitmapWindow1::SetPixels(bitmap);
        BitmapWindow1::Update();
    }

    return 0;
}
