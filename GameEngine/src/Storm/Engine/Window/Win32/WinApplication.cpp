
#include "se_pch.h"
#ifdef WIN32_API

#include "WinApplication.h"

namespace Storm
{

	Window* Window::Create(const WindowProps& props)
	{
		return new WinApplication(props);
	}

	WinApplication::WinApplication(const WindowProps& props)
	{
		std::cout << "Win32 init\n";
	}

	WinApplication::~WinApplication()
	{
		std::cout << "Win32 terminate\n";
	}

	void WinApplication::Run()
	{
		Initialize();

		while (m_running)
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0L, 0L, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					m_running = false;
			}
			
			Sleep(100);

		}

		ShutDown();
	}


	bool WinApplication::Initialize()
	{
		
		ZeroMemory(&wc, sizeof(wc));
		wc = { sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Storm", nullptr };

		::RegisterClassExW(&wc);
		hwnd = CreateWindow(wc.lpszClassName, L"Storm Engine", WS_EX_OVERLAPPEDWINDOW, 100, 100, 1280, 720, NULL, NULL, wc.hInstance, 0);

		if (hwnd)
		{
			ShowWindow(hwnd, SW_NORMAL);
			UpdateWindow(hwnd);
			m_running = true;
		}
		return true;
	}

	bool WinApplication::ShutDown()
	{
		if (hwnd)
			hwnd = nullptr;
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return true;
	}

	void WinApplication::SetVSync(bool vsync)
	{
	}

	bool WinApplication::IsVSync()
	{
		return false;
	}



	LRESULT WINAPI Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		switch (msg)
		{
		case WM_DESTROY:
			//g_running = false;
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
}

#endif