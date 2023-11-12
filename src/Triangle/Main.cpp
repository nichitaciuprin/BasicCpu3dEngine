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

        Vector2Int p0;
        Vector2Int p1;
        Vector2Int p2;

        p0 = { 200, 50 };
        p1 = { 150, 100 };
        p2 = { 250, 150 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, GREEN);

        p0 = { 100, 50 };
        p1 = { 150, 100 };
        p2 = { 50, 150 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, RED);

        p0 = { 200, 250 };
        p1 = { 200, 300 };
        p2 = { 150, 350 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, BLUE);

        p0 = { 200, 250 };
        p1 = { 200, 300 };
        p2 = { 250, 350 };

        bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, GREEN);

        // p0 = { 200, 200 };
        // p1 = { 250, 200 };
        // p2 = { 200, 250 };

        // bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, RED);
        // bitmap->ScreenSpaceDrawTriangle(p2, p1, p0, RED);

        // p2.x = 160.207
        // p2.y = 159.7
        // p1.x = 160.207
        // p1.y = 239.3
        // p0.x = 240.005
        // p0.y = 239.502
        // p2 = { 150, 150 };
        // p1 = { 150, 250 };
        // p0 = { 250, 250 };

        // bitmap->ScreenSpaceDrawTriangle(p0, p1, p2, GREEN);

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