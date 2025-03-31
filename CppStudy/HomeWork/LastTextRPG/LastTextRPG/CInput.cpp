#include <conio.h>
#include "pch.h"
#include "CInput.h"

int CInput::SafeInt(int start, int last)
{
	int tmp(0);
	while (true) {
		cin >> tmp;
		if (cin.fail()) {
			wcout << L"잘못된 입력입니다. " << endl;
			cin.clear();
			cin.ignore(100, '\n');
			continue;
		}
		if (tmp < start || tmp > last) {
			wcout << L"범위를 벗어난 입력입니다. " << endl;
			continue;
		}

		break;
	}
	cin.clear();
	cin.ignore(100, '\n');
	return tmp;
}

wstring CInput::SafeChar()
{
	wstring Buffer;
	getline(wcin, Buffer);  
	return Buffer;
}


KEY CInput::GetKey()
{
	//위: 72 오른쪽 : 77 아래 : 80 왼쪽 : 75
	switch (getCommand()) {
	case 72:
		return KEY::UP;
	case 77:
		return KEY::RIGHT;
	case 80:
		return KEY::DOWN;
	case 75:
		return KEY::LEFT;
	case 27:
		return KEY::ESC;
	default:
		return KEY::NONE;
	}
}

int CInput::getCommand() {
	while (!_kbhit()) {
	}
	while (true)
	{
		int iKey = _getch();
		if (iKey == 72 || iKey == 77 || iKey == 80 || iKey == 75 || iKey==27) {
			return iKey;
		}
	}
}