project "glew"
	kind "StaticLib"
	language "C"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

	files {
		"glew/src/**.c"
	}

	includedirs {
		"glew/include"
	}

	defines "GLEW_STATIC"
	defines "GLEW_NO_GLU"