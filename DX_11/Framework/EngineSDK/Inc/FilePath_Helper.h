#pragma once
#include "Engine_Defines.h"

namespace FilePath_Helper
{
	 _bool ContainsNonAscii(const string& str);
	 _bool IsPathInProjectFolder(const string& path);
	 string OpenFile_Dialogue();
	 string OpenFolder_Dialogue();
	 string SaveFileDialog();
};

