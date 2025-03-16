#include "CStudent.h"
#include "function.h"


CStudent::CStudent()
	:m_cName(nullptr), m_iKor(0), m_iEng(0), m_iMath(0)
{
	cout << "=======================" << endl;
	cout << "학생의 이름은? : ";

	m_cName = SafeCharIn();

	cout << endl;
	cout << "국어 점수 : ";
	m_iKor = SafeCIn(0, 100);
	cout << endl;

	cout << "수학 점수 : ";
	m_iEng = SafeCIn(0, 100);
	cout << endl;

	cout << "영어 점수 : ";
	m_iMath = SafeCIn(0, 100);
	cout << endl;
}

CStudent::CStudent(const char* _name, int i_kor, int i_eng, int i_math)
	:m_cName(nullptr), m_iKor(i_kor), m_iEng(i_eng), m_iMath(i_math)
{
	if (_name) {
		int length = strlen(_name);
		m_cName = new char[length + 1];
		strcpy_s(m_cName, length + 1, _name);
	}
}

CStudent::~CStudent()
{
	if (m_cName) {
		delete[] m_cName;
	}
}

void CStudent::setScore(SUBJECT _subject, int _score) {
	switch (_subject)
	{
	case KOR:
		m_iKor = _score;
		break;
	case ENG:
		m_iEng = _score;
		break;
	case MATH:
		m_iMath = _score;
		break;
	default:
		break;
	}
}

int CStudent::getScore(SUBJECT _subject) const
{
	switch (_subject)
	{
	case KOR:
		return m_iKor;
	case ENG:
		return m_iEng;
	case MATH:
		return m_iMath;
	default:
		break;
	}
}

float CStudent::getAverage() const
{
	return static_cast<float>(m_iKor + m_iEng + m_iMath) / 3.f;;
}

const char* CStudent::getName() const
{
	return m_cName;
}

void CStudent::setName(const char* _name)
{
	if (_name) {
		int length = strlen(_name);
		delete[] m_cName;

		m_cName = new char[length + 1];
		strcpy_s(m_cName, length + 1, _name);

	}
}
