#include "pch.h"
#include "CStudent.h"
#include "CStudentList.h"

int main()
{
	CStudentList studentLsit;
	bool notSet = true;
	studentLsit.AddStudent();
	studentLsit.AddStudent();
	studentLsit.AddStudent();

	while (true) {
		cout << "=================" << endl;
		cout << "1. 전체 보기" << endl;
		cout << "2. 학생 검색" << endl;
		cout << "3. 학생 추가" << endl;
		cout << "4. 종료" << endl;
		int iSelect(0);
		cin >> iSelect;

		if (cin.fail()) {
			cout << "잘못 입력했습니다." << endl;
			cin.clear();
			cin.ignore(100,'\n');
			continue;
		}
		system("cls");
		switch (iSelect)
		{
		case 1:
			studentLsit.PrintAll();
			break;
		case 2: {
			char szName[32];
			cout << ">>>검색할 학생의 이름을 입력해주세요" << endl;
			cin >> szName;
			studentLsit.FindStudent(szName);
		}
			break;
		case 3:
			studentLsit.AddStudent();
			break;

		case 4:
			return 0;
		default:
			break;
		}
		system("pause");
		system("cls");
	}
	studentLsit.PrintAll();
	
}
