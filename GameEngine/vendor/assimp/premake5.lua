project "assimp"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
        "assimp/code/AssetLib/Blender/*.cpp",
        "assimp/code/AssetLib/FBX/*.cpp",
        "assimp/code/AssetLib/glTF/*.cpp",
        "assimp/code/AssetLib/glTF2/*.cpp",
        "assimp/code/AssetLib/Obj/*.cpp",

        "assimp/code/Common/*.cpp",
        "assimp/code/Material/*.cpp",
        "assimp/code/Pbrt/*.cpp",
        "assimp/code/PostProcessing/*.cpp"
	}

	includedirs {
		"assimp/include",
        "assimp/code",
        "assimp",
        "assimp/contrib",
        "assimp/contrib/unzip",
        "assimp/contrib/rapidjson/include"
	}

    prebuildcommands {
        "cp 'generated headers/config.h' assimp/include/assimp/config.h",
        "cp 'generated headers/revision.h' assimp/revision.h"
    }

    cleancommands {
        "rm assimp/include/assimp/config.h",
        "rm assimp/revision.h"
    }

    defines {
        "ASSIMP_BUILD_NO_EXPORT"
    }