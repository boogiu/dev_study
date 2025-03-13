#ifndef _CStudentList_
#define _CStudentList_

#include "pch.h"
#include "CStudent.h"

class CStudentList
{
public:
	CStudentList();
	CStudentList(CStudent* student);
	~CStudentList();

public:
	void SetList();
	void PrintAll();
	void PrintOne(CStudent* student);
	void FindStudent(const char* _name);
	void AddStudent();

private:
	int m_Capacity;
	int m_Count;
	CStudent** m_StudentArr;
	int SafeCIn();
};

#endif // !_CStudentList_
