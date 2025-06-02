#pragma once
#include "CComponent.h"

BEGIN(Engine)
class ENGINE_DLL CMesh :
	public CComponent
{
public:
	enum class MeshType {
		CUBE,
	};
public:
	explicit CMesh();
	virtual  ~CMesh() override;

public:
	static CMesh* Create();

public:
	HRESULT Ready_Component() override;
	void Update_Component(float dt) override;
	void LateUpdate_Component(float dt) override;
	CComponent* Clone() const override;

public:
	static COM_TYPE Get_StaticType() { return COM_TYPE::MESH; }
	COM_TYPE Get_Type() override { return Get_StaticType(); };
	void Set_MeshType(MeshType type);
	const vector<VTXCOL>& Get_VertexBuffer() const { return m_VtxBuffer; }
	const vector<INDEX16>& Get_IndexBuffer() const { return m_IndexBuffer; }

private:
	virtual void Free() override;

private:
	vector<VTXCOL> m_VtxBuffer;
	vector<INDEX16> m_IndexBuffer;
};

END