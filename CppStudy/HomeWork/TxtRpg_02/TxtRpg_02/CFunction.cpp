#include "pch.h"
#include "CFunction.h"

int CFunction::SafeInt(int first, int last)
{
	int tmp(0);
	while (true)
	{
		std::cin >> tmp;

		if (std::cin.fail() || tmp < first || tmp>last) {
			std::cout << "잘못된 값의 입력이 발생했습니다." << std::endl;
			std::cin.clear();
			std::cin.ignore(100,'\n');
			continue;
		}

		break;
	}
	std::cin.ignore(100, '\n');
	std::cin.clear();

	return tmp;
}

//반드시 해제해줘야 함
char* CFunction::SafeChar()
{
	char buffer[64];
	while (true)
	{
		std::cin.getline(buffer, sizeof(buffer));
		
		if (std::cin.fail()) {
			std::cout << "잘못된 값의 입력이 발생했습니다." << std::endl;
			std::cin.ignore(100);
			std::cin.clear();
			continue;
		}
		break;
	}

	int length = strlen(buffer);
	char* result = new char[length + 1];
	strcpy_s(result, length+1, buffer);

	return result;
}

const char* CFunction::ReturnClass(E_CLASS _class)
{
	switch (_class)
	{
	case WARRIOR:
		return "워리어";
	case MAGE:
		return "메이지";
	case ROGUE:		
		return "로그";
	case ARCHOR:
		return "아처";
	default:
		return "백수";
	}
}
