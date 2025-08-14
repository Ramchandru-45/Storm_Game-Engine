#pragma once
#include <ctime>
#include <chrono>

namespace Storm
{
	static int FrameLimit = 0;
	class SE_API Timer
	{
		constexpr int GetWorldTime();
		const void FPS();
	};
}
