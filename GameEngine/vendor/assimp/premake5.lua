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
        "assimp/code/PostProcessing/*.cpp",

        "assimp/contrib/zlib/*.c"
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
        "cp -u 'generated headers/config.h' assimp/include/assimp/config.h",
        "cp -u 'generated headers/revision.h' assimp/revision.h"
    }

    cleancommands {
        "rm assimp/include/assimp/config.h",
        "rm assimp/revision.h"
    }

    defines {
        "ASSIMP_BUILD_NO_EXPORT",
        "ASSIMP_BUILD_NO_3DS_IMPORTER",
        "ASSIMP_BUILD_NO_3D_IMPORTER", -- UNREAL_IMPORTER (for some reason it's called 3D_IMPORTER?)
        "ASSIMP_BUILD_NO_3MF_IMPORTER",
        "ASSIMP_BUILD_NO_AC_IMPORTER",
        "ASSIMP_BUILD_NO_AMF_IMPORTER",
        "ASSIMP_BUILD_NO_ASE_IMPORTER",
        "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
        "ASSIMP_BUILD_NO_ASSJSON_IMPORTER",
        "ASSIMP_BUILD_NO_ASSXML_IMPORTER",
        "ASSIMP_BUILD_NO_B3D_IMPORTER",

        "ASSIMP_BUILD_NO_BVH_IMPORTER",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "ASSIMP_BUILD_NO_COB_IMPORTER",
        "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
        "ASSIMP_BUILD_NO_CSM_IMPORTER",
        "ASSIMP_BUILD_NO_DXF_IMPORTER",



        "ASSIMP_BUILD_NO_HMP_IMPORTER",
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "ASSIMP_BUILD_NO_IRR_IMPORTER",
        "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
        "ASSIMP_BUILD_NO_LWO_IMPORTER",
        "ASSIMP_BUILD_NO_LWS_IMPORTER",
        "ASSIMP_BUILD_NO_M3D_IMPORTER",
        "ASSIMP_BUILD_NO_MD2_IMPORTER",
        "ASSIMP_BUILD_NO_MD3_IMPORTER",
        "ASSIMP_BUILD_NO_MD4_IMPORTER",
        "ASSIMP_BUILD_NO_MD5_IMPORTER",
        "ASSIMP_BUILD_NO_MDC_IMPORTER",
        "ASSIMP_BUILD_NO_MDL_IMPORTER",
        "ASSIMP_BUILD_NO_MMD_IMPORTER",
        "ASSIMP_BUILD_NO_MS3D_IMPORTER",
        "ASSIMP_BUILD_NO_NDO_IMPORTER",
        "ASSIMP_BUILD_NO_NFF_IMPORTER",

        "ASSIMP_BUILD_NO_OFF_IMPORTER",
        "ASSIMP_BUILD_NO_OGRE_IMPORTER",
        "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
        "ASSIMP_BUILD_NO_PLY_IMPORTER",
        "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
        "ASSIMP_BUILD_NO_Q3D_IMPORTER",
        "ASSIMP_BUILD_NO_RAW_IMPORTER",
        "ASSIMP_BUILD_NO_SIB_IMPORTER",
        "ASSIMP_BUILD_NO_SMD_IMPORTER",
        "ASSIMP_BUILD_NO_STEP_IMPORTER",
        "ASSIMP_BUILD_NO_STL_IMPORTER",
        "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
        "ASSIMP_BUILD_NO_X_IMPORTER",
        "ASSIMP_BUILD_NO_X3D_IMPORTER",
        "ASSIMP_BUILD_NO_XGL_IMPORTER"
    }
