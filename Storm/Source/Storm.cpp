#include "pch.h"
#include "Window/Win32/WinApplication.h"

int main(int argc, char* argv[])
{
	
	Storm::WinApplication window(1280, 720);
	try
	{
		window.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
		
	
	return 0;
}

