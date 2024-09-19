#define NULL 0
#define true 1
#define false 0

void __cdecl _assert(const char* message, const char* file, unsigned line);
#define assert(x) (void)((!!(x)) || (_assert(#x, __FILE__, __LINE__), 0))

typedef unsigned char uint8_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long size_t;
typedef int bool;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef size_t WPARAM;
typedef int64_t LPARAM;
typedef int64_t LRESULT;
typedef void* HANDLE;
typedef void* HINSTANCE;
typedef void* HMODULE;
typedef void* HWND;
typedef void* HGDIOBJ;
typedef void* HBITMAP;
typedef void* HBRUSH;
typedef void* HDC;
typedef void* HICON;
typedef void* HMENU;
typedef void* HCURSOR;

void* __cdecl calloc(size_t numOfElements, size_t sizeOfElements);
void  __cdecl free(void* memory);
void* __cdecl malloc(size_t size);
void* __cdecl realloc(void* memory, size_t newSize);

#define LOWORD(l) ((WORD)(((size_t)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((size_t)(l)) >> 16) & 0xffff))

typedef LRESULT(__stdcall *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagRECT
{
    long left;
    long top;
    long right;
    long bottom;
}
RECT, *PRECT, *NPRECT, *LPRECT;
typedef struct tagPOINT
{
    long x;
    long y;
}
POINT, *PPOINT, *NPPOINT, *LPPOINT;
typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
}
RGBQUAD;
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    long biWidth;
    long biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}
BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
}
BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;
typedef struct tagWNDCLASSA
{
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    const char * lpszMenuName;
    const char * lpszClassName;
}
WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
typedef struct tagMSG
{
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
}
MSG, *PMSG, *NPMSG, *LPMSG;
typedef struct tagMINMAXINFO
{
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
}
MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;
typedef struct tagPAINTSTRUCT
{
    HDC hdc;
    bool fErase;
    RECT rcPaint;
    bool fRestore;
    bool fIncUpdate;
    BYTE rgbReserved[32];
}
PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

#define SRCCOPY (DWORD)0x00CC0020
#define BI_RGB 0l
#define DIB_RGB_COLORS 0
#define VK_ESCAPE 0x1B
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define GWL_STYLE (-16)
#define GWLP_USERDATA (-21)
#define WM_DESTROY 0x0002
#define WM_SIZE 0x0005
#define WM_PAINT 0x000F
#define WM_QUIT 0x0012
#define WM_GETMINMAXINFO 0x0024
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WS_VISIBLE 0x10000000l
#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SWP_NOZORDER 0x0004
#define SWP_NOREDRAW 0x0008
#define SWP_NOACTIVATE 0x0010
#define SWP_FRAMECHANGED 0x0020
#define SWP_SHOWWINDOW 0x0040
#define SWP_HIDEWINDOW 0x0080
#define SWP_NOCOPYBITS 0x0100
#define SWP_NOOWNERZORDER 0x0200
#define PM_REMOVE 0x0001
#define SWP_NOSENDCHANGING 0x0400

__declspec(dllimport) HMODULE  __stdcall  GetModuleHandleA(const char* lpModuleName);
__declspec(dllimport) bool     __stdcall  BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
__declspec(dllimport) bool     __stdcall  DeleteObject(HGDIOBJ ho);
__declspec(dllimport) HDC      __stdcall  CreateCompatibleDC(HDC hdc);
__declspec(dllimport) HGDIOBJ  __stdcall  SelectObject(HDC hdc, HGDIOBJ h);
__declspec(dllimport) HBITMAP  __stdcall  CreateDIBSection(HDC hdc, const BITMAPINFO* lpbmi, UINT usage, void** ppvBits, HANDLE hSection, DWORD offset);
__declspec(dllimport) LRESULT  __stdcall  DispatchMessageA(const MSG *lpMsg);
__declspec(dllimport) bool     __stdcall  PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
__declspec(dllimport) WORD     __stdcall  RegisterClassA(const WNDCLASSA *lpWndClass);
__declspec(dllimport) LRESULT  __stdcall  DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
__declspec(dllimport) HWND     __stdcall  CreateWindowExA(DWORD dwExStyle, const char* lpClassName, const char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam);
__declspec(dllimport) bool     __stdcall  DestroyWindow(HWND hWnd);
__declspec(dllimport) bool     __stdcall  CloseWindow(HWND hWnd);
__declspec(dllimport) bool     __stdcall  SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
__declspec(dllimport) bool     __stdcall  UpdateWindow(HWND hWnd);
__declspec(dllimport) HDC      __stdcall  BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
__declspec(dllimport) bool     __stdcall  EndPaint(HWND hWnd, const PAINTSTRUCT* lpPaint);
__declspec(dllimport) bool     __stdcall  InvalidateRect(HWND hWnd, const RECT *lpRect, bool bErase);
__declspec(dllimport) bool     __stdcall  AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, bool bMenu);
__declspec(dllimport) long     __stdcall  GetWindowLongA(HWND hWnd, int nIndex);
__declspec(dllimport) long     __stdcall  SetWindowLongA(HWND hWnd, int nIndex, long dwNewLong);
__declspec(dllimport) int64_t  __stdcall  GetWindowLongPtrA(HWND hWnd, int nIndex);
__declspec(dllimport) int64_t  __stdcall  SetWindowLongPtrA(HWND hWnd, int nIndex, int64_t dwNewLong);
__declspec(dllimport) HCURSOR  __stdcall  LoadCursorA(HINSTANCE hInstance, const char * lpCursorName);

#define IDC_ARROW (char*)((size_t)((WORD)(32512)))
