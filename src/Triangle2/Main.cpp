#include "../Base/Base.h"

void main2()
{
    auto width = 40;
    auto height = 40;
    auto scale = 10;
    auto bitmap = make_unique<Bitmap>(width, height);
    BitmapWindow1::Create(700, 100, width * scale, height * scale);

    int counter = 0;
    bool draw = false;

    while (BitmapWindow1::Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        if (counter == 0)
        {
            counter = 500;
            draw = !draw;
        }
        else
        {
            counter--;
        }

        Vector3 p0;
        Vector3 p1;
        Vector3 p2;

        // p0 = {  1,  38, 0 };
        // p1 = {  20,  1, 0 };
        // p2 = {  38, 20, 0 };
        // bitmap->DrawTriangle4(p0, p1, p2, GREEN);
        // bitmap->DrawTriangleSideCross2(p0, p1, p2, GREEN);
        // bitmap->DrawTriangleSideCross2(p1, p2, p0, GREEN);
        // bitmap->DrawTriangleSideCross2(p2, p0, p1, GREEN);

        // p0 = {   5, 35, 0 };
        // p1 = {   5, 20, 0 };
        // p2 = {  35,  5, 20 };
        // bitmap->DrawTriangle4(p0, p1, p2, GREEN);
        // bitmap->DrawTriangleSideCross2(p0, p1, p2, GREEN);
        // bitmap->DrawTriangleSideCross2(p1, p2, p0, GREEN);
        // bitmap->DrawTriangleSideCross2(p2, p0, p1, GREEN);

        // LEFT
        // p0 = {  20,  5, 0 };
        // p1 = {  35, 20, 0 };
        // p2 = {   5, 35, 0 };
        // bitmap->DrawTriangle4(p0, p1, p2, GREEN);

        // RIGHT
        // p0 = {  20,  5, 0 };
        // p1 = {   5, 20, 0 };
        // p2 = {  35, 35, 0 };
        // bitmap->DrawTriangle4(p0, p1, p2, GREEN);

        // FLAT TOP
        p0 = {   5,  5, 0 };
        p1 = {  35,  5, 0 };
        p2 = {  20, 35, 0 };
        bitmap->DrawTriangle4(p0, p1, p2, GREEN);

        // FLAT BOTTOM
        // p0 = {  20,  5, 0 };
        // p1 = {  35, 35, 0 };
        // p2 = {   5, 35, 0 };
        // bitmap->DrawTriangle4(p0, p1, p2, GREEN);

        if (draw)
        {
            bitmap->SetPixel((int)p0.x, (int)p0.y, RED);
            bitmap->SetPixel((int)p1.x, (int)p1.y, RED);
            bitmap->SetPixel((int)p2.x, (int)p2.y, RED);
            // bitmap->DrawLine3(p0, p2, RED);
            // bitmap->DrawLine3(p0, p1, RED);
            // bitmap->DrawLine3(p1, p2, RED);
            // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p2.x, (int)p2.y, RED);
            // bitmap->DrawLineShort((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, RED);
            // bitmap->DrawLineShort((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, RED);
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