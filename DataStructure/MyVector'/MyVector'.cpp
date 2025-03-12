//벡터 직접 구현(integer만)
/*
******구현 목표
******1. pushback / remove
******2. 복사 생성자 / 깊은 복사 
******3. operator 연산자
*/

#include <iostream>

namespace myVector {
	class vector
	{
	public:
		vector();
		vector(const vector& _vector); //복사 생성자
		~vector();
		void push_back(int i);
		void remove(int _index);
		
		int operator[](int i);
		bool operator==(const vector& _vector);
		vector& operator=(const vector& _vector);

	public:
		int size() const{
			return length;
		}
	private:
		int length;
		int capacity;
		int* content; //배열의 대표 주소를 저장
	};

	vector::vector() : length(0), capacity(2)
	{
		content = new int[capacity];
	}

	vector::vector(const vector& _vector):length(_vector.length),capacity(_vector.capacity)
	{
		content = new int[capacity];
		for (int i = 0; i < length; ++i) {
			content[i] = _vector.content[i];
		}
	}

	vector::~vector()
	{
		delete[] content;
	}

	void vector::push_back(int i)
	{
		if (length >= capacity) {
			int* tmpContent;
			tmpContent = new int[capacity * 2]; //두배 증가
			for (int i = 0; i < length; ++i) {
				tmpContent[i] = content[i]; // 데이터 옮기기
			}
			delete[] content; //이전 배열 삭제
			content = tmpContent; //벡터가 가리키는 주소 변경

			capacity *= 2; //용량 2배 
		}
		content[length] = i; //마지막에 추가
		++length;
	}

	void vector::remove(int _index)
	{

		//예외처리.
		if (_index < 0 || _index >= length) { return; }

		//5번째를 지우고 싶다면. 5번째가 -> 6번째가 되면 됨.
		for (int i = 0; i < length - (_index); ++i) {
			content[i + _index] = content[i + _index + 1]; //언제까지? : 10개 중 5번째 45 56 67 78 89 
		}

		length -= 1; //이러면 맨 뒤에는 그냥 없는셈 치는거로.

		//만약 capacity가 너무 size에 비해 많다면?
		if (length * 2 < capacity) {
			int* tmpContent;
			tmpContent = new int[length +2]; //현재보다 2개만 많은걸로
			for (int i = 0; i < length; ++i) {
				tmpContent[i] = content[i]; // 데이터 옮기기
			}
			delete[] content; //이전 배열 삭제
			content = tmpContent; //벡터가 가리키는 주소 변경

			capacity = length +2; //용량 변경 
		}
	}

	int vector::operator[](int i)
	{
		if (i > length - 1 || i < 0) { throw std::out_of_range("Index out of range"); }
		return content[i];
	}

	bool vector::operator==(const vector& _vector)
	{
		if (length != _vector.size())
		{
			return false;
		}
		for (int i = 0; i < length; ++i) {
			if (content[i] == _vector.content[i]) {
				continue;
			}
			else {
				return false;
			}
		}
		return true;
	}
	vector& vector::operator=(const vector& _vector) {
		if (&_vector == this) { return; }
		delete[] content;
		content = nullptr;
		capacity = _vector.capacity;
		length = _vector.length;

		content = new int[_vector.capacity];
		for (int i = 0; i < length; ++i) {
			content[i] = _vector.content[i];
		}

		return *this;
	}


}

using namespace myVector;

int main()
{
	std::cout << "Hello World!\n";

	vector tstVector;
	tstVector.push_back(1);
	tstVector.push_back(2);
	tstVector.push_back(3);
	tstVector.push_back(5);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);
	tstVector.push_back(6);

	tstVector.push_back(6);

	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	tstVector.remove(3);
	
	tstVector.push_back(7);

	for (int i = 0; i < tstVector.size(); ++i) {
		std::cout << tstVector[i] << std::endl; //5없어야함
	}

}
