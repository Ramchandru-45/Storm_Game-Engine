#pragma once

//Get Platform
#define WIN_PLATFORM

//Graphics API
#define DIRECTX12

//macro
#ifdef ENGINE_EXPORT

#define SE_API __declspec(dllexport)

#else

#define SE_API __declspec(dllimport)

#endif

//Assert Functions or value and throw error (if needed)
#define SE_ASSERT(value, message)\
	if(value)\
	{\
		LOG_INFO(message)\
	}


//Evaluate HResult and throw error if failed
#define HR_EVAL(value , message)\
	if(FAILED(value))\
	{\
		LOG_ERROR(message << " at File: " << __FILE__)\
		throw std::runtime_error(message);\
	}
		


//Log Information to be passed
#define LOG_INFO(message) std::cout << "[INFO]: " << message << std::endl;

//Log Warning to be passed
#define LOG_WARNING(message) std::cout << "[WARNING]: " << message << std::endl;

//Log Error to be passed
#define LOG_ERROR(message) std::cout << "[ERROR]: " << message << std::endl;
