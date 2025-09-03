-- premake5.lua

workspace "Storm Engine"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	architecture "x64"
	startproject "Storm"
	language "C"
	cdialect "c11"
	language "C++"
	cppdialect "C++17"

	flags {"MultiProcessorCompile"}

	outputdir = "%{wks.location}/build/%{cfg.buildcfg}/%{cfg.architecure}"
	version = "v1_0"
	copypath = "%{outputdir}"

	filter "configurations:Debug"
      defines { "SE_DEBUG" }
      symbols "On"

	filter "configurations:Release"
		defines { "SE_NDEBUG" }
		optimize "On"

	filter "configurations:Dist"
		defines {"DIST"}
		optimize "On"

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

	IncludeDir = {}
	IncludeDir["storm"]		=	"%{wks.location}/Storm"
	IncludeDir["engine"]	=	"%{wks.location}/GameEngine"
	IncludeDir["imgui"]		=	"%{wks.location}/GameEngine/vendor/imgui"
	IncludeDir["glfw"]		=	"%{wks.location}/GameEngine/vendor/GLFW/include"
	IncludeDir["spdlog"]	=	"%{wks.location}/GameEngine/vendor/spdlog/include"
	IncludeDir["glm"]		=	"%{wks.location}/GameEngine/vendor/glm"

	include "GameEngine"

	include "Storm"