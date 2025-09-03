#pragma once

#include "Storm/Core/core.h"

#include "Storm/Engine/Window/Window.h"
#include "Storm/Core/Layer.h"
#include "Storm/Core/LayerStack.h"
#include "Storm/Engine/Events/ApplicationEvent.h"

#include "Storm/Core/TimeStep.h"
#include "Storm/Core/Timer.h"

static bool enableImGui;
namespace Storm
{
	class STORM_API Application
	{
	public: 
		Application();
		virtual ~Application();

		void OnEvent(Event& e);
		
		void RunApp();
		virtual void ShutDown();

		void ProcessEvent();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static Storm::Application& Get() { return *sInstance; }
		Window& GetWindow() const { return *MainWindow; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private: //System
		std::unique_ptr<Window> MainWindow = nullptr;
		bool m_running = false;
		bool is_minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* sInstance;
	};

	//Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}