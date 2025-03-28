// Tread_00.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <iostream>
#include <thread>

using std::thread;
using std::cout;
using std::endl;

void function1() {
    for (int i =0 ; i < 100; ++i) {
        cout << "쓰레드 1번 작동 중 : " << i << endl;
    }
}

void function2() {
    for (int i =0; i < 100; ++i) {
        cout << "쓰레드 2번 작동 중 : " << i << endl;
    }
}

void function3() {
    for (int i =0; i < 100; ++i) {
        cout << "쓰레드 3번 작동 중 : " << i << endl;
    }
}

int main()
{
    thread tr1(function1);
    thread tr2(function2);
    thread tr3(function3);

    tr1.join();
    tr2.join();
    tr3.join();
}