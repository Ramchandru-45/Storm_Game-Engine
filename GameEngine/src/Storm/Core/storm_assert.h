#pragma once

#include "Storm/Core/Log.h"

//HRESULT evaluation and throw error
#ifdef DIST
#define HR_EVAL(value, ...)
#define SE_ASSERT(value , message)
#else
#define HR_EVAL(value, ...)\
		if(FAILED(value))\
		{\
			SE_ERROR(__VA_ARGS__);\
			__debugbreak();\
		}

#define SE_ASSERT(value , ...)\
	if(value)\
		SE_INFO(__VA_ARGS__);\
	else\
		__debugbreak()

#endif //DIST
