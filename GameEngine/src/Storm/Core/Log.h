#pragma once

#include "Storm/Core/core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace Storm
{
	class STORM_API Log
	{
	public:
		static void Init();
		static void Terminate();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return StormCoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return StormClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> StormCoreLogger;
		static std::shared_ptr<spdlog::logger> StormClientLogger;

	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}


//Log Core Macro
#define SE_CORE_TRACE(...)	Storm::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SE_CORE_INFO(...)	Storm::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SE_CORE_WARN(...)	Storm::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SE_CORE_ERROR(...)	Storm::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SE_CORE_FATAL(...)	Storm::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Log Client Macro
#define SE_TRACE(...)		Storm::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SE_INFO(...)		Storm::Log::GetClientLogger()->info(__VA_ARGS__)
#define SE_WARN(...)		Storm::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SE_ERROR(...)		Storm::Log::GetClientLogger()->error(__VA_ARGS__)
#define SE_FATAL(...)		Storm::Log::GetClientLogger()->critical(__VA_ARGS__)
