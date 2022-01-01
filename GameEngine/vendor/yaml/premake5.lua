project "yaml"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
        "yaml-cpp/src/*.cpp"
	}

	includedirs {
        "yaml-cpp/include"
	}
