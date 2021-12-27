project "imgui"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"imgui/*.cpp",
        "imgui/backends/imgui_impl_glfw.cpp",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/misc/cpp/imgui_stdlib.cpp"
	}

    includedirs {
        "imgui",
        "../glfw/glfw/include"
    }
