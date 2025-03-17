#ifndef _DEFINE_
#define _DEFINE_

#define SAFE_DELETE(p) {if(p){ delete p; p =nullptr;}}

enum SCENE
{
	START,
	SELECT,
	VILLAGE,
	SHOP,
	MOTEL,
	BATTLE,
	ENDSCENE
};
enum MONSTERS
{
	GOBLE,
	MOBLE,
	NOBLE,
	BOBLE,
	ENDMONSTER
};

//½Ì±ÛÅæ µðÆÄÀÎ
#define SINGLETON(M)\
private:\
	M();\
	~M();\
public:\
	static M& GetInstance() {\
		static M instance;\
		return instance;\
	}\
	M(const M&) = delete;\
	M& operator=(M&) = delete;\

#endif // !_DEFINE_



