#include "CStudent.h"


CStudent::CStudent(const char* _name)
	:m_cName(nullptr), m_iKor(0), m_iEng(0), m_iMath(0)
{
		if (_name) {
			int length = strlen(_name);
			m_cName = new char[length + 1];
			strcpy_s(m_cName, length + 1, _name);
		}
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

char* CStudent::getName() const
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
