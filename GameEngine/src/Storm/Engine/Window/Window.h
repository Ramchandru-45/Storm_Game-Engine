#pragma once

#include "Storm/Core/core.h"
#include "Storm/Engine/Events/Event.h"

struct GLFWwindow;

namespace Storm
{
	struct WindowProps
	{
		uint32_t Width, Height;
		std::string Title;

		WindowProps(uint32_t width = 1280, uint32_t height = 720, std::string title = "Storm")
			:Width(width), Height(height), Title(title) {}
	};

	class STORM_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void GetGraphicsContext() const = 0;
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}


