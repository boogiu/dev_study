#pragma once
#include "SkeletalModel.h"

NS_BEGIN(Loader)
class CLoadSkeletalModel :
	public CSkeletalModel
{
private:
	CLoadSkeletalModel();
	CLoadSkeletalModel(const CLoadSkeletalModel& Prototype);
	virtual ~CLoadSkeletalModel() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	HRESULT Load_Model(const aiScene* pAIScene, const string& fileName);
	HRESULT Save_Model();
	HRESULT Save_Model(const string& SavePath);

public:
	void Render_GUI() override;
private:
	HRESULT Release_Mesh();
private:
	string m_fileName = {};
	ID3D11Device* m_pDevice = { nullptr };

public:
	static CLoadSkeletalModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

NS_END