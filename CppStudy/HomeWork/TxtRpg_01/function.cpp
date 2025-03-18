#include "function.h"
#include "pch.h"
int SafeInt(int first, int last) {
	int iTmp(0);
	while (true) {
		cin >> iTmp;
		if (iTmp > last || iTmp < first || cin.fail()) {
			cout << "잘못 입력하셨습니다." << endl;
			cin.ignore();
			cin.clear();
			continue;
		}
		break;
	}
	cin.ignore();
	cin.clear();
	return iTmp;
}

char* SafeChar() {
	char cTmp[32];
	while (true)
	{
		cin >> cTmp;

		if (cin.fail()) {
			cout << "잘못 입력하셨습니다." << endl;
			cin.ignore();
			cin.clear();
			continue;
		}
		break;
	}

	char* result = new char[32]; // 널문자 포함 +1
	strcpy_s(result, sizeof(result), cTmp);

	return result;
}
