--game_engine.lua

group "Dependencies"
	
	include "vendor/glfw"
	include "vendor/imgui"
	include "vendor/spdlog"
	
group ""

group "Core"
project "GameEngine"
	location "%{wks.location}/GameEngine"
	kind "SharedLib"
	targetdir ("%{outputdir}/GameEngine/")
	objdir ("%{prj.location}/obj/")
	targetname ("storm%{version}")

	defines {"DLL_EXPORT"}
		
	includedirs{
		"%{IncludeDir.engine}",
		"%{IncludeDir.engine}/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
	}

	
	files{
		"Storm.h",
		"se_pch.h",
		"se_pch.cpp",

		"src/**.c",
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	pchheader "se_pch.h"
	pchsource "%{wks.location}/GameEngine/se_pch.cpp"

	links {
		"GLFW",
		"IMGUI",
		"SPDLOG"
	}

	postbuildcommands {
		"{COPYFILE} \"%{cfg.targetdir}/%{cfg.targetname}.dll\" \"%{cfg.targetdir}/../Storm\""
	}
group ""