#pragma once


#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;
#include "framework.h"
#include "Define.h"
#include "CRandom.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#ifndef DBG_NEW 
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
		#define new DBG_NEW 
	#endif
#endif