#pragma once
#include "Engine_Defines.h"

namespace Helper
{
	_bool ContainsNonAscii(const string& str);
	_bool IsPathInProjectFolder(const string& path);
	string OpenFile_Dialogue();
	string OpenFolder_Dialogue();
	string SaveFileDialog();
	wstring ConvertToWideString(const string& str);
	string ConvertToString(const wstring& wstr);
};

