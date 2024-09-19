#include "deps.h"

typedef struct BitmapWindow
{
    _Bool keydown_W;
    _Bool keydown_A;
    _Bool keydown_S;
    _Bool keydown_D;
    _Bool keydown_E;
    _Bool keydown_Q;
    _Bool keydown_UP;
    _Bool keydown_LEFT;
    _Bool keydown_DOWN;
    _Bool keydown_RIGHT;
    HWND _hwnd;
    HDC _hdc;
    HBITMAP _hbitmap;
    uint32_t* _pixels;
    int _width;
    int _height;
}
BitmapWindow;
_Bool _BitmapWindow_Registered = 0;
void _BitmapWindow_InitBitmap(BitmapWindow* instance)
{
    instance->_hdc = CreateCompatibleDC(0);
}
void _BitmapWindow_ResetBitmap(BitmapWindow* instance, int clientWidth, int clientHeight)
{
    BITMAPINFO bitmapinfo = {};
    bitmapinfo.bmiHeader.biSize = sizeof(bitmapinfo.bmiHeader);
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biBitCount = 32;
    bitmapinfo.bmiHeader.biCompression = 0l;
    bitmapinfo.bmiHeader.biWidth = clientWidth;
    bitmapinfo.bmiHeader.biHeight = clientHeight;
    if (instance->_hbitmap)
        DeleteObject(instance->_hbitmap);
    instance->_hbitmap = CreateDIBSection(((void *)0), &bitmapinfo, 0, (void**)&instance->_pixels, 0, 0);
    (void) ((!!(instance->_hbitmap != 0)) || (_assert("instance->_hbitmap != 0","main.c",60),0));
    SelectObject(instance->_hdc, instance->_hbitmap);
    instance->_width = clientWidth;
    instance->_height = clientHeight;
}
void _BitmapWindow_PaintBitmap(BitmapWindow* instance)
{
    PAINTSTRUCT paint;
    HDC device_context = BeginPaint(instance->_hwnd, &paint);
    BitBlt(device_context,
            paint.rcPaint.left, paint.rcPaint.top,
            paint.rcPaint.right - paint.rcPaint.left,
            paint.rcPaint.bottom - paint.rcPaint.top,
            instance->_hdc,
            paint.rcPaint.left, paint.rcPaint.top,
            (DWORD)0x00CC0020);
    EndPaint(instance->_hwnd, &paint);
}
LRESULT __attribute__((__stdcall__)) _BitmapWindow_MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BitmapWindow* instance = (BitmapWindow*)(GetWindowLongPtrW(hwnd, (-21)));
    if (instance == ((void *)0))
        return DefWindowProcW(hwnd, message, wParam, lParam);
    switch(message)
    {
        case 0x0012:
        case 0x0002:
        {
            instance->_hwnd = 0;
            break;
        }
        case 0x000F:
        {
            _BitmapWindow_PaintBitmap(instance);
            break;
        }
        case 0x0005:
        {
            int clientWidth = ((WORD) (((DWORD_PTR) (lParam)) & 0xffff));
            int clientHeight = ((WORD) ((((DWORD_PTR) (lParam)) >> 16) & 0xffff));
            _Bool sizeChanged =
                instance->_width != clientWidth ||
                instance->_height != clientHeight;
            if (sizeChanged)
                _BitmapWindow_ResetBitmap(instance, clientWidth, clientHeight);
            break;
        }
        case 0x0100:
        case 0x0101:
        {
            _Bool isDown = (message == 0x0100);
            switch (wParam)
            {
                case 0x1B : { DestroyWindow(hwnd); break; }
                case 'W' : { instance->keydown_W = isDown; break; }
                case 'A' : { instance->keydown_A = isDown; break; }
                case 'S' : { instance->keydown_S = isDown; break; }
                case 'D' : { instance->keydown_D = isDown; break; }
                case 'E' : { instance->keydown_E = isDown; break; }
                case 'Q' : { instance->keydown_Q = isDown; break; }
                case 0x26 : { instance->keydown_UP = isDown; break; }
                case 0x25 : { instance->keydown_LEFT = isDown; break; }
                case 0x28 : { instance->keydown_DOWN = isDown; break; }
                case 0x27 : { instance->keydown_RIGHT = isDown; break; }
                default : { break; }
            }
            break;
        }
        case 0x0024:
        {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = 10;
            lpMMI->ptMinTrackSize.y = 10;
            break;
        }
        default: return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}
