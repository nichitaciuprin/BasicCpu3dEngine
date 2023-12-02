#include "../Base/Base.h"

void main2()
{
    auto width = 40;
    auto height = 40;

    auto scale2 = 10;

    auto bitmap = make_unique<Bitmap>(width, height);

    BitmapWindow1::Create(700, 100, width * scale2, height * scale2);

    int counter = 0;
    bool draw = false;

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

        // Vector3 p0 = { 0, 0, 0 };
        // Vector3 p1 = { 4, 1, 0 };
        // bitmap->DrawLine3(p1, p0, WHITE);
        // bitmap->DrawLine3(p0, p1, WHITE);
        // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, RED);
        // bitmap->DrawLineShort((int)p1.x, (int)p1.y, (int)p0.x, (int)p0.y, RED);

        Vector3 p0 = {   5, 35, 0 };
        Vector3 p1 = {   5, 20, 0 };
        Vector3 p2 = {  35,  5, 20 };
        bitmap->DrawTriangle3(p0, p1, p2, GREEN);
        // bitmap->DrawLine4(p0, p2, WHITE);
        // bitmap->DrawLine4(p0, p1, WHITE);
        // bitmap->DrawLine4(p1, p2, WHITE);

        if (counter == 0)
        {
            counter = 500;
            draw = !draw;
        }
        else
        {
            counter--;
        }

        if (draw)
        {
            bitmap->SetPixel((int)p0.x, (int)p0.y, RED);
            bitmap->SetPixel((int)p1.x, (int)p1.y, RED);
            bitmap->SetPixel((int)p2.x, (int)p2.y, RED);
        }


        // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p2.x, (int)p2.y, RED);
        // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, RED);
        // bitmap->DrawLineShort((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, RED);

        // Vector3 p0 = {  20,  5, 0 };
        // Vector3 p1 = {  35, 20, 0 };
        // Vector3 p2 = {   5, 35, 0 };
        // bitmap->DrawTriangle3(p0, p1, p2, GREEN);
        // bitmap->DrawLine4(p0, p2, WHITE);
        // bitmap->DrawLine4(p0, p1, WHITE);
        // bitmap->DrawLine4(p1, p2, WHITE);

        // bitmap->DrawLine3(p0, p2, RED);
        // bitmap->DrawLine3(p0, p1, RED);
        // bitmap->DrawLine3(p1, p2, RED);
        // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p2.x, (int)p2.y, WHITE);
        // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, WHITE);
        // bitmap->DrawLineShort((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, WHITE);
        // bitmap->DrawTriangleBorder(p0, p1, p2, GREEN);

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