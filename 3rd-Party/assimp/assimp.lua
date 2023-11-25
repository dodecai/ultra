project "assimp"
  kind "StaticLib"
  language "C++"
  conformancemode "true"
  cdialect "C17"
  cppdialect "C++20"
  disablewarnings { "4244", "4996" }
  staticruntime "on"
  toolset "msc"
  warnings "Default"

  defines {
      -- Options
      -- ASSIMP_ASAN
      -- ASSIMP_UBSAN
      -- "ASSIMP_BUILD_ZLIB",
      -- "ASSIMP_BUILD_DOUBLE_PRECISION",
      -- "BUILD_SHARED_LIBS",
      "ASSIMP_BUILD_NO_EXPORT",
      "ASSIMP_BUILD_NO_OWN_ZLIB",

      -- AssetLib
      "ASSIMP_BUILD_NO_3D_IMPORTER",
      "ASSIMP_BUILD_NO_3DS_IMPORTER",
      "ASSIMP_BUILD_NO_3MF_IMPORTER",
      "ASSIMP_BUILD_NO_AC_IMPORTER",
      "ASSIMP_BUILD_NO_AMF_IMPORTER",
      "ASSIMP_BUILD_NO_ASE_IMPORTER",
      "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
      "ASSIMP_BUILD_NO_B3D_IMPORTER",
      "ASSIMP_BUILD_NO_BLEND_IMPORTER",
      "ASSIMP_BUILD_NO_BVH_IMPORTER",
      "ASSIMP_BUILD_NO_C4D_IMPORTER",
      "ASSIMP_BUILD_NO_COB_IMPORTER",
      "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
      "ASSIMP_BUILD_NO_CSM_IMPORTER",
      "ASSIMP_BUILD_NO_DXF_IMPORTER",
      "ASSIMP_BUILD_NO_FBX_IMPORTER",
      "ASSIMP_BUILD_NO_GLTF_IMPORTER",
      -- "ASSIMP_BUILD_NO_GLTF1_IMPORTER",
      -- "ASSIMP_BUILD_NO_GLTF2_IMPORTER",
      "ASSIMP_BUILD_NO_HMP_IMPORTER",
      "ASSIMP_BUILD_NO_IFC_IMPORTER",
      "ASSIMP_BUILD_NO_IQM_IMPORTER",
      "ASSIMP_BUILD_NO_IRR_IMPORTER",
      "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
      "ASSIMP_BUILD_NO_LWO_IMPORTER",
      "ASSIMP_BUILD_NO_LWS_IMPORTER",
      "ASSIMP_BUILD_NO_M3D_IMPORTER",
      "ASSIMP_BUILD_NO_MD2_IMPORTER",
      "ASSIMP_BUILD_NO_MD3_IMPORTER",
      "ASSIMP_BUILD_NO_MD5_IMPORTER",
      "ASSIMP_BUILD_NO_MDC_IMPORTER",
      "ASSIMP_BUILD_NO_MDL_IMPORTER",
      "ASSIMP_BUILD_NO_MMD_IMPORTER",
      "ASSIMP_BUILD_NO_MS3D_IMPORTER",
      "ASSIMP_BUILD_NO_NDO_IMPORTER",
      "ASSIMP_BUILD_NO_NFF_IMPORTER",
      -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
      "ASSIMP_BUILD_NO_OFF_IMPORTER",
      "ASSIMP_BUILD_NO_OGRE_IMPORTER",
      "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
      "ASSIMP_BUILD_NO_PLY_IMPORTER",
      "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
      "ASSIMP_BUILD_NO_Q3D_IMPORTER",
      "ASSIMP_BUILD_NO_RAW_IMPORTER",
      "ASSIMP_BUILD_NO_SIB_IMPORTER",
      "ASSIMP_BUILD_NO_SMD_IMPORTER",
      "ASSIMP_BUILD_NO_STL_IMPORTER",
      "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
      "ASSIMP_BUILD_NO_X_IMPORTER",
      "ASSIMP_BUILD_NO_X3D_IMPORTER",
      "ASSIMP_BUILD_NO_XGL_IMPORTER",

      -- ProcessHelper
      -- "ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS",
      "ASSIMP_BUILD_NO_DEBONE_PROCESS",
      "ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS",
      "ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS",
      "ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS",
      "ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS",
      "ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS",
      -- "ASSIMP_BUILD_NO_FLIPUVS_PROCESS",
      -- "ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS",
      "ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS",
      "ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS",
      -- "ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS",
      "ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS",
      "ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS",
      "ASSIMP_BUILD_NO_JOINVERTICES_PROCESS",
      "ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS",
      -- "ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS",
      "ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS",
      "ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS",
      "ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS",
      "ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS",
      "ASSIMP_BUILD_NO_REMOVEVC_PROCESS",
      "ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS",
      "ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS",
      "ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS",
      "ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS",
      -- "ASSIMP_BUILD_NO_TRIANGULATE_PROCESS",
      -- "ASSIMP_BUILD_NO_VALIDATEDS_PROCESS",
  }

  files {
      "assimp.lua",
      "include/**",
      "revision.h",
      "code/AssetLib/Collada/ColladaLoader.cpp",
      "code/AssetLib/Collada/ColladaParser.cpp",
      "code/AssetLib/Ply/PlyParser.cpp",
      "code/AssetLib/Ply/PlyLoader.cpp",
      "code/AssetLib/Obj/ObjFileImporter.cpp",
      "code/AssetLib/Obj/ObjFileMtlImporter.cpp",
      "code/AssetLib/Obj/ObjFileParser.cpp",
      "code/AssetLib/glTF/glTFImporter.cpp",
      "code/AssetLib/glTF2/glTF2Importer.cpp",

      "code/CAPI/CInterfaceIOWrapper.cpp",

      "code/Common/Assimp.cpp",
      "code/Common/AssertHandler.cpp",
      "code/Common/BaseImporter.cpp",
      "code/Common/BaseProcess.cpp",
      "code/Common/CreateAnimMesh.cpp",
      "code/Common/DefaultIOStream.cpp",
      "code/Common/DefaultIOSystem.cpp",
      "code/Common/DefaultLogger.cpp",
      "code/Common/Exceptional.cpp",
      "code/Common/Importer.cpp",
      "code/Common/ImporterRegistry.cpp",
      "code/Common/IOSystem.cpp",
      "code/Common/material.cpp",
      "code/Common/PostStepRegistry.cpp",
      "code/Common/scene.cpp",
      "code/Common/ScenePreprocessor.cpp",
      "code/Common/SGSpatialSort.cpp",
      "code/Common/SkeletonMeshBuilder.cpp",
      "code/Common/SpatialSort.cpp",
      "code/Common/Version.cpp",
      "code/Common/VertexTriangleAdjacency.cpp",

      "code/Material/MaterialSystem.cpp",
      
      
      "code/PostProcessing/ArmaturePopulate.cpp",
      "code/PostProcessing/GenBoundingBoxesProcess.cpp",
      "code/PostProcessing/GenVertexNormalsProcess.cpp",
      "code/PostProcessing/EmbedTexturesProcess.cpp",
      "code/PostProcessing/ConvertToLHProcess.cpp",
      
      "code/PostProcessing/ProcessHelper.cpp",
      "code/PostProcessing/MakeVerboseFormat.cpp",
      "code/PostProcessing/CalcTangentsProcess.cpp",
      "code/PostProcessing/ScaleProcess.cpp",
      "code/PostProcessing/ScaleProcess.cpp",
      "code/PostProcessing/EmbedTexturesProcess.cpp",
      "code/PostProcessing/TriangulateProcess.cpp",
      "code/PostProcessing/ValidateDataStructure.cpp",
	  -- "contrib/rabidjson/**.c",
	  -- "contrib/rabidjson/**.h",
	  -- "contrib/pugixml/**.c",
	  -- "contrib/pugixml/**.h",
	  -- "contrib/zlib/*.c",
	  -- "contrib/zlib/*.h",
      "contrib/irrXML/*",
  }

  includedirs {
	  "./",
      "include",
      "code",
      "contrib",
      -- "contrib/rapidjson/include",
      -- "contrib/pugixml/src",
      -- "contrib/unzip",
      -- "contrib/utf8cpp",
      "contrib/irrXML",
      "contrib/zlib",
  }
  
  filter  "configurations:Debug"
    runtime "Debug"
    optimize "on"
    symbols "off"

  filter  "configurations:Release"
    runtime "Release"
    optimize "on"
      
  filter "system:windows"
    systemversion "latest"
