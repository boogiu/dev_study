#include "CStudentList.h"

CStudentList::CStudentList()
	:m_StudentArr(nullptr), m_Capacity(1), m_Count(0)
{
	m_StudentArr = new CStudent * [m_Capacity];
}

CStudentList::CStudentList(CStudent* student)
	:m_StudentArr(nullptr), m_Capacity(1), m_Count(1)
{
	m_StudentArr = new CStudent * [m_Capacity];
	m_StudentArr[0] = student;
}

CStudentList::~CStudentList()
{
	for (int i = 0; i < m_Count; ++i) {
		if (m_StudentArr != nullptr) {
			delete m_StudentArr[i];
			m_StudentArr[i] = nullptr;
		}
	}
	delete[] m_StudentArr;
}

void CStudentList::SetList()
{
	int iCount = 0;
	while (iCount<3)
	{
		AddStudent();
		++iCount;
	}
}

void CStudentList::PrintAll()
{
	for (int i = 0;i < m_Count; ++i) {
		cout << "=====================================" << endl;
		cout << "학생 :" << m_StudentArr[i]->getName() << endl;
		cout << "국어 :" << m_StudentArr[i]->getScore(KOR) << endl;
		cout << "영어 :" << m_StudentArr[i]->getScore(ENG) << endl;
		cout << "수학 :" << m_StudentArr[i]->getScore(MATH) << endl;
		cout << "평균 :" << m_StudentArr[i]->getAverage() << endl;
		cout << "=====================================" << endl;
	}
}

void CStudentList::PrintOne(CStudent* student)
{
	cout << "=====================================" << endl;
	cout << "학생 :" << student->getName() << endl;
	cout << "국어 :" << student->getScore(KOR) << endl;
	cout << "영어 :" << student->getScore(ENG) << endl;
	cout << "수학 :" << student->getScore(MATH) << endl;
	cout << "평균 :" << student->getAverage() << endl;
	cout << "=====================================" << endl;
}

void CStudentList::FindStudent(const char* _name)
{
	for (int i = 0; i < m_Count; ++i) {
		if (!strcmp(m_StudentArr[i]->getName(), _name)) {
			PrintOne(m_StudentArr[i]);
			return;
		}
	}
	cout << "찾는 학생 없음" << endl;
}

void CStudentList::AddStudent()
{
	if (m_Count == m_Capacity) {
		CStudent** tmp = new CStudent * [m_Capacity * 2];
		for (int i = 0; i < m_Count; ++i) {
			tmp[i] = m_StudentArr[i];
		}
		delete[] m_StudentArr;
		//재할당
		m_StudentArr = tmp;
		m_Capacity *= 2;
	}

	char szName[32];
	int score1(0);
	int score3(0);
	int score2(0);

	cout << "=======================" << endl;
	cout << "학생의 이름은? : ";
	cin >> szName;
	cout << endl;
	cout << "국어 점수 : ";
	score1 = SafeCIn();
	cout << endl;

	cout << "수학 점수 : ";
	score2 = SafeCIn();
	cout << endl;

	cout << "영어 점수 : ";
	score3 = SafeCIn();
	cout << endl;

	CStudent* tmpStudent = new CStudent(szName, score1, score2, score3);
	m_StudentArr[m_Count] = tmpStudent;
	++m_Count;
}


int CStudentList::SafeCIn()
{
	int tmp = 0;
	while (true) {
		cin >> tmp;
		if (cin.fail()) {
			cout << "잘못 입력했습니다." << endl;
			cout << "다시 입력해주세요." << endl;

			cin.clear();
			cin.ignore(100, '\n');
			continue;
		}
		break;
	}
	return tmp;
}