#include "se_pch.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Storm
{
	std::shared_ptr<spdlog::logger> Log::StormCoreLogger;
	std::shared_ptr<spdlog::logger> Log::StormClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		StormCoreLogger = spdlog::stdout_color_mt("STORM");
		StormCoreLogger->set_level(spdlog::level::trace);

		StormClientLogger = spdlog::stdout_color_mt("APP");
		StormClientLogger->set_level(spdlog::level::trace);
	}
	void Log::Terminate()
	{
		spdlog::drop_all();
	}
}