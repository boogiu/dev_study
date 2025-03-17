

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <string>
#include "function.h"

using namespace std;

#define SAFE_DELETE(p) {if(p){ delete p; p =nullptr;}}


#endif //PCH_H
