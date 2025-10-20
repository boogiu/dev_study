
#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CBaseField :
	public CGameObject
{

	typedef struct tagInstanceGrassTile
	{
		_float4			vRight, vUp, vLook, vTranslation;
	}INSTANCE_GRASS;

	typedef struct  tagVertexTileGrass {
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[4] = {
			{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,	 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,		48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
	}VTX_TILEGRASS;

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

private:
	vector<INSTANCE_GRASS> m_Grasses;

};

NS_END
