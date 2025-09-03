#pragma once

#include "Storm/Core/core.h"
#include "Storm/Engine/Window/Window.h"
#include "Storm/Engine/Graphics/GraphicsContext.h"


#ifdef DIRECTX12
	#define GLFW_INCLUDE_NONE
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
#endif

namespace Storm
{
	class STORM_API GLFWApplication : public Window
	{
	public:
		GLFWApplication(const WindowProps& props);
		virtual ~GLFWApplication();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void GetGraphicsContext() const { return (void) &m_Context; }
		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		std::shared_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
	
}