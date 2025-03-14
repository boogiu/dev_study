#ifndef _CStudentList_
#define _CStudentList_

#include "pch.h"
#include "CStudent.h"

class CStudent;

class CStudentList
{
public:
	CStudentList();
	~CStudentList();

public:
	void PrintAll();
	void PrintOne(CStudent* student);
	void FindStudent(const char* _name);
	void AddStudent();

private:
	int m_Capacity;
	int m_Count;
	CStudent** m_StudentArr;
};

#endif // !_CStudentList_
