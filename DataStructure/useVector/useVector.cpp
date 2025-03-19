// useVector.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <iostream>
#include <vector>

using namespace std;

//만약 템플릿으로 하고 싶다면
template<typename T>
void PrintVec(vector<T>& vec) {
	for (typename vector<T>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
		cout << *iter << endl;
	}//포문 안에 typename을 넣어주어야 함.
}

int main()
{
	vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);
	vec.push_back(50);
	vec.push_back(60);
	vec.erase(vec.begin() + 2); //30이 지워질까?
	//vec.erase(vec[2]);
	
	vec.insert(vec.begin() + 2, 90); //

	//주의 사항 
	//원소를 추가하거나 지우게 되면 기존에 사용하던 iterator는 사용할 수가 없다. 
	//따라서
	vector<int>::iterator iter = vec.begin();
	/*
	for (; iter != vec.end(); ++iter) {
		if (*iter == 50) {
			vec.erase(iter);
			iter = vec.begin(); //다시 이터레이터를 할당해주는 과정을 거쳐야 한다.
		}
	}
	*/
	//^위에 상황은 다만, 하나를 지우고 다시 처음부터 돌아가는 과정 즉, 50을 만나면 다시 begin부터 end까지 돈다. (비효율적)

	for (vector<int>::size_type i =0; i != vec.size(); ++i) { //size_type은 vec.size()했을 때 반환되는 size_t와 자료형을 맞춘 것임.
		if (vec[i] == 50) {
			vec.erase(vec.begin() + i); //그래서 이런 형태로 진행하는 것도 나쁘지 않다.
		}
	}


	PrintVec(vec);
}
