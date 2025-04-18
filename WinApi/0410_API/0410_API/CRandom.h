#pragma once
class CRandom
{
public:
	static int GetRandom(int first, int last);

private:
	static random_device seed;
	static mt19937 Engine;
	
};

