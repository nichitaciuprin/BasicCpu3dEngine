#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"
#include "Subgen.h"
#include "Helper.h"
#include "Models.h"
#include "Clipping.h"
#include "Bitmap.h"
#include "SysWindow.h"

void main2()
{
    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    while (window->Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        Vector3 p0;
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;

        // MAIN
        {
            p0 = { 250,  50 };
            p1 = { 200, 100 };
            p2 = { 300, 150 };
            bitmap->DrawTriangle(p0, p1, p2, RED);

            p0 = { 150,  50 };
            p1 = { 200, 100 };
            p2 = { 100,  150 };
            bitmap->DrawTriangle(p0, p1, p2, GREEN);
        }

        // SQUARES
        {
            p0 = {  0,  0 };
            p1 = { 25,  0 };
            p2 = { 25, 25 };
            p3 = {  0, 25 };
            bitmap->DrawTriangle(p2, p3, p0, RED);
            bitmap->DrawTriangle(p0, p1, p2, GREEN);

            p0 = {  0 + 50,  0 };
            p1 = { 25 + 50,  0 };
            p2 = { 25 + 50, 25 };
            p3 = {  0 + 50, 25 };
            bitmap->DrawTriangle(p0, p3, p2, BLUE);
            bitmap->DrawTriangle(p1, p0, p2, YELLOW);
        }

        // MALTA CROSS
        {
            p0 = { 200, 250 };

            p1 = { 150, 250 };
            p2 = { 100, 200 }; bitmap->DrawTriangle(p0, p1, p2, RED);
            p2 = { 100, 300 }; bitmap->DrawTriangle(p2, p0, p1, RED);

            p1 = { 250, 250 };
            p2 = { 300, 300 }; bitmap->DrawTriangle(p1, p0, p2, GREEN);
            p2 = { 300, 200 }; bitmap->DrawTriangle(p1, p2, p0, GREEN);

            p1 = { 200, 300 };
            p2 = { 150, 350 }; bitmap->DrawTriangle(p0, p1, p2, BLUE);
            p2 = { 250, 350 }; bitmap->DrawTriangle(p0, p2, p1, BLUE);

            p1 = { 200, 200 };
            p2 = { 150, 150 }; bitmap->DrawTriangle(p0, p1, p2, MAGENTA);
            p2 = { 250, 150 }; bitmap->DrawTriangle(p0, p1, p2, MAGENTA);
        }

        // ROMB
        {
            p1 = { 100,  50 };
            p2 = {  25, 125 };
            p0 = {  50,  75 }; bitmap->DrawTriangle(p0, p1, p2, ORANGE);
            p0 = {  75, 100 }; bitmap->DrawTriangle(p0, p1, p2, ORANGE);
        }

        bitmap->DrawBorder(GREEN);
        window->SetPixels(bitmap->pixels.data(), bitmap->Width(), bitmap->Height());
        window->Update();
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