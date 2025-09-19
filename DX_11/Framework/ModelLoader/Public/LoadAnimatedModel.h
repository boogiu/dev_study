#pragma once
#include "AnimatedModel.h"
#include "Loader_Defines.h"

NS_BEGIN(Loader)
class CLoadAnimatedModel :
    public CAnimatedModel
{
private:
	CLoadAnimatedModel();
	CLoadAnimatedModel(const CLoadAnimatedModel& Prototype);
	virtual ~CLoadAnimatedModel() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype() override;
	HRESULT Initialize(COMPONENT_DESC* pArg) override;
	CModelData* Get_Data() { return m_pData; }
public:
	HRESULT Load_Model(const aiScene* pAIScene, const string& fileName);
	HRESULT Save_Model();
private:
	HRESULT Release_Mesh();
private:
	string m_fileName = {};
	ID3D11Device* m_pDevice = { nullptr };

public:
	static CLoadAnimatedModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

NS_END