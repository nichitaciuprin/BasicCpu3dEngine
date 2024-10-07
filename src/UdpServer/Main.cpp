#include "Std.h"
#include "StdExt.h"
#include "SysHelper.h"
#include "Subgen.h"
#include "Helper.h"
#include "Models.h"
#include "Clipping.h"
#include "Bitmap.h"
#include "BitmapWindow.h"
#include "NetHelper.h"

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

int main()
{
    auto width = 32;
    auto height = 32;

    char buffer[1024];

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow2>(0, 100, width*16, height*16);

    Camera camera = { 0, 1, 95 };

    while (window->Exists())
    {
        CheckFPS();

        bool w, a, s, d;
        NetServerProcess(buffer, &w, &a, &s, &d);

        UpdateCameraRotation(&camera, 0.023f, window->KeyDown_LEFT(), window->KeyDown_UP(), window->KeyDown_DOWN(), window->KeyDown_RIGHT());
        UpdateCameraPosition(&camera, 0.008f, w, a, s, d, window->KeyDown_E(), window->KeyDown_Q());

        Draw(*bitmap, camera, clock());

        window->SetPixelsScaled(bitmap->pixels.data(), bitmap->Width(), bitmap->Height(), 16);
        window->Update();

        for (int i = 0; i < 1024; i++)
            buffer[i] = PixelToLightValue(bitmap->pixels[i]);

        Halt(10);
    }
}
