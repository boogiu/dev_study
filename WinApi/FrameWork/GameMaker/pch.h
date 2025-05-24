#ifndef PCH_H
#define PCH_H

// 윈도우 & GDI 관련
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Msimg32.lib")
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

//공용 프로젝트 헤더
#include "framework.h"
#include "EnumDefs.h"
#include "Define.h"
#include "Structures.h"
#include "Templates.h"

// STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <io.h>
#include <cmath>    
#include <math.h>
 using namespace std;

//  FMOD
#include "./inc/fmod.hpp"
#include "./inc/fmod_errors.h"

// 디버그 / 릴리즈 분기 처리
#ifdef _DEBUG
#pragma comment(lib, "./lib/x64/fmod_vc.lib")   // 디버그용 라이브러리
#else
#pragma comment(lib, "./lib/x64/fmod_vc.lib")    // 릴리즈용 라이브러리
#endif

using namespace FMOD;

// ★ 전역 윈도우 핸들
extern HWND g_hWnd;

// ★ CRT 메모리 누수 추적 (디버그 전용)
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif

#endif // PCH_H
