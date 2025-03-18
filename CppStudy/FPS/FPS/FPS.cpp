// FPS.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// frame per Second를 직접 구현해보는 연습
#include <iostream>
#include <windows.h>
#include "FPS.h"

using namespace std;

int main() {
    LARGE_INTEGER GAP, counter, currTime;
    LARGE_INTEGER prevTime;

    QueryPerformanceCounter(&prevTime);  // 프로그램 시작 당시 타이머 값 얻기
    QueryPerformanceFrequency(&counter); //컴퓨터 진동수

    double secondpercount=(double) 1.0 / counter.QuadPart; //프레임 1회에 걸리는 초(1 진동에 몇초?)

    double elapsedTime = 0.0; // 시간 측정 시작!
    int frameCount = 0; //프레임 몇개인지 수집!
    while (true) {

        QueryPerformanceCounter(&currTime);  // 현재 타이머 값 얻기

        //cout << "지난 타이머 값: " << prevTime.QuadPart << endl;
        //cout << "이번 타이머 값: " << currTime.QuadPart << endl;

        GAP.QuadPart =currTime.QuadPart -prevTime.QuadPart;
        //cout << "진동수 갭 :: " << GAP.QuadPart << endl;
        
        //cout << "프레임 1회당 : " << GAP.QuadPart * secondpercount<<"초" << endl;
        //GAP.QuadPart* secondpercount 프레임 1번 돌 때 걸리는 시간
        elapsedTime += GAP.QuadPart * secondpercount; //시간 누적
        
        //cout << "누적 시간 : " << elapsedTime << "초" << endl;

        if (elapsedTime >= 1.0) {
            Highlight(frameCount);
            elapsedTime = 0;
            frameCount = 0;
        }
        else {
            ++frameCount;
        }
        //cout << "초당 프레임 : " << frameCount << endl;
        prevTime = currTime;
    }

    return 0;
}

void Highlight(int i) {
    cout << i << endl;
}
