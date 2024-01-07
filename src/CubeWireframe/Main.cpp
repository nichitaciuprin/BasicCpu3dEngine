#include "../Base/Base.h"

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

        auto time = (float)clock() / 40;
        Camera camera = { 0, 0, 0 };
        auto view = MatrixView(camera);
        Vector3 position = { 0, 0, 2 };
        Vector3 rotation = { 0, time, 0 };
        Vector3 scale = { 1, 1, 1 };
        auto world = MatrixWorld(position, rotation, scale);
        bitmap->DrawCubeWireframe(world * view, RED);

        bitmap->DrawBorder(GREEN);
        window->SetPixels(bitmap);
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