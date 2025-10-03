#pragma once
#include "Material.h"

NS_BEGIN(MapEditor)
class CMapMaterial final: public CMaterial
{
private:
	CMapMaterial();
	CMapMaterial(const CMapMaterial& rhs);
	~CMapMaterial() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	virtual void Render_GUI() override;

public:
	static CMapMaterial* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

NS_END