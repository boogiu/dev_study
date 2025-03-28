#pragma once
#include "Define.h"

class CInput
{
public:
	static int SafeInt(int start, int last);
	static std::wstring SafeChar();
	static KEY GetKey();

private:
	static int getCommand();

};

