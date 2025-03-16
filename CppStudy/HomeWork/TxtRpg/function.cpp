#include "pch.h"
#include "function.h"

int SafeInt(int _first, int _last)
{
	while (true) {
		int tmp(0);
		cin >> tmp;

		if (cin.fail()) {
			cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
			cin.clear();
			cin.ignore();
			continue;
		}
		if (tmp < _first || tmp > _last) {
			cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
			cin.clear();
			cin.ignore();
			continue;
		}
		return tmp;
	}
	return 0;
}

char* SafeChar()
{
		char tmp[32];
		char* result;

		while (true) {
			cin.getline(tmp, 32);
			if (cin.fail()) { // 입력 길이가 31자를 초과했을 경우
				cout << "입력 가능한 범위를 벗어났습니다. (최대 31자)\n";
				cout << "다시 입력해주세요.\n";
				cin.ignore(100, '\n'); // 남아 있는 입력 제거
				cin.clear(); // 오류 플래그 초기화
			}
			else {
				break;
			}
		}

		int length = strlen(tmp); // 입력된 문자열 길이
		result = new char[length + 1]; // 동적 할당
		strcpy_s(result, length + 1, tmp); // 복사

		return result;
}
