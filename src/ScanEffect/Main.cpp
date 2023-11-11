#include "../Base/Base.h"

int main()
{
    BitmapWindow1::Create(700,0,100,100);
    auto width = BitmapWindow1::GetClientWidth();
    auto height = BitmapWindow1::GetClientHeight();

    auto bitmap = make_unique<Bitmap>(width,height);

    size_t animationIteration = 0;

    while (BitmapWindow1::Exists())
    {
        CheckFPS();

        bitmap->ScanEffectRandom(animationIteration);
        animationIteration++;
        bitmap->DrawBorder(GREEN);

        BitmapWindow1::SetPixels(bitmap);

        BitmapWindow1::Update();
    }

    return 0;
}
