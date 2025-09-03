#pragma once 

#include "Storm/Core/Application.h"


extern Storm::Application* Storm::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Storm
{
	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			Log::Init();
			SE_WARN("Engine starty");
			SE_INFO("App starty");

			Storm::Application* app = Storm::CreateApplication(argc, argv);
			app->RunApp();
			delete app;
			Log::Terminate();
			g_ApplicationRunning = false;
		}
		return 0;
	}
}


#ifdef DIST

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return Storm::Main(__argc, __argv);
}

#else

int main(int argc, char* argv[])
{
	return Storm::Main(argc, argv);
}

#endif // DIST
