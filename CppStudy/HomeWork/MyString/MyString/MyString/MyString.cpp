#include <iostream>
#include "CString.h"

int main()
{
	CString str1("안녕");
	CString str2("하세요");

	CString str3 = str1 + str2;

	bool Same = (str3 == str2);
}
