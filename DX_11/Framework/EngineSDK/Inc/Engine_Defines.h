#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d11.h>
#include <d3dcompiler.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <memory>
#include <process.h>
#include <typeindex>
#include <filesystem>
#include <DirectXMath.h>

using namespace std; 
using namespace DirectX;

#include <FMOD_Inc/fmod.hpp>
#include <Effect_Inc/d3dx11effect.h>

// 기존 new 매크로로 인해 충돌 방지
#pragma push_macro("new")
#undef new
#include "GUI_Inc/imgui.h"
#include "GUI_Inc/backends/imgui_impl_win32.h"
#include "GUI_Inc/backends/imgui_impl_dx11.h"
#include "NFD_Inc/nfd.h"
#pragma pop_macro("new")

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Build_Struct.h"

using namespace Engine;

#include <windowsx.h>

#pragma warning(disable :4251) //맵 컨테이너 경고 무시

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

#endif // Engine_Define_h__
