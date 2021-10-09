project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++20"
	systemversion "latest"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

	files {
		"src/**.cpp"
	}

	includedirs {
		"../GameEngine/src/"
	}

	links {
		"GameEngine"
	}

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"