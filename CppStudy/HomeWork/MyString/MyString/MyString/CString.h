#pragma once


class CString
{
public:
	CString();
	CString(const char* _cValue);
	CString(int _length);
	CString(const CString& rhs);

	~CString();

public:
	CString operator+ (const CString& rhs);
	CString& operator= (const CString& rhs);
	bool operator==(const CString& rhs);
public:
	const char* c_str() const { return m_cData; } // C-Style 문자열 반환
	int size() const { return m_iLength; };
	int capacity() const { return m_iCapacity; }
private:
	char* m_cData;
	int m_iLength;
	int m_iCapacity;
};

