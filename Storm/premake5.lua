-- storm.lua

project "Storm"
	location "%{wks.location}/Storm"
	kind "ConsoleApp"
	targetdir ("%{outputdir}/Storm/")
	objdir ("%{prj.location}/obj/")
	targetname "Storm Engine"

	includedirs{
		"%{IncludeDir.storm}",
		"%{IncludeDir.engine}",
		"%{IncludeDir.engine}/src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
	}
		
		
	files{
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
	}


	links {
		"GameEngine"
	}

	filter "configurations:DIST"
		kind "WindowedApp"