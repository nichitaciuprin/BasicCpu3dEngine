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

        Camera camera = { 0, 0, 0 };
        auto view = MatrixView(&camera);

        {
            auto time = (float)clock() / 40;
            Vector3 position = { 0, 0, 2 };
            Vector3 rotation = { 0, time, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube3(world * view);
        }
        {
            Vector3 position = { 0.5f, 0.5f, 2 };
            Vector3 rotation = { 0, 0, 0 };
            Vector3 scale = { 1, 1, 1 };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube3(world * view);
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