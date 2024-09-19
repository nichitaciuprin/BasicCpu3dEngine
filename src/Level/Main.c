// #include "deps2.h"

// typedef struct BitmapWindow
// {
//     _Bool keydown_W;
//     _Bool keydown_A;
//     _Bool keydown_S;
//     _Bool keydown_D;
//     _Bool keydown_E;
//     _Bool keydown_UP;
//     _Bool keydown_Q;
//     _Bool keydown_LEFT;
//     _Bool keydown_DOWN;
//     _Bool keydown_RIGHT;

//     HWND       _hwnd;

//     HDC        _hdc;
//     HBITMAP    _hbitmap;
//     uint32_t*  _pixels;
//     int        _width;
//     int        _height;
// }
// BitmapWindow;

// _Bool _BitmapWindow_Registered = 0;

// void _BitmapWindow_InitBitmap(BitmapWindow* instance)
// {
//     instance->_hdc = CreateCompatibleDC(0);
// }
// void _BitmapWindow_ResetBitmap(BitmapWindow* instance, int clientWidth, int clientHeight)
// {
//     BITMAPINFO bitmapinfo = {};
//     bitmapinfo.bmiHeader.biSize = sizeof(bitmapinfo.bmiHeader);
//     bitmapinfo.bmiHeader.biPlanes = 1;
//     bitmapinfo.bmiHeader.biBitCount = 32;
//     bitmapinfo.bmiHeader.biCompression = BI_RGB;
//     bitmapinfo.bmiHeader.biWidth  = clientWidth;
//     bitmapinfo.bmiHeader.biHeight = clientHeight;

//     if (instance->_hbitmap)
//         DeleteObject(instance->_hbitmap);

//     instance->_hbitmap = CreateDIBSection(NULL, &bitmapinfo, DIB_RGB_COLORS, (void**)&instance->_pixels, 0, 0);
//     assert(instance->_hbitmap != 0);
//     SelectObject(instance->_hdc, instance->_hbitmap);

//     instance->_width  = clientWidth;
//     instance->_height = clientHeight;
// }
// void _BitmapWindow_PaintBitmap(BitmapWindow* instance)
// {
//     PAINTSTRUCT paint;

//     HDC device_context = BeginPaint(instance->_hwnd, &paint);

//     BitBlt(device_context,
//             paint.rcPaint.left, paint.rcPaint.top,
//             paint.rcPaint.right - paint.rcPaint.left,
//             paint.rcPaint.bottom - paint.rcPaint.top,
//             instance->_hdc,
//             paint.rcPaint.left, paint.rcPaint.top,
//             SRCCOPY);

//     EndPaint(instance->_hwnd, &paint);
// }

// LRESULT __stdcall _BitmapWindow_MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     BitmapWindow* instance = (BitmapWindow*)(GetWindowLongPtrA(hwnd, GWLP_USERDATA));

//     if (instance == NULL)
//         return DefWindowProcA(hwnd, message, wParam, lParam);

//     switch(message)
//     {
//         case WM_QUIT:
//         case WM_DESTROY:
//         {
//             instance->_hwnd = 0;
//             break;
//         }
//         case WM_PAINT:
//         {
//             _BitmapWindow_PaintBitmap(instance);
//             break;
//         }
//         case WM_SIZE:
//         {
//             int clientWidth = LOWORD(lParam);
//             int clientHeight = HIWORD(lParam);

//             _Bool sizeChanged =
//                 instance->_width != clientWidth ||
//                 instance->_height != clientHeight;

//             if (sizeChanged)
//                 _BitmapWindow_ResetBitmap(instance, clientWidth, clientHeight);

//             break;
//         }
//         case WM_KEYDOWN:
//         case WM_KEYUP:
//         {
//             _Bool isDown = (message == WM_KEYDOWN);

//             switch (wParam)
//             {
//                 case VK_ESCAPE : { DestroyWindow(hwnd);              break; }
//                 case 'W'       : { instance->keydown_W     = isDown; break; }
//                 case 'A'       : { instance->keydown_A     = isDown; break; }
//                 case 'S'       : { instance->keydown_S     = isDown; break; }
//                 case 'D'       : { instance->keydown_D     = isDown; break; }
//                 case 'E'       : { instance->keydown_E     = isDown; break; }
//                 case 'Q'       : { instance->keydown_Q     = isDown; break; }
//                 case VK_UP     : { instance->keydown_UP    = isDown; break; }
//                 case VK_LEFT   : { instance->keydown_LEFT  = isDown; break; }
//                 case VK_DOWN   : { instance->keydown_DOWN  = isDown; break; }
//                 case VK_RIGHT  : { instance->keydown_RIGHT = isDown; break; }
//                 default        : {                                   break; }
//             }
//             break;
//         }
//         case WM_GETMINMAXINFO:
//         {
//             // Sets windows size defaults
//             LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
//             lpMMI->ptMinTrackSize.x = 10;
//             lpMMI->ptMinTrackSize.y = 10;
//             break;
//         }
//         default: return DefWindowProcA(hwnd, message, wParam, lParam);
//     }

//     return 0;
// }

// BitmapWindow* BitmapWindow_Create(int x, int y, int clientWidth, int clientHeight)
// {
//     BitmapWindow* instance = (BitmapWindow*)malloc(sizeof(BitmapWindow));

