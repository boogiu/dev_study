

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <string>
#include "function.h"

using namespace std;

#define SAFE_DELETE(p) {if(p){ delete p; p =nullptr;}}

enum SCENE
{
	START,
	SELECT,
	END
};

extern SCENE nowScene;

#endif //PCH_H
