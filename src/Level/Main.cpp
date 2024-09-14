#include "BaseOld.h"

void main2()
{
    // auto width = 512;
    // auto height = 512;

    // auto bitmap = make_unique<Bitmap>(width, height);
    // auto window = make_unique<BitmapWindow>(700, 100, width, height);

    // Camera camera = { 0, 1, 95 };

    // while (window->Exists())
    // {
    //     CheckFPS();
    //     bitmap->Fill(BLACK);

    //     auto state = window->GetInputState();
    //     UpdateCameraRotation(camera, state, 0.00080f);
    //     UpdateCameraPosition(camera, state, 0.00020f);

    //     auto view = MatrixView(camera);

    //     {
    //         Vector3 p0 = { -1, 0,  2 }; p0 *= view;
    //         Vector3 p1 = { -1, 0, 95 }; p1 *= view;
    //         Vector3 p2 = {  1, 0, 95 }; p2 *= view;
    //         Vector3 p3 = {  1, 0,  2 }; p3 *= view;
    //         bitmap->DrawPoligon1(p0, p1, p2, p3, WHITE);
    //     }
    //     {
    //         float size = 5;
    //         Vector3 p0 = { -size, 0, -size + 100 }; p0 *= view;
    //         Vector3 p1 = { -size, 0,  size + 100 }; p1 *= view;
    //         Vector3 p2 = {  size, 0,  size + 100 }; p2 *= view;
    //         Vector3 p3 = {  size, 0, -size + 100 }; p3 *= view;
    //         bitmap->DrawPoligon1(p0, p1, p2, p3, WHITE);
    //     }

    //     bitmap->ApplyBlackWhiteColorDepth();

    //     {
    //         auto time = (float)clock() / 40;
    //         Vector3 position = { 0, 0.5f, 100 };
    //         Vector3 rotation = { 0, time, 0 };
    //         Vector3 scale = { 1, 1, 1 };
    //         auto world = MatrixWorld(position, rotation, scale);
    //         bitmap->DrawCubeColored(world * view);
    //     }
    //     {
    //         auto time = (float)clock() / 20;
    //         Vector3 position = { 0, 1.5f, 100 };
    //         Vector3 rotation = { 0, time, 0 };
    //         Vector3 scale = { 1, 1, 1 };
    //         auto world = MatrixWorld(position, rotation, scale);
    //         bitmap->DrawCubeColored(world * view);
    //     }

    //     window->SetPixels(bitmap);
    //     window->Update();
    // }
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cout << "EXCEPTION" << endl;
        cerr << e.what() << endl;
    }

    return 0;
}