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

        {
            auto time = ((float)clock() / 4000) - 1;
            float zFar = 0;
            float zClose = -1;
            {
                Vector3 p0 = { -0.2f,  0.6f,  zFar };
                Vector3 p1 = {  0.0f,  0.6f,  zFar };
                Vector3 p2 = {  0.4f, -0.6f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, RED);
            }
            {
                Vector3 p0 = {  0.6f, -0.2f,  zFar };
                Vector3 p1 = {  0.6f, -0.4f,  zFar };
                Vector3 p2 = { -0.8f, -0.2f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, GREEN);
            }
            {
                Vector3 p0 = { -0.4f, -0.4f,  zFar };
                Vector3 p1 = { -0.6f, -0.4f,  zFar };
                Vector3 p2 = {  0.2f,  0.6f,  zClose };
                bitmap->DrawTriangle2(p0, p1, p2, BLUE);
            }
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