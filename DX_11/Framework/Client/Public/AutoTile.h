#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CAutoTile :
    public CGameObject
{

public:
	typedef struct TILE_TYPE_DESC : public GAMEOBJECT_DESC {
		string TypeName = {};
		TILE_INDEX index = {};
	};
private:
	CAutoTile();
	CAutoTile(const CAutoTile& rhs);
	virtual ~CAutoTile() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt) override;
	void Update(_float dt) override;
	void Late_Update(_float dt) override;
	
public:
	virtual void Render_GUI();
	HRESULT Link_Data(const string& folderName);
	HRESULT Read_TileRule();

private:
	void Update_State(_uint N_State);

private:
	string ModelName = {};
	string MaterialName = {};
	string m_BaseTypeName = {};
	_float m_fRotation = {};
	_uint m_CurState = {};

public:

public:
	static CAutoTile* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END