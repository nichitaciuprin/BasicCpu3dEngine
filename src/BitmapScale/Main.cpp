#include "../Base/Base.h"

void main2()
{
    auto width = 10;
    auto height = 10;

    auto scale = 40;

    auto bitmap = make_unique<Bitmap>(width, height);

    BitmapWindow1::Create(700, 100, width * scale, height * scale);

    while (BitmapWindow1::Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        // bitmap->DrawSquare(RED);

        float zFar = 0;
        float zClose = -10;

        {
            Vector3 p0 = { 1, 2, zFar };
            Vector3 p1 = { 8, 2, zClose };
            bitmap->DrawLine(p0, p1, RED);
        }
        {
            Vector3 p0 = { 7, 1, zFar };
            Vector3 p1 = { 7, 8, zClose };
            bitmap->DrawLine(p0, p1, GREEN);
        }
        {
            Vector3 p1 = { 8, 7, zFar };
            Vector3 p0 = { 1, 7, zClose };
            bitmap->DrawLine(p0, p1, BLUE);
        }
        {
            Vector3 p1 = { 2, 8, zFar };
            Vector3 p0 = { 2, 1, zClose };
            bitmap->DrawLine(p0, p1, YELLOW);
        }

        BitmapWindow1::SetPixels2(bitmap, scale);
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