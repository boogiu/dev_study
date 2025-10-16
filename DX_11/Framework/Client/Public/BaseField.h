
#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CBaseField :
	public CGameObject
{
public:
	typedef struct tagBaseFieldDesc : public GAMEOBJECT_DESC
	{
		string LevelTag;
		string ModelName;
		string MaterialName;

		tagBaseFieldDesc() DEFAULT;
		virtual ~tagBaseFieldDesc() DEFAULT;
	}BASEFIELD_DESC;

private:
	CBaseField();
	CBaseField(const CBaseField& rhs);
	virtual ~CBaseField() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt) override;
	void Update(_float dt) override;
	void Late_Update(_float dt) override;
	virtual void Render_GUI();
private:
	void Override_Pass();

public:
	static CBaseField* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;

};

NS_END
