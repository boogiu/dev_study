#pragma once
#include "Engine_Defines.h"
namespace GUIWidget
{
    ENGINE_DLL void ShowListButton(const vector<string>& vector, function<void(const string&)> callback);
    ENGINE_DLL void ShowList(const vector<string>& vector, function<void(const string&)> callback);
    ENGINE_DLL _vector Vector4Float(const string& Name,_fvector vector, bool Editable = true);
}