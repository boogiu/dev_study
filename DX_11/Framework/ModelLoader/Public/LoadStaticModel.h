#pragma once
#include "Loader_Defines.h"
#include "StaticModel.h"

NS_BEGIN(Loader)
class CLoadStaticModel :
    public CStaticModel
{
private:
	CLoadStaticModel();
	CLoadStaticModel(const CLoadStaticModel& Prototype);
	virtual ~CLoadStaticModel() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype() override;
	HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	virtual void Render_GUI()override;
	HRESULT Load_Model(const aiScene* pAiScene, const string& fileName);
	HRESULT Save_Model();
private:
	HRESULT Release_Mesh();
private:
	string m_fileName  = {};
	ID3D11Device* m_pDevice = { nullptr };

public:
	static CLoadStaticModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

NS_END