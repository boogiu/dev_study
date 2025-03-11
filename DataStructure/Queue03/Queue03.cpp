// Queue03.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;

string solution(vector<string> card1, vector<string> card2, vector<string> goal);
queue<string> IntoQueue(vector<string> _vec);

int main()
{
    vector<string> tstCard1 = {"I", "Drink","Water"};
    vector<string> tstCard2 = { "Want","To" };
    vector<string> tstGoal = { "I","Want","To","Drink","Water" };

    cout << solution(tstCard1, tstCard2, tstGoal);
}

string solution(vector<string> _card1, vector<string> _card2, vector<string> _goal)
{
    queue<string> card1 = IntoQueue(_card1);
    queue<string> card2 = IntoQueue(_card2);
    queue<string> goal = IntoQueue(_goal);

    
    while (!goal.empty()) {
        if (!card1.empty() && card1.front() == goal.front()) {
            card1.pop();
            goal.pop();
        }
        else if (!card2.empty()&&card2.front() == goal.front()) {
            card2.pop();
            goal.pop();
        }
        else {
            return "No";
        }
    }
    return "Yes";
}
queue<string> IntoQueue(vector<string> _vec) {
    queue<string> returnQue;
    for (string str : _vec) {
        returnQue.push(str);
    }
    return returnQue;
}

