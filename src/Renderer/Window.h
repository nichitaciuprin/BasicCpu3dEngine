#pragma once

class Window
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

    Window(HINSTANCE hInstance)
    {
        windowClosed = false;

        MaybeRegisterClass(hInstance);

        RECT rc = { 0, 0, width, height };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        auto width2 = rc.right - rc.left;
        auto height2 = rc.bottom - rc.top;
        m_hwnd = CreateWindowExW(0, className, className, WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT, width2, height2,
                                 nullptr, nullptr, hInstance, nullptr);
        if (!m_hwnd) throw;

        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        GetClientRect(m_hwnd, &rc);

        SetInstance(m_hwnd,this);

        CreateSwapChain();
        CreateRenderTargets();
        OnWindowResize(width,height);
    }

    void Clear()
    {
        auto deviceContext = DeviceRecources::GetInstance()->GetDeviceContext();
        FLOAT backgroundColor[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
        deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        deviceContext->ClearRenderTargetView(renderTargetView, backgroundColor);
    }
    Matrix GetPerspective()
    {
        return MatrixPerspective((float)width,(float)height,0.1f,1000.f);
    }
    void Present()
    {
        swapChain->Present(1,0);
    }
    void Update()
    {
        MSG msg = {};
        while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                windowClosed = true;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    bool Closed()
    {
        return windowClosed;
    }

private:
    static bool classRegistered;
    static const LPCWSTR className;
    static const LPCWSTR iconName;

    int width = 600;
    int height = 600;
    HWND m_hwnd;
    bool windowClosed;
    IDXGISwapChain1* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;

    static void MaybeRegisterClass(HINSTANCE hInstance)
    {
        if (classRegistered) return;
        WNDCLASSEXW windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WndProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        windowClass.lpszClassName = className;
        windowClass.hIcon = LoadIconW(hInstance, iconName);
        windowClass.hIconSm = LoadIconW(hInstance, iconName);
        if (!RegisterClassExW(&windowClass)) throw;
        classRegistered = true;
    }
    static void SetInstance(HWND hwnd, Window* window)
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    static Window* GetInstance(HWND hwnd)
    {
        return reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto window = GetInstance(hwnd);

        if (window == NULL)
            return DefWindowProc(hwnd, message, wParam, lParam);

        switch (message)
        {
            case WM_DESTROY:
            {
                window->windowClosed = true;
                break;
            }
            case WM_SIZE:
            {
                // Window size was changed
                window->GetWindowInfo(&window->width,&window->height);
                window->OnWindowResize(window->width,window->height);
                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                bool isDown = (message == WM_KEYDOWN);
                switch (wParam)
                {
                    case VK_ESCAPE : { DestroyWindow(hwnd);               break; }
                    case 'W'       : { window->keydown_W        = isDown; break; }
                    case 'A'       : { window->keydown_A        = isDown; break; }
                    case 'S'       : { window->keydown_S        = isDown; break; }
                    case 'D'       : { window->keydown_D        = isDown; break; }
                    case 'E'       : { window->keydown_E        = isDown; break; }
                    case 'Q'       : { window->keydown_Q        = isDown; break; }
                    case VK_UP     : { window->keydown_VK_UP    = isDown; break; }
                    case VK_LEFT   : { window->keydown_VK_LEFT  = isDown; break; }
                    case VK_DOWN   : { window->keydown_VK_DOWN  = isDown; break; }
                    case VK_RIGHT  : { window->keydown_VK_RIGHT = isDown; break; }
                    default        : {                                    break; }
                }
                break;
            }
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    void CreateSwapChain()
    {
        auto device = DeviceRecources::GetInstance()->GetDevice();

        HRESULT hResult;
        IDXGIFactory2* dxgiFactory;
        IDXGIDevice1* dxgiDevice;

        hResult = device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
        assert(SUCCEEDED(hResult));

        IDXGIAdapter* dxgiAdapter;
        hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
        assert(SUCCEEDED(hResult));
        dxgiDevice->Release();

        DXGI_ADAPTER_DESC adapterDesc;
        dxgiAdapter->GetDesc(&adapterDesc);

        OutputDebugStringA("Graphics Device: ");
        OutputDebugStringW(adapterDesc.Description);

        hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
        assert(SUCCEEDED(hResult));
        dxgiAdapter->Release();

        DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
        d3d11SwapChainDesc.Width = 0; // use window width
        d3d11SwapChainDesc.Height = 0; // use window height
        d3d11SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        d3d11SwapChainDesc.SampleDesc.Count = 1;
        d3d11SwapChainDesc.SampleDesc.Quality = 0;
        d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11SwapChainDesc.BufferCount = 2;
        d3d11SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        d3d11SwapChainDesc.Flags = 0;

        hResult = dxgiFactory->CreateSwapChainForHwnd(device, m_hwnd, &d3d11SwapChainDesc, 0, 0, &swapChain);
        assert(SUCCEEDED(hResult));

        dxgiFactory->Release();
    }
    void CreateRenderTargets()
    {
        HRESULT hResult;

        auto device = DeviceRecources::GetInstance()->GetDevice();

        ID3D11Texture2D* d3d11FrameBuffer;
        hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
        assert(SUCCEEDED(hResult));

        hResult = device->CreateRenderTargetView(d3d11FrameBuffer, 0, &renderTargetView);
        assert(SUCCEEDED(hResult));

        D3D11_TEXTURE2D_DESC depthBufferDesc;
        d3d11FrameBuffer->GetDesc(&depthBufferDesc);

        d3d11FrameBuffer->Release();

        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* depthBuffer = {};
        device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

        assert(depthBuffer != nullptr);

        device->CreateDepthStencilView(depthBuffer, nullptr, &depthStencilView);

        depthBuffer->Release();
    }
    void GetWindowInfo(int* outWindowWidth, int* outWindowHeight)
    {
        RECT clientRect;
        GetClientRect(m_hwnd, &clientRect);
        *outWindowWidth = clientRect.right - clientRect.left;
        *outWindowHeight = clientRect.bottom - clientRect.top;
    }
    void OnWindowResize(int windowWidth, int windowHeight)
    {
        auto deviceContext = DeviceRecources::GetInstance()->GetDeviceContext();

        float windowWidthF = (float)windowWidth;
        float windowHeightF = (float)windowHeight;
        // float windowAspectRatio = windowWidthF / windowHeightF;

        D3D11_VIEWPORT viewport = { 0.0f, 0.0f, windowWidthF, windowHeightF, 0.0f, 1.0f };
        deviceContext->RSSetViewports(1, &viewport);

        deviceContext->OMSetRenderTargets(0, 0, 0);

        renderTargetView->Release();
        depthStencilView->Release();

        HRESULT hResult = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        assert(SUCCEEDED(hResult));

        CreateRenderTargets();
    }
};
bool Window::classRegistered = false;
const LPCWSTR Window::className = L"BoidsDirectX";
const LPCWSTR Window::iconName = L"IDI_ICON";