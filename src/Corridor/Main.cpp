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

        auto time = (float)clock() / 1000;
        Camera camera = { Vector3Zero(), time, 0 };
        auto view = MatrixView(&camera);
        {
            Vector3 position = { 0, 0, 0 };
            Vector3 rotation = { 0, 0, 0 };
            Vector3 scale = { 1, 1, 4 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCubeWireframe(world * view, RED);
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