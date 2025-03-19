'#pragma once
#include <iostream>;

namespace TemplateStd {
	template <typename A, typename B>
	class InstanceTemplate
	{
	public:
		void Add(A a, B b) {
			std::cout << a + b << std::endl;
		}
	};

	//템플릿 특수화
	template <typename B>
	class InstanceTemplate<double, B> {
	public:
		void Double(double a, B b) {
			std::cout << a * b << std::endl;
		}
	};

	//템플릿 특수화
	template <>
	class InstanceTemplate<char, char> {
	public:
		void Double(char a, char b) {
			std::cout << a << b << std::endl;
		}
	};

	//함수 템플릿
	template<typename B>
	void coutAll(B _b) {
		std::cout << _b << std::endl;
	}


	//함수 객체 (functor) : 아래는 벡터의 일부라고 가정. size 및 swap 모두 구현되어 있음.
	template<typename cont, typename comp>
	void bubble_sort(cont& _cont, comp& _comp) {
		for (int i = 0; i < _cont.size(); ++i) {
			for (int j = i + 1; j < _cont.size(); j++) {
				if (!comp(cont[i], cont[j])) {
					cont.swap(i, j);
				}
			}
		}

		struct comp1 {
			bool operator() (int a, int b) { return a > b };
		};

		struct comp2 {
			bool operator() (int a, int b) { return a < b; }
		};

		/*
			!comp(cont[i], cont[j])
			부분을 확인했을 때, copm1 comp2는 객체이지만, 함수와 같이 활용하고 있음.
			단지,  operator () 연산자 오버로딩을 통해서 함수를 호출하는 것처럼 보이게 할 뿐.

			이것이 functor다.
			이것을 쓰는 이유는 무엇인가?

			위의 template<typename cont, typename comp>과 같이
			"어떻게 비교를 할 것인가?" 를 상황에 따라 정의하고 싶을 때, 함수 역할을 하는 객체를 함께 인자로 전달하여
			비교의 방식을 바꿀 수 있게 된다.
			ex)bubble_sort(vector, comp1)을 하면 오름차, bubble_sort(vector,comp2)를 하면 내림차로 정렬.
		*/
	}
}

