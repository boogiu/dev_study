// FPS.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// frame per Second를 직접 구현해보는 연습
#include <iostream>
#include <windows.h>
#include "FPS.h"

using namespace std;


void Highlight(int i) {
    cout << "초당 프레임 : " << i << endl;
}

int main() {
    LARGE_INTEGER Gap, Frequency, CurrTime;
    LARGE_INTEGER PrevTime;
    
    QueryPerformanceFrequency(&Frequency); //컴퓨터 진동수 (고정값)
    double SecondPerFrequency = (double)1.0 / Frequency.QuadPart; //프레임 1회에 걸리는 초(1 진동에 몇초?)

    QueryPerformanceCounter(&PrevTime);  // 프로그램 시작 당시 타이머 값 얻기

    double elapsedTime = 0.0; // 시간 측정 시작!
    int frameCount = 0; //프레임 몇개인지 수집!

    while (true) {
        QueryPerformanceCounter(&CurrTime);  // 현재 타이머 값 얻기

        //cout << "지난 타이머 값: " << PrevTime.QuadPart << endl;
        //cout << "이번 타이머 값: " << CurrTime.QuadPart << endl;

        Gap.QuadPart = CurrTime.QuadPart -PrevTime.QuadPart;
        //cout << "진동수 갭 :: " << Gap.QuadPart << " 차이" << endl;
        
        //cout << "프레임 1회당 : " << Gap.QuadPart * SecondPerFrequency <<"초" << endl;
        //GAP.QuadPart* secondpercount 프레임 1번 돌 때 걸리는 시간
        elapsedTime += Gap.QuadPart * SecondPerFrequency; //시간 누적
        
        //cout << "누적 시간 : " << elapsedTime << "초" << endl;
        ++frameCount;

        if (elapsedTime >= 1.0) {
            system("cls");
            Highlight(frameCount);
            elapsedTime = 0;
            frameCount = 0;
        }
        PrevTime = CurrTime;
    }

    return 0;
}