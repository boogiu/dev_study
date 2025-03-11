// Hash03.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
string Hash03(vector<string> _particpants, vector<string> completion);

int main()
{
    vector<string> tst_par = {"Jay","Jay" ,"May", "Day","Day","May"};
    vector<string> tst_com = { "Jay" ,"Jay" ,"May", "Day","Day" };

    cout<<    Hash03(tst_par, tst_com);
}

string Hash03(vector<string> _particpants, vector<string> completion) {
    unordered_map<string,int> marathoner;
    string answer("없음");

    //완주자 채워넣음
    for (string str : completion) {
        if (marathoner.find(str) == marathoner.end()) {
            marathoner.insert({ str,0 });
        }
        else {
            //marathoner.insert({ str,1 }); 
            marathoner[str] += 1;//동명이인은 1
        }
    }
    
    //참여자 순회
    while (!marathoner.empty()) {
        for (string str : _particpants) {
            if (marathoner.find(str) != marathoner.end())
            {
                if (marathoner[str] == 0) { //찾았고 값이 0이라면.
                    marathoner.erase(str);
                }
                else {
                    marathoner[str] -= 1;
                }
            }
            else {
                answer= str;
            }
        }
    }

    return answer;
}
