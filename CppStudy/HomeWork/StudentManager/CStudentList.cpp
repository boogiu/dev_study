#include "CStudentList.h"

CStudentList::CStudentList()
	:m_StudentArr(nullptr), m_Capacity(5), m_Count(0)
{
	m_StudentArr = new CStudent * [m_Capacity]; //초기 용량 5명
	
	AddStudent();
	AddStudent();
	AddStudent();

}

CStudentList::~CStudentList()
{
	for (int i = 0; i < m_Count; ++i) {
		if (m_StudentArr != nullptr) {
			delete m_StudentArr[i]; // 순회하며 해제
			m_StudentArr[i] = nullptr; //댕글링 포인터 발생하지 않도록.
		}
	}
	delete[] m_StudentArr;
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
	int iCount(0); //해당하는 이름을 가진 학생 수

	for (int i = 0; i < m_Count; ++i) {
		if (!strcmp(m_StudentArr[i]->getName(), _name)) { //이름 비교 같으면 0반환
			PrintOne(m_StudentArr[i]);
			++iCount;
		}
	}
	cout << "===========================" << endl;
	cout << "찾는 학생" <<iCount<<"명 검색 됨." << endl;
}

void CStudentList::AddStudent()
{
	if (m_Count == m_Capacity) {
		CStudent** tmp = new CStudent * [m_Capacity * 2]; //용량이 이미 가득찼으면 메모리 재할당
		for (int i = 0; i < m_Count; ++i) {
			tmp[i] = m_StudentArr[i];
		}
		delete[] m_StudentArr;
		//재할당
		m_StudentArr = tmp;
		m_Capacity *= 2;
	}
	
	m_StudentArr[m_Count] = new CStudent(); //하나 생성
	++m_Count;
}

