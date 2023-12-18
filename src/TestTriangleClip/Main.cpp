#include "../Base/Base.h"

void main2()
{
    // int int1 = 1;
    // int int2 = 2;
    // int& r1 = int1;
    // int& r2 = int2;
    // // r1 = r2;
    // swap(r1, r2);
    // cout << int1 << endl;
    // cout << int2 << endl;
    // cout << r1 << endl;
    // cout << r2 << endl;
    // return;

    auto width = 400;
    auto height = 400;

    auto bitmap = make_unique<Bitmap>(width, height);
    auto window = make_unique<BitmapWindow>(700, 100, width, height);

    Camera camera = { 0, 0, 0 };

    while (window->Exists())
    {
        CheckFPS();
        bitmap->Fill(BLACK);

        UpdateCameraRotation
        (
            &camera, 0.001f,
            window->keydown_VK_LEFT,
            window->keydown_VK_UP,
            window->keydown_VK_DOWN,
            window->keydown_VK_RIGHT
        );

        UpdateCameraPosition
        (
            &camera, 0.0002f,
            window->keydown_W, window->keydown_A, window->keydown_S, window->keydown_D,
            window->keydown_E, window->keydown_Q
        );

        auto view = MatrixView(&camera);

        {
            // Vector3 p0 = { -1,  0,  1 }; p0 *= view;
            // Vector3 p1 = {  1,  1,  1 }; p1 *= view;
            // Vector3 p2 = {  1, -1,  1 }; p2 *= view;
            // Vector3 p2 = { -1,  0,  1 }; p2 *= view;
            // Vector3 p0 = {  1,  1,  1 }; p0 *= view;
            // Vector3 p1 = {  1, -1,  1 }; p1 *= view;
            // Vector3 p1 = { -1,  0,  1 }; p1 *= view;
            // Vector3 p2 = {  1,  1,  1 }; p2 *= view;
            // Vector3 p0 = {  1, -1,  1 }; p0 *= view;
            // bitmap->DrawTriangle1(p0, p1, p2, WHITE);
        }
        {
            Vector3 p0 = {  1,  0,  1 }; p0 *= view;
            Vector3 p1 = { -1, -1,  1 }; p1 *= view;
            Vector3 p2 = { -1,  1,  1 }; p2 *= view;
            // Vector3 p2 = {  1,  0,  1 }; p2 *= view;
            // Vector3 p0 = { -1, -1,  1 }; p0 *= view;
            // Vector3 p1 = { -1,  1,  1 }; p1 *= view;
            // Vector3 p1 = {  1,  0,  1 }; p1 *= view;
            // Vector3 p2 = { -1, -1,  1 }; p2 *= view;
            // Vector3 p0 = { -1,  1,  1 }; p0 *= view;
            bitmap->DrawTriangle1(p0, p1, p2, WHITE);
        }

        // Vector3 p0 = { -0.438564f,  0.692213f,  1.444640f };
        // Vector3 p1 = {  1.428180f,  0.000000f,  0.904786f };
        // Vector3 p2 = { -0.438564f, -0.692213f,  1.444640f };
        // vector<Vector3> in;
        // vector<Vector3> out;
        // in.reserve(6);
        // out.reserve(6);
        // in.push_back(p0);
        // in.push_back(p1);
        // in.push_back(p2);
        // // ClipPoligon(in, out);
        // for (auto& point : in) Vector3Print(point);
        // ClipPoligonRight(in, out);
        // bitmap->DrawPoligon2(out, RED);

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