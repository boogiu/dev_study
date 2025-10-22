
#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CBaseField :
	public CGameObject
{
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
	void Load_BaseTile(ifstream& ifs, _uint Count);

public:
	//void Change_Tile();

private:
	void Add_Tile(INSTANCE_TILE tile);
	virtual void Render_GUI();

private:
	vector<INSTANCE_TILE> m_Tiles;

public:
	static CBaseField* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

NS_END

struct TileBatch_Notice_Info {
	_uint vectorIndex = {};
	TILE_INDEX index = {};
};