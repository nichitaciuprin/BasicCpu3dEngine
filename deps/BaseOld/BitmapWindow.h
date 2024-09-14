#pragma once

struct InputState
{
    bool w, a, s, d;
    bool left, up, down, right;
    bool e, q;
};

class BitmapWindow
{
public:
    bool keydown_W = false;
    bool keydown_A = false;
    bool keydown_S = false;
    bool keydown_D = false;
    bool keydown_E = false;
    bool keydown_Q = false;
    bool keydown_VK_UP = false;
    bool keydown_VK_LEFT = false;
    bool keydown_VK_DOWN = false;
    bool keydown_VK_RIGHT = false;

    BitmapWindow(int x, int y, int clientWidth, int clientHeight);
    ~BitmapWindow();

    bool Exists() const;
    void Update();
    void SetPixels(const unique_ptr<Bitmap>& bitmap);
    void SetPixelsScaled(const unique_ptr<Bitmap>& bitmap, int scale);

    int GetClientWidth() const;
    int GetClientHeight() const;

    void SetPixel(int x, int y, Pixel pixel);

    InputState GetInputState();

private:
    static bool           _windowClassRegistered;
    static const LPCWSTR  _windowClassName;
    static const LPCWSTR  _windowName;

    HWND       _hwnd;

    HDC        _hdc;
    HBITMAP    _hbitmap;
    uint32_t*  _pixels;
    int        _width;
    int        _height;

    InputState inputState = {};

    void InitBitmap();
    void ResetBitmap(int clientWidth, int clientHeight);
    void PaintBitmap();

    static void SetInstance(HWND hwnd, BitmapWindow* window);
    static BitmapWindow* GetInstance(HWND hwnd);
    static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
