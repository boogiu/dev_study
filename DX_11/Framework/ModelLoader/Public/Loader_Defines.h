#pragma once
#include <windows.h>
#include <d3d11.h>

#include "Engine_Defines.h"
namespace Loader {
	static constexpr unsigned int g_iWinSizeX = 1280;
	static constexpr unsigned int g_iWinSizeY = 720;

	static constexpr unsigned int g_iMainFrame = 144;
	static float g_iImportPreRotate= -0.f;
	static float g_iExportPreRotate= -180.f;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

using namespace Loader;

#pragma push_macro("new")
#undef new
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#pragma pop_macro("new")
using namespace Assimp;

struct TextureInfo
{
    TEXTURE_INFO_HEADER header;
};

struct TextureFile
{
    TEXTURE_FILE_HEADER header;
    vector<TextureInfo> textures;
};

struct MaterialInfo
{
    MATERIAL_INFO_HEADER header;
    vector<TextureFile> textureTypes;
};

struct MaterialFile
{
    MATERIAL_FILE_HEADER header;
    vector<MaterialInfo> materials;
};