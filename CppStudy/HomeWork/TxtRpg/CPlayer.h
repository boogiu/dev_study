#ifndef _PLAYER_
#define _PLAYER_

class CPlayer
{
public :
	CPlayer();
	~CPlayer();

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	void SetName();
	char* GetName();

	void SetClass(int _class);
	void GetClass();

private :
	char* cName;
};

#endif // !_PLAYER_
