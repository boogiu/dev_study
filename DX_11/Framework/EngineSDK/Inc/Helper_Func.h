#pragma once
#include "Engine_Defines.h"

namespace Helper
{
	ENGINE_DLL _bool ContainsNonAscii(const string& str);
	ENGINE_DLL _bool IsPathInProjectFolder(const string& path);
	ENGINE_DLL string OpenFile_Dialogue();
	ENGINE_DLL string OpenFolder_Dialogue();
	ENGINE_DLL vector<string> OpenMultiFolders();
	ENGINE_DLL string SaveFileDialog();
	ENGINE_DLL string SaveFileDialog(const string& fileName, const string& filter);
	ENGINE_DLL string SaveFileDialogByWinAPI(const string& fileName, const string& filterStr);
	ENGINE_DLL wstring ConvertToWideString(const string& str);
	ENGINE_DLL string ConvertToString(const wstring& wstr);
	ENGINE_DLL string GetFileNameWithExtension(const string& filePath);
	ENGINE_DLL string GetFileNameWithOutExtension(const string& filePath);
	ENGINE_DLL string GetFileBaseName(const string& fileName);
	ENGINE_DLL string  ToLower(const string& fileName);
	ENGINE_DLL HRESULT SaveTextureToDDs(ID3D11DeviceContext* pContext,const string& filePath, ID3D11ShaderResourceView* pSRV);
	ENGINE_DLL HRESULT SaveTextureToDDs(ID3D11DeviceContext* pContext,const wstring& filePath, ID3D11ShaderResourceView* pSRV);
};

