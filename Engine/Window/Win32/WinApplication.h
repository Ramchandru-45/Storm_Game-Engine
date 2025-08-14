#pragma once
#include <Windows.h>
#include "Renderer/Renderer.h"
#include "Renderer/DirectX 12/Directx12.h"

namespace Storm
{
	extern LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool minimized{ false };

	class SE_API WinApplication
	{
	public:
		WinApplication(int width, int height);
		~WinApplication();

		void OnCreate();
		void OnUpdate();
		void OnRender();
		void OnDestroy();

		bool Initialize();
		bool select_api();
		int Run();
		bool Release();

		inline bool isRun() { return m_isRunning; }

	private: // System variables
		HWND m_hwnd = nullptr;
		int m_width = 0;
		int m_height = 0;
		bool m_isRunning = false;

	private: // Subsystem variables
		std::unique_ptr<Renderer> graphics;
	};

}

