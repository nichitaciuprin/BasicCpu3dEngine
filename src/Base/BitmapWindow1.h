#pragma once

class BitmapWindow1
{
public:
    static void Create(int x, int y, int clientWidth, int clientHeight)
    {
        if (instance != nullptr) return;
        instance = make_unique<BitmapWindow>(x, y, clientWidth, clientHeight);
    }
    static void Destroy()
    {
        if (instance == nullptr) return;
        instance.release();
    }
    static bool Exists()
    {
        if (instance == nullptr) return false;
        return instance->Exists();
    }
    static void Update()
    {
        if (instance == nullptr) return;
        instance->Update();
    }
    static void SetPixels(const unique_ptr<Bitmap>& bitmap)
    {
        if (instance == nullptr) return;
        instance->SetPixels(bitmap);
    }
    static uint32_t GetClientWidth()
    {
        if (instance == nullptr) return 0;
        return instance->GetClientWidth();
    }
    static uint32_t GetClientHeight()
    {
        if (instance == nullptr) return 0;
        return instance->GetClientHeight();
    }

private:
    static unique_ptr<BitmapWindow> instance;
};

unique_ptr<BitmapWindow> BitmapWindow1::instance = nullptr;