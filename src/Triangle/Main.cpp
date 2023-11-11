#include "../Base/Base.h"

void main2()
{
    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);

    BitmapWindow1::Create(700, 100, width, height);

    while (BitmapWindow1::Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        // int height = 300

        Vector3 p0 = { 200, 50, 0 };
        Vector3 p1 = { 150, 100, 0 };
        Vector3 p2 = { 250, 150, 0 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, GREEN);

        p1 = { 250, 100, 0 };
        p2 = { 150, 150, 0 };
        p0.x -= 100;
        p1.x -= 100;
        p2.x -= 100;

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, RED);

        p0 = { 200, 50  + 200, 0 };
        p1 = { 200, 100 + 200, 0 };
        p2 = { 150, 150 + 200, 0 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, BLUE);

        p0 = { 200, 50  + 200, 0 };
        p1 = { 200, 100 + 200, 0 };
        p2 = { 250, 150 + 200, 0 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, GREEN);

        bitmap->DrawBorder(GREEN);
        BitmapWindow1::SetPixels(bitmap);
        BitmapWindow1::Update();
    }
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}