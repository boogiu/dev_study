#pragma once
#include "framework.h"
#include "define.h"

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

extern HWND g_hWnd;

//디버그 용 헤더
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif