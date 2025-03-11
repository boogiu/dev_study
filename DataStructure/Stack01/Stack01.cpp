/*
주요 포인트
******	1. 맵을 통해서 두 문자를 비교하는 것.			******
******	if문과 switch문보다 간결하고 깔끔하게 가능함.	******
******	2. 회전하는 것처럼 보이는 것.					******
******	나머지 연산을 통해서 순서를 변경.				******
*/



#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include<iostream>
using namespace std;
int solution(string s);
int main()
{
	string test;
	cin >> test;
	
	cout << solution(test) << "개";
}
unordered_map<char, char> bracketPair = { {'(',')'},{'{','}'},{'[',']'} };

bool isValid(string &s, int start) {
	stack<char> stack;
	unsigned int sz = s.size();

	for (int i = 0; i < sz; i++) {
		char c = s[(start + i) % sz]; 
		// start가 0이라면, 0, 1, 2, 3, 4, 5
		//start가 1이라면, 1, 2, 3, 4, 5, 0
		//start가 2라면 , 2,3 ,4,5,0,1
		// 즉 실제 회전이 아니라, 회전하는 것처럼 기준이 변경되는 것.

		if (bracketPair.count(c)) { //count는 find와 유사. 맵으로 열린 괄호는 키로 사용했기에, 해당 사항이 참이면 닫힌 괄호라는 뜻. 
			if (stack.empty() || stack.top() != bracketPair[c])
				return false; //비어있거나, top과 같지 않으면 false
			stack.pop(); //그게 아니면 팝
		}
		else {
			stack.push(c);
		}
	}

	return stack.empty();
}

int solution(string s) {
	int iAnswer(0);
	int n = s.size();

	for (int i = 0; i < n; ++i) {
		iAnswer += isValid(s, i);
		//참은1 거짓은 0이니까.
	}
	return iAnswer;
}



