#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
# ☰ Library: stb
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# ☲ Properties
#━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
# ☷ Build
#‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
# The needed headers and sources to build the library.
set(STB_HEADERS
	".Library/stb/stb_image.h"
)
set(STB_SOURCES
	".Library/stb/stb_image.cpp"
)
add_library(stb ${STB_HEADERS} ${STB_SOURCES} ".Library/assimp/aabb.h" ".Library/assimp/ai_assert.h" ".Library/assimp/anim.h" ".Library/assimp/BaseImporter.h" ".Library/assimp/Bitmap.h" ".Library/assimp/BlobIOSystem.h" ".Library/assimp/ByteSwapper.h" ".Library/assimp/camera.h" ".Library/assimp/cexport.h" ".Library/assimp/cfileio.h" ".Library/assimp/cimport.h" ".Library/assimp/ColladaMetaData.h" ".Library/assimp/color4.h" ".Library/assimp/commonMetaData.h" ".Library/assimp/CreateAnimMesh.h" ".Library/assimp/DefaultIOStream.h" ".Library/assimp/DefaultIOSystem.h" ".Library/assimp/DefaultLogger.hpp" ".Library/assimp/Defines.h" ".Library/assimp/defs.h" ".Library/assimp/Exceptional.h" ".Library/assimp/Exporter.hpp" ".Library/assimp/fast_atof.h" ".Library/assimp/GenericProperty.h" ".Library/assimp/Hash.h" ".Library/assimp/Importer.hpp" ".Library/assimp/importerdesc.h" ".Library/assimp/IOStream.hpp" ".Library/assimp/IOStreamBuffer.h" ".Library/assimp/IOSystem.hpp" ".Library/assimp/light.h" ".Library/assimp/LineSplitter.h" ".Library/assimp/LogAux.h" ".Library/assimp/Logger.hpp" ".Library/assimp/LogStream.hpp" ".Library/assimp/material.h" ".Library/assimp/MathFunctions.h" ".Library/assimp/matrix3x3.h" ".Library/assimp/matrix4x4.h" ".Library/assimp/MemoryIOWrapper.h" ".Library/assimp/mesh.h" ".Library/assimp/metadata.h" ".Library/assimp/NullLogger.hpp" ".Library/assimp/ParsingUtils.h" ".Library/assimp/pbrmaterial.h" ".Library/assimp/postprocess.h" ".Library/assimp/Profiler.h" ".Library/assimp/ProgressHandler.hpp" ".Library/assimp/qnan.h" ".Library/assimp/quaternion.h" ".Library/assimp/RemoveComments.h" ".Library/assimp/scene.h" ".Library/assimp/SceneCombiner.h" ".Library/assimp/SGSpatialSort.h" ".Library/assimp/SkeletonMeshBuilder.h" ".Library/assimp/SmallVector.h" ".Library/assimp/SmoothingGroups.h" ".Library/assimp/SpatialSort.h" ".Library/assimp/StandardShapes.h" ".Library/assimp/StreamReader.h" ".Library/assimp/StreamWriter.h" ".Library/assimp/StringComparison.h" ".Library/assimp/StringUtils.h" ".Library/assimp/Subdivision.h" ".Library/assimp/texture.h" ".Library/assimp/TinyFormatter.h" ".Library/assimp/types.h" ".Library/assimp/vector2.h" ".Library/assimp/vector3.h" ".Library/assimp/version.h" ".Library/assimp/Vertex.h" ".Library/assimp/XmlParser.h" ".Library/assimp/XMLTools.h" ".Library/assimp/ZipArchiveIOSystem.h" ".Library/assimp/Compiler/poppack1.h" ".Library/assimp/Compiler/pstdint.h" ".Library/assimp/Compiler/pushpack1.h" ".Library/assimp/port/AndroidJNI/AndroidJNIIOSystem.h")
#‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
# ☷ IDE
#‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
set_property(TARGET stb PROPERTY FOLDER 3rd-Party/stb)
#■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
