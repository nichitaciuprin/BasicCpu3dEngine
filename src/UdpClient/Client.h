unique_ptr<BitmapWindow2> window = nullptr;

void Window32Render(char* frame)
{
    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    if (window == nullptr)
        window = make_unique<BitmapWindow2>(0, 100, size, size);

    window->SetPixelsScaled((uint32_t*)frame, width, height, scale);
    window->Update();
}
void Window32RenderBw(char* frame)
{
    auto width = 32;
    auto height = 32;
    auto scale = 16;
    auto size = 32*16;

    if (window == nullptr)
        window = make_unique<BitmapWindow2>(700, 100, size, size);

    window->SetPixelsScaled2((uint8_t*)frame, width, height, scale);
    window->Update();
}
bool Window32Closed()
{
    return !window->Exists();
}
bool Window32_W() { return window->KeyDown_W(); }
bool Window32_A() { return window->KeyDown_A(); }
bool Window32_S() { return window->KeyDown_S(); }
bool Window32_D() { return window->KeyDown_D(); }
