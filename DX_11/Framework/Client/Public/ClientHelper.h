#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

namespace ClientHelper
{
	HRESULT Add_ModelPathFromDirectory(string directoryPath);
	HRESULT Add_MaterialPathFromDirectory(string directoryPath);
	HRESULT Add_AnimPathFromDirectory(string directoryPath, string Subject);
	HRESULT Add_TexturePathFromDirectory(string directoryPath);
};

