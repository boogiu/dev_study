// Practice05.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
string solution(int n, int k, vector<string> cmd);

int main()
{
	string arr = solution(8, 2, { "D 2","C", "U 3","C","D 4","C","U 2","Z","Z","U 1","C" });
	cout << arr;
}

string solution(int n, int k, vector<string> cmdArr) {
	//n은 행의 개수
	//k는 처음 선택한 행.

	vector<int> dataVector(n, 1);
	//1은 생존, 0은 선택.
	stack<int> deleteStack; // 삭제한 인덱스의 번호를 기억.
	int iSelect(k);

	for (string cmd : cmdArr) {
		switch (cmd[0])
		{
		case 'U'://데이터 범위 넘어가면 입력 x
			iSelect = (iSelect - (int)(cmd[2] - 48)) >= 0 ? (iSelect - (int)(cmd[2] - 48)) : iSelect;
			break;
		case 'D'://데이터 범위 넘어가면 입력 x
			iSelect = (iSelect + (int)(cmd[2] - 48)) >= 0 ? (iSelect + (int)(cmd[2] - 48)) : iSelect;
			break;
		case 'C': //마지막 행인 경우 위로. 여기서 삭제된 행은 건너 뛰어야함.
		{
			deleteStack.push(iSelect);
			dataVector[iSelect] = 0;
			if (n - 1 == iSelect) {
				while (1==dataVector[iSelect])
				{
					iSelect = (iSelect - 1);
				}
			}
			else {
				while (1 == dataVector[iSelect])
				{
					iSelect = (iSelect + 1);
				}
			}
		}
		break;
		case 'Z': //행 그대로
			if (!deleteStack.empty()) {
				dataVector[deleteStack.top()] = 1;
				deleteStack.pop();
			}
			break;
		default:
			break;
		}
	}

	string sAnswer("");
	for (int i = 0; i < dataVector.size(); ++i) {
		if (1 == dataVector[i]) {
			sAnswer += 'O';
		}
		else {
			sAnswer += 'X';
		}
	}
	return sAnswer;
}

