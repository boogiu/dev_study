
#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool solution(string str);

int main()
{
    string test;
    cin >> test;
    cout << solution(test);
}

bool solution(string str) {
    stack<char> stack;

    for (char c : str) {
        if (stack.empty()) {
            stack.push(c);
        }
        else if (stack.top() == c) {
            stack.pop();
        }
        else {
            stack.push(c);
        }
    }

    return stack.empty();
}
