#include "CString.h"
#include <string>
using namespace std;

CString::CString()
    : m_cData(nullptr),m_iLength(0),m_iCapacity(0)
{
}

CString::CString(const char* _cValue)
    : m_cData(nullptr), m_iLength(0), m_iCapacity(0)
{
    if (!_cValue) { return; }

    int iLength = strlen(_cValue);
    
    m_iLength = iLength; 
    m_iCapacity = iLength + 1;  //널문자

    m_cData = new char[m_iCapacity];

    strcpy_s(m_cData,  sizeof(char)* m_iCapacity, _cValue);
    m_cData[iLength] = '\0';
}

CString::CString(int _length)
    : m_cData(nullptr), m_iLength(_length), m_iCapacity(_length+1)
{
    m_cData = new char[m_iCapacity];
}

CString::CString(const CString& rhs)
    : m_cData(nullptr), m_iLength(0), m_iCapacity(0)
{
    if (!rhs.c_str()) { return; }

    m_iLength = rhs.size();
    m_iCapacity = m_iLength + 1;  //널문자

    m_cData = new char[m_iCapacity];

    strcpy_s(m_cData, sizeof(char) * m_iCapacity, rhs.c_str());
    m_cData[m_iLength] = '\0';
}

CString::~CString()
{
    delete[] m_cData;
}

CString CString::operator+(const CString& rhs)
{
    int tmpSize = rhs.size() + m_iLength;

    char* tmpData = new char[tmpSize + 1];

    strcpy_s(tmpData, tmpSize+1, m_cData);
    strcat_s(tmpData, tmpSize+1, rhs.c_str());
    
    CString Result(tmpData);
   
    delete[] tmpData;
    return Result;
}

CString& CString::operator=(const CString& rhs)
{
    delete[] m_cData;
    m_iLength = rhs.size();
    m_iCapacity = rhs.capacity();
    m_cData = new char[m_iCapacity];
    strcpy_s(m_cData, m_iCapacity, rhs.c_str());
    return *this;
}

bool CString::operator==(const CString& rhs)
{
    return(!strcmp(m_cData, rhs.c_str()));
}
