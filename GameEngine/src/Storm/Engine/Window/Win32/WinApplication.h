#pragma once

#ifdef WIN32_API
#include "Engine/Window/Window.h"

namespace Storm
{
	
	class STORM_API WinApplication : public Window
	{
	public:
		WinApplication(const WindowProps& props);
		~WinApplication();

		bool Initialize() override;
		void Run() override;
		bool ShutDown() override;

		virtual void SetVSync(bool vsync) override;
		virtual bool IsVSync() override;

	private:
		bool m_running{ false };
		WNDCLASSEXW wc = {};
	};
}

#endif