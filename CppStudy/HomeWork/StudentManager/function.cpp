#include "function.h"

int SafeCIn(int _first, int _last) // 범위 지정 :: 이상 이하
{
	int tmp = 0;
	while (true) {
		cin >> tmp;

		if (cin.fail()) { //자료형 확인
			cout << "잘못 입력했습니다." << endl;
			cout << "다시 입력해주세요." << endl;

			cin.clear();
			cin.ignore(100, '\n'); // 남아 있는 입력 제거
			continue;
		}

		if (tmp > _last || tmp < _first) { //범위 확인
			cout << "입력 가능한 범위를 벗어났습니다." << endl;
			cout << "다시 입력해주세요." << endl;
			continue;
		}

		cin.ignore(100, '\n'); // 남아 있는 입력 제거
		break;
	}
	return tmp;
}

char* SafeCharIn() {
	char _tmp[32]; // 최대 31글자 + 널문자('\0')
	char* result;

	while (true) {
		cin.getline(_tmp, 32);
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

	//cin.ignore(100, '\n'); getLine은 \n전까지 읽기 때문에
	//정상 작동 이후에 ignore해주면 엔터가 먹히지 않는 상황이 발생할 수 잇음.

	int length = strlen(_tmp); // 입력된 문자열 길이
	result = new char[length + 1]; // 동적 할당
	strcpy_s(result, length + 1, _tmp); // 복사

	return result;
}
