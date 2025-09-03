#pragma once

#include "Storm/Core/core.h"
#include <GLFW/glfw3.h>

namespace Storm
{
	struct APIProp
	{
		uint32_t width = 0, height = 0;
		HWND hwnd = nullptr;
		GLFWwindow* gwnd = nullptr;

		APIProp() {}
		APIProp(int w, int h, HWND wnd, GLFWwindow* gwnd) : width(w), height(h), hwnd(wnd), gwnd(gwnd) {}
	};

	class STORM_API GraphicsContext
	{
	public:
		virtual ~GraphicsContext() {}

		virtual void SetWindowSize(int w, int h) = 0;

		virtual void OnUpdate() = 0;


		virtual bool multiGPUEnabled() const = 0;
		virtual void setmultiGPUSupport(bool mGPU) const = 0;

		virtual void ExecuteCommandList() = 0;

		//Instantiate the Graphics API
		static GraphicsContext* CreateGraphicsContext(const APIProp& props = APIProp());
	};
}

