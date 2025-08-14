#include "pch.h"
#include "WinApplication.h"

Storm::WinApplication* pointer = nullptr;

Storm::WinApplication::WinApplication(int width, int height)
{
    m_width = width;
    m_height = height;
    if (!pointer)
        pointer = this;
}

Storm::WinApplication::~WinApplication()
{
}

void Storm::WinApplication::OnCreate()
{
    if (!m_hwnd)
    {
        LOG_ERROR("WindowHandle is Null")
        return;
    }
    SE_ASSERT(select_api(), "No Graphics API Selected");

    if (!graphics.get())
        LOG_ERROR("Graphics is NULL");

    //graphics->ListAdapters();
    graphics->Create(m_hwnd);
    

        
}

void Storm::WinApplication::OnUpdate()
{
    if (graphics)
    {
        //LOG_INFO("Update")
        graphics->Update();
    }
}

void Storm::WinApplication::OnRender()
{
    if (graphics)
    {
        //LOG_INFO("Render")
        graphics->Render();
    }
}

void Storm::WinApplication::OnDestroy()
{
    graphics->Destroy();
    UnregisterClass(L"Storm", NULL);
}


bool Storm::WinApplication::Initialize()
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Storm::WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Storm";
    
    
    ::RegisterClass(&wc);

    m_hwnd = CreateWindow(wc.lpszClassName, L"Storm - Game Engine", WS_OVERLAPPEDWINDOW, 100, 100, m_width, m_height, NULL, NULL, wc.hInstance, 0);
   
    if (!m_hwnd)
    {
        LOG_ERROR("m_hwnd is null")
            return false;
    }

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);
    m_isRunning = true;
    LOG_INFO("Window Initialized")

    return true;
}

bool Storm::WinApplication::select_api()
{

#if DIRECTX12 1 && WIN_PLATFORM 1
    graphics = std::make_unique<Storm::Directx12>();
    LOG_INFO("DIRECTX 12 API IS SELECTED");
#endif

    return true;
}

bool Storm::WinApplication::Release()
{
    m_isRunning = false;
    LOG_INFO("Window is Shutting-Down");
    return true;
}

LRESULT Storm::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_SIZE:
        if (IsIconic(hwnd))
            minimized = true;
        break;
    case WM_DESTROY:
        pointer->Release();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
