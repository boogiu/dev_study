// JJ.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

class jj {
    jj() {
        int* ptr = new int(0);
    };
};

int main()
{
    for(int i = 0; i < 300000000000; ++i) {
        jj* mykk = new jj();
        std::cout << i;
    };
};

