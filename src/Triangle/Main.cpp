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
        Vector2Int p3;

        // MAIN
        {
            p0 = { 250,  50 };
            p1 = { 200, 100 };
            p2 = { 300, 150 };
            bitmap->DrawTriangle3(p0, p1, p2, RED);

            p0 = { 150,  50 };
            p1 = { 200, 100 };
            p2 = { 100,  150 };
            bitmap->DrawTriangle3(p0, p1, p2, GREEN);
        }

        // SQUARES
        {
            p0 = {  0,  0 };
            p1 = { 25,  0 };
            p2 = { 25, 25 };
            p3 = {  0, 25 };
            bitmap->DrawTriangle3(p2, p3, p0, RED);
            bitmap->DrawTriangle3(p0, p1, p2, GREEN);

            p0 = {  0 + 50,  0 };
            p1 = { 25 + 50,  0 };
            p2 = { 25 + 50, 25 };
            p3 = {  0 + 50, 25 };
            bitmap->DrawTriangle3(p0, p3, p2, BLUE);
            bitmap->DrawTriangle3(p1, p0, p2, YELLOW);
        }

        // BIRDS
        {
            p0 = { 200, 250 };

            p1 = { 200, 300 };
            p2 = { 150, 350 }; bitmap->DrawTriangle3(p0, p1, p2, RED);
            p2 = { 250, 350 }; bitmap->DrawTriangle3(p0, p2, p1, RED);

            p1 = { 250, 250 };
            p2 = { 300, 300 }; bitmap->DrawTriangle3(p1, p0, p2, GREEN);
            p2 = { 300, 200 }; bitmap->DrawTriangle3(p1, p2, p0, GREEN);

            p1 = { 150, 250 };
            p2 = { 100, 200 }; bitmap->DrawTriangle3(p0, p1, p2, BLUE);
            p2 = { 100, 300 }; bitmap->DrawTriangle3(p2, p0, p1, BLUE);

            p1 = { 200, 200 };
            p2 = { 150, 150 }; bitmap->DrawTriangle3(p0, p1, p2, MAGENTA);
            p2 = { 250, 150 }; bitmap->DrawTriangle3(p0, p1, p2, MAGENTA);
        }

        // ROMB
        {
            p1 = { 100,  50 };
            p2 = {  25, 125 };
            p0 = {  50,  75 }; bitmap->DrawTriangle3(p0, p1, p2, ORANGE);
            p0 = {  75, 100 }; bitmap->DrawTriangle3(p0, p1, p2, ORANGE);
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