#include "Storm.h"
#include "Storm/Core/EntryPoint.h"
#include "EditorLayer.h"

extern bool enableImGui;
namespace Storm
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication()
		{
			PushLayer(new EditorLayer());
			enableImGui = true;
		}

	};



	Application* CreateApplication(int argc, char** argv)
	{

		SE_INFO("Hello from App");
		SE_CORE_WARN("Initialized Editor");
		return new EditorApplication();
		
	}
}