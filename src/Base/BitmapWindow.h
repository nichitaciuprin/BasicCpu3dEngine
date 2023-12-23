#pragma once

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

    BitmapWindow(int x, int y, int clientWidth, int clientHeight)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        if (!_windowClassRegistered)
        {
            _windowClassRegistered = true;
            WNDCLASS window_class = {};
            window_class.lpfnWndProc = MessageHandler;
            window_class.hInstance = hInstance;
            window_class.lpszClassName = _windowClassName;
            window_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            RegisterClass(&window_class);
        }

        LONG lStyle = WS_VISIBLE;

        // Makes default window
        // lStyle += WS_OVERLAPPEDWINDOW;

        RECT rect = { 0, 0, (LONG)clientWidth, (LONG)clientHeight };
        AdjustWindowRect(&rect, lStyle, FALSE);
        auto windowWidth = rect.right - rect.left;
        auto windowHeight = rect.bottom - rect.top;

        InitBitmap();

        _hwnd = CreateWindow(_windowClassName, _windowName, lStyle,
                             (LONG)x, (LONG)y, windowWidth, windowHeight,
                             NULL, NULL, hInstance, NULL);

        assert(_hwnd != NULL);

        SetInstance(_hwnd, this);

        // Forces window to update style
        // Setting lStyle before CreateWindow() wont work
        SetWindowLong(_hwnd, GWL_STYLE, lStyle);
        SetWindowPos(_hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    }
    ~BitmapWindow()
    {
        if (!Exists()) return;
        DestroyWindow(_hwnd);
        _hwnd = 0;
    }
    bool Exists() const
    {
        return _hwnd != 0;
    }
    void Update()
    {
        if (!Exists()) return;

        MSG message = {};
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            DispatchMessage(&message);

        InvalidateRect(_hwnd, NULL, FALSE);
        UpdateWindow(_hwnd);
    }
    void SetPixels(const unique_ptr<Bitmap>& bitmap)
    {
        if (!Exists()) return;

        // copy from Top-Down bitmap to Bottom-Up bitmap

        auto width = MathMin(bitmap->Width(), _width);
        auto height = MathMin(bitmap->Height(), _height);

        for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            auto pixel = bitmap->pixels[x + y * width];
            auto y2 = _height - 1 - y;
            _pixels[x + y2 * _width] = pixel;
        }

        // auto size = width * height;

        // for (int i = 0; i < size; i++)
        // {
        //     auto pixel = bitmap->pixels[i];
        //     auto y = i % _height;
        //     auto x = i % y;
        //     y = _height - y;
        //     _pixels[x * y] = pixel;
        // }
    }
    void SetPixels2(const unique_ptr<Bitmap>& bitmap, int scale)
    {
        if (!Exists()) return;

        auto width = bitmap->Width();
        auto height = bitmap->Height();

        for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            auto pixel = bitmap->pixels[x + y * width];
            auto x2 = x * scale;
            auto y2 = y * scale;
            for (int i = 0; i < scale; i++)
            for (int j = 0; j < scale; j++)
                SetPixel(x2+i, y2+j, pixel);
        }
    }
    int GetClientWidth() const
    {
        return _width;
    }
    int GetClientHeight() const
    {
        return _height;
    }
    void SetPixel(int x, int y, Pixel pixel)
    {
        // window bitmap is bottom-up
        y = _height - 1 - y;
        _pixels[x + y * _width] = pixel;
    }

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

    void InitBitmap()
    {
        _hdc = CreateCompatibleDC(0);
    }
    void ResetBitmap(int clientWidth, int clientHeight)
    {
        BITMAPINFO bitmapinfo = {};
        bitmapinfo.bmiHeader.biSize = sizeof(bitmapinfo.bmiHeader);
        bitmapinfo.bmiHeader.biPlanes = 1;
        bitmapinfo.bmiHeader.biBitCount = 32;
        bitmapinfo.bmiHeader.biCompression = BI_RGB;
        bitmapinfo.bmiHeader.biWidth  = clientWidth;
        bitmapinfo.bmiHeader.biHeight = clientHeight;

        if (_hbitmap)
            DeleteObject(_hbitmap);

        _hbitmap = CreateDIBSection(NULL, &bitmapinfo, DIB_RGB_COLORS, (void**)&_pixels, 0, 0);
        assert(_hbitmap != nullptr);
        SelectObject(_hdc, _hbitmap);

        _width  = clientWidth;
        _height = clientHeight;
    }
    void PaintBitmap()
    {
        PAINTSTRUCT paint;

        HDC device_context = BeginPaint(_hwnd, &paint);

        BitBlt(device_context,
                paint.rcPaint.left, paint.rcPaint.top,
                paint.rcPaint.right - paint.rcPaint.left,
                paint.rcPaint.bottom - paint.rcPaint.top,
                _hdc,
                paint.rcPaint.left, paint.rcPaint.top,
                SRCCOPY);

        EndPaint(_hwnd, &paint);
    }

    static void SetInstance(HWND hwnd, BitmapWindow* window)
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    static BitmapWindow* GetInstance(HWND hwnd)
    {
        return reinterpret_cast<BitmapWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto bitmapWindow = GetInstance(hwnd);

        if (bitmapWindow == nullptr)
            return DefWindowProc(hwnd, message, wParam, lParam);

        switch(message)
        {
            case WM_QUIT:
            case WM_DESTROY:
            {
                bitmapWindow->_hwnd = 0;
                break;
            }
            case WM_PAINT:
            {
                bitmapWindow->PaintBitmap();
                break;
            }
            case WM_SIZE:
            {
                int clientWidth = LOWORD(lParam);
                int clientHeight = HIWORD(lParam);

                auto sizeChanged =
                    bitmapWindow->_width != clientWidth ||
                    bitmapWindow->_height != clientHeight;

                if (sizeChanged)
                    bitmapWindow->ResetBitmap(clientWidth, clientHeight);

                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                bool isDown = (message == WM_KEYDOWN);
                switch (wParam)
                {
                    case VK_ESCAPE : { DestroyWindow(hwnd);                     break; }
                    case 'W'       : { bitmapWindow->keydown_W        = isDown; break; }
                    case 'A'       : { bitmapWindow->keydown_A        = isDown; break; }
                    case 'S'       : { bitmapWindow->keydown_S        = isDown; break; }
                    case 'D'       : { bitmapWindow->keydown_D        = isDown; break; }
                    case 'E'       : { bitmapWindow->keydown_E        = isDown; break; }
                    case 'Q'       : { bitmapWindow->keydown_Q        = isDown; break; }
                    case VK_UP     : { bitmapWindow->keydown_VK_UP    = isDown; break; }
                    case VK_LEFT   : { bitmapWindow->keydown_VK_LEFT  = isDown; break; }
                    case VK_DOWN   : { bitmapWindow->keydown_VK_DOWN  = isDown; break; }
                    case VK_RIGHT  : { bitmapWindow->keydown_VK_RIGHT = isDown; break; }
                    default        : {                                          break; }
                }
                break;
            }
            case WM_GETMINMAXINFO:
            {
                // Sets windows size defaults
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                lpMMI->ptMinTrackSize.x = 10;
                lpMMI->ptMinTrackSize.y = 10;
                break;
            }
            default: return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }
};

bool           BitmapWindow::_windowClassRegistered = false;
const LPCWSTR  BitmapWindow::_windowClassName = L"WindowClass1";
const LPCWSTR  BitmapWindow::_windowName = L"WindowName1";