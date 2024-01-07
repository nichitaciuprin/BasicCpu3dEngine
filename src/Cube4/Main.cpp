#include "../Base/Base.h"

void main2()
{
    auto width = 600;
    auto height = 600;

    auto scale2 = 1;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    while (window->Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        auto time = (float)clock() / 50;
        // auto time = 0;
        Camera camera = { 0, 0, 0 };
        auto view = MatrixView(&camera);
        Vector3 position = { 0, 0, 2 };
        // Vector3 rotation = { time, time, time*2 };
        Vector3 rotation = { 0, time, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap->DrawCubeColored(world * view);
        // bitmap->DrawCube3(world * view);

        // bitmap->DrawBorder(GREEN);
        window->SetPixelsScaled(bitmap, scale2);
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