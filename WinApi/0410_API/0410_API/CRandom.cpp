#include "pch.h"
#include "CRandom.h"

random_device CRandom::seed;
mt19937 CRandom::Engine(seed()); // static 멤버 초기화

int CRandom::GetRandom(int first, int last)
{
    uniform_int_distribution<int> dis(first, last);
    return dis(Engine);
}
