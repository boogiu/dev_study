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
};

#endif // !_PLAYER_
