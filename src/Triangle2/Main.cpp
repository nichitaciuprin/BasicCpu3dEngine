#include "../Base/Base.h"

void main2()
{
    auto width = 40;
    auto height = 40;

    auto scale2 = 10;

    auto bitmap = make_unique<Bitmap>(width, height);

    BitmapWindow1::Create(700, 100, width * scale2, height * scale2);

    while (BitmapWindow1::Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        // Vector3 p0 = { -0.8f, -0.8f, 0 };
        // Vector3 p2 = {  0.0f,  0.8f, 0 };
        // Vector3 p3 = {  0.7f, -0.1f, 0 };
        // bitmap->DrawTriangle2(p2, p3, p0, GREEN);

        // Vector3 p0 = {  1,  38, 0 };
        // Vector3 p2 = {  20,  1, 0 };
        // Vector3 p3 = {  38, 20, 0 };
        // bitmap->DrawTriangle3(p2, p3, p0, GREEN);

        Vector3 p0 = {  35,  5, 0 };
        Vector3 p1 = {   5, 20, 0 };
        Vector3 p2 = {   5, 35, 0 };
        bitmap->DrawTriangle3(p0, p1, p2, GREEN);

        BitmapWindow1::SetPixels2(bitmap, scale2);
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