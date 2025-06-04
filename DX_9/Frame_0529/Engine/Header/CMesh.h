#pragma once
#include "CComponent.h"

BEGIN(Engine)
class ENGINE_DLL CMesh :
	public CComponent
{
public:
	enum class MeshType {
		CUBE,
		SPHERE,
		CYLINDER,
		CAPSULE,
		SKYBOX
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
	const vector<VTXLIGHTTEX>& Get_VertexBuffer() const { return m_VtxBuffer; }
	const vector<INDEX16>& Get_IndexBuffer() const { return m_IndexBuffer; }
	const D3DMATERIAL9& Get_Material() const { return m_Material; }
	const string& Get_Key() { return TextureKey; }
	bool isSky() { return m_bSky; }
private:
	void Compute_Normal();
	void Generate_Sphere(float radius, int stacks, int slices);
	void Generate_Sky();
	//void Generate_Cylinder(float radius, float height,int slices, D3DCOLOR color);
	//void Generate_Capsule(float radius, float height, int stacks, int slices, D3DCOLOR color);
private:
	virtual void Free() override;

private:
	bool m_bSky = false;
	string TextureKey;
	D3DMATERIAL9 m_Material;
	vector<VTXLIGHTTEX> m_VtxBuffer;
	vector<INDEX16> m_IndexBuffer;
};

END