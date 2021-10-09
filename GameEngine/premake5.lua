project "GameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "c++20"
	systemversion "latest"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

	files {
		"src/**.cpp"
	}

	includedirs {
	}

	links {
	}

	filter "configurations:Debug"
		defines "GAME_ENGINE_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "GAME_ENGINE_RELEASE"
		optimize "On"
		runtime "Release"