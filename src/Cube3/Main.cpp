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

        auto time = (float)clock() / 4000;
        Camera camera = { { 0, 1.5f, 0 }, time, 0 };
        auto view = MatrixView(&camera);
        Vector3 rotation = { 0, 0, 0 };
        Vector3 scale = { 1, 1, 1 };

        for (int x = -5; x < 5; x++)
        for (int z = -5; z < 5; z++)
        {
            if (abs(x) != 4 && abs(z) != 4) continue;
            Vector3 position = { (float)x, -0.5f, (float)z };
            auto world = MatrixWorld(position, rotation, scale);
            bitmap->DrawCube1(world * view);
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