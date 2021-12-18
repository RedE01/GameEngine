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

	-- sysincludedirs is used instead of includedirs here so that the compiler doesn't generate warnings for these files
	sysincludedirs {
		"vendor/glfw/glfw/include",
		"vendor/glew/glew/include",
		"vendor/glm/glm/glm",
		"vendor/entt/src",
		"vendor/assimp/assimp/include",
		"vendor/stb",
        "vendor/imgui/imgui"
	}

	warnings "Extra"
	defines "GLEW_NO_GLU"

	filter "configurations:Debug"
		defines "GAME_ENGINE_DEBUG"
		defines "GAME_ENGINE_EDITOR"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "GAME_ENGINE_RELEASE"
		optimize "On"
		runtime "Release"
