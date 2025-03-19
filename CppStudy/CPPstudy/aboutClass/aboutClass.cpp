//클래스에 대한 공부

#include <iostream>

namespace myClass {
	class Kitty
	{
	public:
		Kitty(); //생성자
		~Kitty(); //소멸자
		Kitty(const Kitty& _kitty); // 복사 생성자
		Kitty& operator = (const Kitty& _kitty); //대입 연산자
	private:

	};

}
