#include <iostream>
#include <queue>
using namespace std;
int solution(int n, int k);

int main()
{
    cout<<solution(5, 2);
}
//큐의 특징 FIFO -> 12345 
int solution(int n, int k) {
    queue<int> que;

    for (int i = 1; i <= n; ++i) {
        que.push(i);
    }

    int iCount(0);
    while (1 < que.size()) {
        ++iCount;
        if (k == iCount) {
            que.pop();
            iCount = 0;
        }
        else {
            que.push(que.front());
            que.pop();
        }
    }
    return que.front();
}
