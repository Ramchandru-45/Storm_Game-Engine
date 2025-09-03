#include "se_pch.h"
#include "Application.h"
#include <GLFW/glfw3.h>

#include "Storm/ImGuiLayer/ImGuiLayer.h"

Storm::Application* Storm::Application::sInstance;

Storm::Application::Application()
{
	SE_INFO("Application Started");
	sInstance = this;
	MainWindow = std::unique_ptr<Window>(Window::Create(WindowProps(1280, 720, "Storm - Editor")));
	if (GetWindow().GetNativeWindow() != nullptr)
		m_running = true;

	MainWindow->SetEventCallback(SE_BIND_EVENT_FN(Application::OnEvent));

	if (!enableImGui)
	{
		PushOverlay(new ImGuiLayer());
	}
}

Storm::Application::~Application()
{
	ShutDown();
}

void Storm::Application::OnEvent(Event& e)
{

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(SE_BIND_EVENT_FN(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(SE_BIND_EVENT_FN(Application::OnWindowResize));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if (e.Handled)
			break;
		(*it)->OnEvent(e);
	}
}

void Storm::Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Storm::Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

void Storm::Application::RunApp()
{
	Timer timer;
	uint32_t fps = 0;
	while (m_running)
	{
		float time = glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		for (auto &layer : m_LayerStack)
		{
			layer->OnUpdate(timestep);
		}

		if (!is_minimized)
		{
			for (auto& layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			MainWindow->OnUpdate();

		}
		fps++;
		if (timer.ElapsedMillis() >= 1000.0f)
		{
			SE_TRACE("FPS {0}", fps);
			fps = 0;
			timer.Reset();
		}
	}
}

bool Storm::Application::OnWindowClose(WindowCloseEvent& e)
{
	m_running = false;
	return false;
}

bool Storm::Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 && e.GetHeight() == 0)
		is_minimized = true;

	is_minimized = false;

	return false;
}

void Storm::Application::ShutDown()
{
	

}

void Storm::Application::ProcessEvent()
{
	
}
