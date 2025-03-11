
//괄호 회전

#include <iostream>
#include<string>
#include<stack>

using namespace std;

int Solution(string str);
string Circular(string s, int x);
bool CheckRight(string str);

int main()
{
    string sInput;
    cin >> sInput;

    cout << Solution(sInput) << "개";
    return 0;
}

int Solution(string str) {
    int iAnswer(0);

    //문자열 개수만큼 반복
    for (int i = 0; i < str.size(); ++i) {
        //회전
        string tmpStr = Circular(str, i);
        //검사
        if (CheckRight(tmpStr)) {
            //카운트
            ++iAnswer;
        }
    }
    
    return iAnswer;
}//O(n^2)

string Circular(string s, int x) {
    //정해진 숫자만큼 맨 앞 글자를 뒤로
    for (int i = 0; i < x; ++i) {
        char tmpChar = s[0];
        s.erase(0,1);
        s += tmpChar;
    }

    return s;
    //O(n)
}

bool CheckRight(string str) {

    stack<char> stack;
    
    for (char c : str) {
        switch (c) {
            case '(':
            case '{':
            case '[':
                stack.push(c);
                break;
            case ')' :
                if (stack.empty()) {
                    return false;
                }
                if (stack.top() == '(') {
                    stack.pop();
                }
                break;
            case '}' :
                if (stack.empty()) {
                    return false;
                }
                if (stack.top() == '{') {
                    stack.pop();
                }
                break;
            case ']' :
                if (stack.empty()) {
                    return false;
                }
                if (stack.top() == '[') {
                    stack.pop();
                }
                break;
                
            default:
                cout << "잘못된 입력";
                break;
        }

    }
    return stack.empty();

    //O(n)
}