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

        float zFar = 10;
        float zClose = 9;

        {
            Vector3 p0 = { -1,  3,  zFar };
            Vector3 p1 = {  0,  3,  zFar };
            Vector3 p2 = {  2, -3,  zClose };
            p0 /= 10;
            p1 /= 10;
            p2 /= 10;
            bitmap->DrawTriangle1(p0, p1, p2, RED);
        }
        {
            Vector3 p0 = {  3, -1,  zFar };
            Vector3 p1 = {  3, -2,  zFar };
            Vector3 p2 = { -3, -1,  zClose };
            p0 /= 10;
            p1 /= 10;
            p2 /= 10;
            bitmap->DrawTriangle1(p0, p1, p2, GREEN);
        }
        {
            Vector3 p0 = { -2, -2,  zFar };
            Vector3 p1 = { -3, -2,  zFar };
            Vector3 p2 = {  1,  3,  zClose };
            p0 /= 10;
            p1 /= 10;
            p2 /= 10;
            bitmap->DrawTriangle1(p0, p1, p2, BLUE);
        }

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