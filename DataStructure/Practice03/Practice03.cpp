//주식 가격

#include <iostream>
#include <stack>
#include <vector>
using namespace std;
int CheckPrice(vector<int> vPriceArr, int start);
vector<int> solution(vector<int> vPriceArr);

int main()
{
    vector<int> test = { 1,2,3,2,3 };
    solution(test);
}

vector<int> solution(vector<int> vPriceArr) {
    vector<int> vAnswer;

    for (int i = 0; i < vPriceArr.size(); ++i) {
        vAnswer.push_back(CheckPrice(vPriceArr, i));
    }

    return vAnswer;
 }

int CheckPrice(vector<int> vPriceArr, int start) {
    int stdPrice = vPriceArr[start];
    int iPeriod(0);

    for (;start+1 < vPriceArr.size(); ++start) {
        if (stdPrice > vPriceArr[start]) {
            return iPeriod;
        }
        else {
            ++iPeriod;
        }
    }
    return iPeriod;
}