-- premake5.lua

workspace "Storm Engine"
	configurations{
		"Debug",
		"Release"
	}
	architecture "x64"
	startproject "Storm"

	flags {"MultiProcessorCompile"}

	outputdir = "%{cfg.buildcfg}/%{cfg.architecure}"

	filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	project "Storm"
		location "%{wks.location}/Storm"
		kind "ConsoleApp"
		language "C++"
		targetdir ("%{prj.location}/bin/"..outputdir.."")
		objdir ("%{prj.location}/bin-int/"..outputdir.."")
		targetname "Storm Engine"

		includedirs{
			"%{wks.location}/Engine"
		}

		files{
			"%{prj.location}/**.h",
			"%{prj.location}/**cpp",
		}
	

		links {"Engine"}
		libdirs {"../Engine/bin/"}

	project "Engine"
		location "%{wks.location}/Engine"
		kind "SharedLib"
		language "C++"
		targetdir ("%{prj.location}/bin/"..outputdir.."")
		objdir ("%{prj.location}/bin-int/"..outputdir.."")
		targetname "Engine"

		defines {"ENGINE_EXPORT"}

		includedirs{
			"%{wks.location}/Engine"
		}

		pchheader "pch.h"
		pchsource "%{prj.location}/pch.cpp"

		files{
			"%{prj.location}/**.h",
			"%{prj.location}/**.cpp"
		}

		postbuildcommands {
            "copy /Y \"%{cfg.targetdir}\\%{cfg.targetname}.dll\" \"%{wks.location}/Storm/bin/"..outputdir.."\""
        }