//     instance->keydown_W = 0;
//     instance->keydown_A = 0;
//     instance->keydown_S = 0;
//     instance->keydown_D = 0;
//     instance->keydown_E = 0;
//     instance->keydown_Q = 0;
//     instance->keydown_UP = 0;
//     instance->keydown_LEFT = 0;
//     instance->keydown_DOWN = 0;
//     instance->keydown_RIGHT = 0;

//     instance->_hwnd = 0;

//     instance->_hdc = 0;
//     instance->_hbitmap = 0;
//     instance->_pixels = 0;
//     instance->_width = 0;
//     instance->_height = 0;

//     HINSTANCE hInstance = GetModuleHandleA(0);

//     const char* className = "BitmapWindowClass";

//     if (!_BitmapWindow_Registered)
//     {
//         _BitmapWindow_Registered = 1;
//         WNDCLASSA window_class = {};
//         window_class.lpfnWndProc = _BitmapWindow_MessageHandler;
//         window_class.hInstance = hInstance;
//         window_class.lpszClassName = className;
//         window_class.hCursor = LoadCursorA(0, IDC_ARROW);
//         RegisterClassA(&window_class);
//     }

//     long lStyle = WS_VISIBLE;

//     // Makes default window
//     // lStyle += WS_OVERLAPPEDWINDOW;

//     RECT rect = { 0, 0, (long)clientWidth, (long)clientHeight };
//     AdjustWindowRect(&rect, lStyle, false);
//     int windowWidth = rect.right - rect.left;
//     int windowHeight = rect.bottom - rect.top;

//     _BitmapWindow_InitBitmap(instance);
//     _BitmapWindow_ResetBitmap(instance, clientWidth, clientHeight);

//     instance->_hwnd = CreateWindowExA
//     (
//         0,
//         className, "BitmapWindow", lStyle,
//         (long)x, (long)y, windowWidth, windowHeight,
//         NULL, NULL, hInstance, NULL
//     );

//     assert(instance->_hwnd != NULL);

//     SetWindowLongPtrA(instance->_hwnd, GWLP_USERDATA, (int64_t)(instance));

//     // Forces window to update style
//     // Setting lStyle before CreateWindow() wont work
//     SetWindowLongA(instance->_hwnd, GWL_STYLE, lStyle);
//     SetWindowPos(instance->_hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

//     return instance;
// }
// _Bool BitmapWindow_Exists(BitmapWindow* instance)
// {
//     return instance->_hwnd != 0;
// }
// void BitmapWindow_Destroy(BitmapWindow* instance)
// {
//     if (!BitmapWindow_Exists(instance)) return;
//     DestroyWindow(instance->_hwnd);
//     instance->_hwnd = 0;
// }
// void BitmapWindow_Update(BitmapWindow* instance)
// {
//     if (!BitmapWindow_Exists(instance)) return;

//     MSG message = {};

//     while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
//         DispatchMessageA(&message);

//     InvalidateRect(instance->_hwnd, NULL, false);
//     UpdateWindow(instance->_hwnd);
// }
// void BitmapWindow_SetPixel(BitmapWindow* instance, int x, int y, uint32_t pixel)
// {
//     if (!BitmapWindow_Exists(instance)) return;

//     // window bitmap is bottom-up
//     y = instance->_height - 1 - y;
//     instance->_pixels[x + y * instance->_width] = pixel;
// }
// void BitmapWindow_SetPixels(BitmapWindow* instance, uint32_t* pixels, int width, int height)
// {
//     if (!BitmapWindow_Exists(instance)) return;

//     // copy from Top-Down bitmap to Bottom-Up bitmap

//     for (int y = 0; y < height; y++)
//     for (int x = 0; x < width; x++)
//     {
//         uint32_t pixel = pixels[x + y * width];
//         int y2 = height - 1 - y;
//         instance->_pixels[x + y2 * width] = pixel;
//     }
// }
// void TestDraw(BitmapWindow* window, int width, int height)
// {
//     static int iteration = 0;
//     iteration++;

//     for (int y = 0; y < height; y++)
//     for (int x = 0; x < width; x++)
//     {
//         uint32_t a = 0;
//         uint32_t r = (uint8_t)(iteration / 9);
//         uint32_t g = (uint8_t)(iteration / 6);
//         uint32_t b = (uint8_t)(iteration / 3);

//         uint32_t pixel =
//             (a << (8 * 3)) +
//             (r << (8 * 2)) +
//             (g << (8 * 1)) +
//             (b << (8 * 0));

//         BitmapWindow_SetPixel(window, x, y, pixel);
//     }
// }

#include "LightWindow.h"

void TestDraw(BitmapWindow* window, int width, int height)
{
    static int iteration = 0;
    iteration++;

    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        uint32_t a = 0;
        uint32_t r = (uint8_t)(iteration / 9);
        uint32_t g = (uint8_t)(iteration / 6);
        uint32_t b = (uint8_t)(iteration / 3);

        uint32_t pixel =
            (a << (8 * 3)) +
            (r << (8 * 2)) +
            (g << (8 * 1)) +
            (b << (8 * 0));

        BitmapWindow_SetPixel(window, x, y, pixel);
    }
}

int main()
{
    int size = 512;

    BitmapWindow* window = BitmapWindow_Create(0, 0, size, size);

    while (BitmapWindow_Exists(window))
    {
        BitmapWindow_Update(window);
        TestDraw(window, size, size);
    }

    BitmapWindow_Destroy(window);

    return 0;
}
