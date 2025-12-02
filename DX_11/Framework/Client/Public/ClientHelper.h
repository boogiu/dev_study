#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

namespace ClientHelper
{
	namespace Engine {
		class CAnimator3D;
	}
	HRESULT Add_ModelPathFromDirectory(string directoryPath);
	HRESULT Add_MaterialPathFromDirectory(string directoryPath);
	HRESULT Add_AnimPathFromDirectory(string directoryPath, string Subject);
	HRESULT Add_TexturePathFromDirectory(string directoryPath);
	HRESULT Add_SoundClips(string directoryPath);
	HRESULT Add_AllClipsByFile(string filePath, string Level, string Subject, CAnimator3D* pAnimator);
	wstring ExtractVowels(const std::wstring& src);
	wchar_t  MapJungToAEIOU(int jungIndex);
	itemType Get_Type(const string& typeString);
};

