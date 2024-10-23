#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"
#include "Subgen.h"
#include "Helper.h"
#include "Models.h"
#include "Clipping.h"
#include "Bitmap.h"
#include "SysWindow.h"

void Draw(Bitmap& bitmap, Camera camera, long time)
{
    bitmap.Fill(BLACK);

    auto view = MatrixView(&camera);

    {
        Vector3 p0 = { -1, 0,  2 }; p0 *= view;
        Vector3 p1 = { -1, 0, 95 }; p1 *= view;
        Vector3 p2 = {  1, 0, 95 }; p2 *= view;
        Vector3 p3 = {  1, 0,  2 }; p3 *= view;
        bitmap.DrawPoligon1(p0, p1, p2, p3, WHITE);
    }
    {
        float size = 5;
        Vector3 p0 = { -size, 0, -size + 100 }; p0 *= view;
        Vector3 p1 = { -size, 0,  size + 100 }; p1 *= view;
        Vector3 p2 = {  size, 0,  size + 100 }; p2 *= view;
        Vector3 p3 = {  size, 0, -size + 100 }; p3 *= view;
        bitmap.DrawPoligon1(p0, p1, p2, p3, WHITE);
    }

    // TODO fix this
    // bitmap->ApplyBlackWhiteColorDepth();

    {
        auto time2 = (float)time / 600;
        Vector3 position = { 0, 0.5f, 100 };
        Vector3 rotation = { 0, time2, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap.DrawCubeColored(world * view);
    }
    {
        auto time2 = (float)time / 300;
        Vector3 position = { 0, 1.5f, 100 };
        Vector3 rotation = { 0, time2, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap.DrawCubeColored(world * view);
    }
}

unique_ptr<Bitmap> testBitmap;
unique_ptr<BitmapWindow2> testWindow;
bool TestRenderCalled = false;
void TestRender()
{
    if (!TestRenderCalled)
    {
        TestRenderCalled = true;

        // int x, y;
        // GetConsolePosition(&x, &y);

        testBitmap = make_unique<Bitmap>(512, 512);
        testWindow = make_unique<BitmapWindow2>(0, 0, 512, 512);

        return;
    }

    Camera camera = { 0, 1, 95 };
    Draw(*testBitmap, camera, GetTime());
    // testBitmap->DrawBorder(GREEN);
    testWindow->SetPixels(testBitmap->pixels.data(), 32*16, 32*16);
    testWindow->Update();
}


int main()
{
    auto scale = 16;
    auto size1 = 32;
    auto size2 = 32*scale;

    unique_ptr<Bitmap> bitmapNet = make_unique<Bitmap>(size1, size1);

    Camera camera;

    while (true)
    {
        FixedTimeStart();

        // cout << "!" << endl;

        // Halt(1000);

        TestRender();

        FixedTimeEnd();
    }

    return 0;
}
