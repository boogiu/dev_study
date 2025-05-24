#pragma once
#include <random>
// CFunction.h

class CFunction
{
public:
    // 정수 범위 랜덤
    static int GetInt(int min, int max)
    {
        static std::mt19937 rng(Seed());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    // 실수 범위 랜덤
    static float GetFloat(float min, float max)
    {
        static std::mt19937 rng(Seed());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
    static bool LoadExternalFont(const std::wstring& fontPath);
    static HFONT CreateFontW(const std::wstring& fontName, int size, bool bold = false, bool italic = false);

private:
    static unsigned int Seed()
    {
        static std::random_device rd;
        return rd(); // 매 실행마다 다른 seed 반환
    }
};
