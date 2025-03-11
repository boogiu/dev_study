// 각자 기준으로 비교하게 되면, (즉, 1번과 2,3,4,5 / 2번과 3,4,5, 시간 복잡도가 O(n^2)임.
//그래서 다시 해보는 것으로.

#include <iostream>
#include <vector>
#include <stack>
using namespace std;
vector<int> solution(vector<int> vec);

int main()
{
    vector<int> test = { 1,2,3,2,3 };
    solution(test);
}

vector<int> solution(vector<int> vec) {
    int priceSize(vec.size());
    vector<int> answer(priceSize); //우선 벡터와 같은 크기의 답안지 크기 설정.
    stack<int> stack; //인덱스를 기억할 스택
    
    for (int i = 0; i < priceSize; ++i) {
        while (!stack.empty() && (vec[stack.top()] > vec[i])) {
            //현재 인덱스의 값이 이전 인덱스의 값보다 작을 때까지 비교.

            answer[stack.top()] = i - stack.top();
            //만약 현재 인덱스의 값이 이전 인덱스값보다 작다면, 이전인덱스 - 현재 인덱스는 거리이기 때문에. 
            //이전 인덱스 위치에 그 거리값을 넣어줌. -> 거리 확정된 사항.
            stack.pop(); //그리고 top을 지워주고 다시 비교.
        }
        //현재 인덱스보다 큰 값이 없어졌다면, 즉 while을 나왔다면.
        stack.push(i); //현재 인덱스 넣어주기.
    }
    while (!stack.empty()) { //남은 인덱스들은 계속 오르는 것.
        answer[stack.top()] = priceSize - stack.top() - 1;
        stack.pop();
    }

    return answer;

}