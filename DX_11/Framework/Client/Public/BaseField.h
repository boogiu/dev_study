
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
	void Load_Tile(INSTANCE_TILE tile);

private:
	HRESULT CreateTexture2DArrayFromFiles(vector<string> TexturePath, vector<string> TextureKey, ID3D11ShaderResourceView** pSrv);
	void Override_Pass();
	virtual void Render_GUI();
	HRESULT ReadyTexture2DArray();
private:
	vector<INSTANCE_TILE> m_Tiles;
	ID3D11ShaderResourceView* m_pTextureArrayDiffuse = { nullptr };
	ID3D11ShaderResourceView* m_pTextureArrayPalette = { nullptr };

public:
	static CBaseField* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;


};

NS_END
