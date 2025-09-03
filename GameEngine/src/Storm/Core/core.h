#pragma once

#ifdef _WIN32
#define WIN_PLATFORM
#define DIRECTX12
#elif __linux__
#define LINUX_PLATFORM
#else
#error Storm only suports linux and windows
#endif

#define BIT(x) (1 << x)
#define SE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef WIN_PLATFORM
	#ifdef DLL_EXPORT
		#define STORM_API __declspec(dllexport)
	#else
		#define STORM_API __declspec(dllimport)
	#endif
#endif

#include "Storm/Core/Log.h"
#include "Storm/Core/storm_assert.h"