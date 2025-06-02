#include "Engine_Define.h"
#include "CMesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
}

CMesh* CMesh::Create()
{
	CMesh* instance = new CMesh;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		return nullptr;
	}

	return instance;
}

HRESULT CMesh::Ready_Component()
{
	return S_OK;
}

void CMesh::Update_Component(float dt)
{

}

void CMesh::LateUpdate_Component(float dt)
{
}


CComponent* CMesh::Clone() const
{
	return nullptr;
}

void CMesh::Set_MeshType(MeshType type)
{
	switch (type)
	{
	case Engine::CMesh::MeshType::CUBE:
		m_VtxBuffer = {
			//¾Õ¸é
			{{-1.f,1.f,-1.f},D3DCOLOR_RGBA(255,0,255,255)},
			{{1.f,1.f,-1.f},D3DCOLOR_RGBA(255,0,255,255)},
			{{1.f,-1.f,-1.f},D3DCOLOR_RGBA(0,255,0,255)},
			{{-1.f,-1.f,-1.f},D3DCOLOR_RGBA(0,255,0,255)},
			//µÞ¸é
			{{-1.f,1.f,1.f},D3DCOLOR_RGBA(255,255,255,255)},
			{{1.f,1.f,1.f},D3DCOLOR_RGBA(255,255,255,255)},
			{{1.f,-1.f,1.f},D3DCOLOR_RGBA(255,0,255,255)},
			{{-1.f,-1.f,1.f},D3DCOLOR_RGBA(255,0,255,255)},
		};

		m_IndexBuffer = {
			{0,4,5 },{0,5,1}, //À­¸é
			{7,3,2},{7,2,6 },//¾Æ·§¸é
			{3,0,1 },{3,1,2 }, //¾Õ¸é
			{6,5,4 },{6,4,7 }, //µÞ¸é
			{2,1,5 },{2,5,6}, //¿À¸¥ÂÊ
			{7,4,0 },{7,0,3 },
		};

		break;
	default:
		break;
	}
}

void CMesh::Free()
{
}