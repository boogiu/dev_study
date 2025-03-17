

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <string>
#include "function.h"
#include "CBroker.h"

using namespace std;

#define SAFE_DELETE(p) {if(p){ delete p; p =nullptr;}}

enum SCENE
{
	START,
	SELECT,
	VILLAGE,
	END
};

extern SCENE nowScene;
extern CBroker broker;

#endif //PCH_H