BitmapWindow* BitmapWindow_Create(int x, int y, int clientWidth, int clientHeight)
{
    BitmapWindow* instance = (BitmapWindow*)malloc(sizeof(BitmapWindow));
    instance->keydown_W = 0;
    instance->keydown_A = 0;
    instance->keydown_S = 0;
    instance->keydown_D = 0;
    instance->keydown_E = 0;
    instance->keydown_Q = 0;
    instance->keydown_UP = 0;
    instance->keydown_LEFT = 0;
    instance->keydown_DOWN = 0;
    instance->keydown_RIGHT = 0;
    instance->_hwnd = 0;
    instance->_hdc = 0;
    instance->_hbitmap = 0;
    instance->_pixels = 0;
    instance->_width = 0;
    instance->_height = 0;
    HINSTANCE hInstance = GetModuleHandleW(0);
    const LPCWSTR className = L"BitmapWindowClass";
    if (!_BitmapWindow_Registered)
    {
        _BitmapWindow_Registered = 1;
        WNDCLASS window_class = {};
        window_class.lpfnWndProc = _BitmapWindow_MessageHandler;
        window_class.hInstance = hInstance;
        window_class.lpszClassName = className;
        window_class.hCursor = LoadCursorW(0, ((LPWSTR)((ULONG_PTR)((WORD)(32512)))));
        RegisterClassW(&window_class);
    }
    LONG lStyle = 0x10000000l;
    RECT rect = { 0, 0, (LONG)clientWidth, (LONG)clientHeight };
    AdjustWindowRect(&rect, lStyle, 0);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;
    _BitmapWindow_InitBitmap(instance);
    _BitmapWindow_ResetBitmap(instance, clientWidth, clientHeight);
    instance->_hwnd = CreateWindowExW((DWORD)0,className,L"BitmapWindow",lStyle,(LONG)x,(LONG)y,windowWidth,windowHeight,((void *)0),((void *)0),hInstance,((void *)0));
    (void) ((!!(instance->_hwnd != ((void *)0))) || (_assert("instance->_hwnd != NULL","main.c",211),0));
    SetWindowLongPtrW(instance->_hwnd, (-21), (LONG_PTR)(instance));
    SetWindowLongW(instance->_hwnd, (-16), lStyle);
    SetWindowPos(instance->_hwnd, ((void *)0), 0,0,0,0, 0x0020 | 0x0002 | 0x0001 | 0x0004 | 0x0200);
    return instance;
}
_Bool BitmapWindow_Exists(BitmapWindow* instance)
{
    return instance->_hwnd != 0;
}
void BitmapWindow_Destroy(BitmapWindow* instance)
{
    if (!BitmapWindow_Exists(instance)) return;
    DestroyWindow(instance->_hwnd);
    instance->_hwnd = 0;
}
void BitmapWindow_Update(BitmapWindow* instance)
{
    if (!BitmapWindow_Exists(instance)) return;
    MSG message = {};
    while (PeekMessageW(&message, ((void *)0), 0, 0, 0x0001))
        DispatchMessageW(&message);
    InvalidateRect(instance->_hwnd, ((void *)0), 0);
    UpdateWindow(instance->_hwnd);
}
void BitmapWindow_SetPixel(BitmapWindow* instance, int x, int y, uint32_t pixel)
{
    if (!BitmapWindow_Exists(instance)) return;
    y = instance->_height - 1 - y;
    instance->_pixels[x + y * instance->_width] = pixel;
}
void BitmapWindow_SetPixels(BitmapWindow* instance, uint32_t* pixels, int width, int height)
{
    if (!BitmapWindow_Exists(instance)) return;
    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        uint32_t pixel = pixels[x + y * width];
        int y2 = height - 1 - y;
        instance->_pixels[x + y2 * width] = pixel;
    }
}
void TestDraw(BitmapWindow* window, int width, int height)
{
    for (size_t y = 0; y < height; y++)
    for (size_t x = 0; x < width; x++)
    {
        uint32_t a = 0;
        uint32_t r = (uint8_t)(clock() / 9);
        uint32_t g = (uint8_t)(clock() / 6);
        uint32_t b = (uint8_t)(clock() / 3);
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
