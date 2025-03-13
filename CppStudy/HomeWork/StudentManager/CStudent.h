#ifndef _CStudent_
#define _CStudent_

#include "pch.h"

enum SUBJECT{
	KOR=1,
	ENG,
	MATH,
	NONE
};

class CStudent
{
public:
	CStudent(const char* _name);
	CStudent(const char* _name, int i_kor, int i_eng, int i_math);
	~CStudent();

public:
	void setScore(SUBJECT _subject, int _score);
	void setName(const char* _name);

	int getScore(SUBJECT _subject) const;
	float getAverage() const;
	char* getName() const;
private:
	char* m_cName;
	int m_iKor;
	int m_iEng;
	int m_iMath;
};

#endif // !1
