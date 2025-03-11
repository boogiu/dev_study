//,,,

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

vector<int> solution(vector<int> progress, vector<int> speeds);

int main()
{
    vector<int> tstProgress = { 95,90,99,99,80,99 };
    vector<int> tstSpeeds = { 1,1,1,1,1,1 };

    solution(tstProgress, tstSpeeds);
}

vector<int> solution(vector<int> progress, vector<int> speeds) {
    
    vector<int> vRestTime;
    vector<int> vAnswer;

    int iCount(0);
    int iMaxWork(0);

    for (int i = 0; i < progress.size(); ++i) {
        int iRestTime = (int)((100 - progress[i]) / speeds[i]);
        vRestTime.push_back(iRestTime);
    }
    iMaxWork = vRestTime[0];

    for (int i : vRestTime) {
        if (i <= iMaxWork) {
            iCount += 1;
        }
        else {
            vAnswer.push_back(iCount);
            iCount = 1;
            iMaxWork = i;
        }
    }
    vAnswer.push_back(iCount);

    return vAnswer;
}
