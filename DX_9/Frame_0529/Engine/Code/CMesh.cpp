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
	// 환경광(Ambient): 씬 전체에 퍼지는 보라색 계열의 약한 조명
	m_Material.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);  

	// 난반사(Diffuse): 조명에 의해 퍼지는 실제 보라색
	m_Material.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);  

	// 정반사(Specular): 보라색 하이라이트 효과
	m_Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 0.0f); 

	// 자체발광(Emissive): 약간의 보라색 발광 효과 추가
	m_Material.Emissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);

	// 정반사 강도(Power): 하이라이트가 퍼지는 정도 (높을수록 뾰족함)
	m_Material.Power = 52.f;
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
			//	{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
			//{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1
			//{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}		}, // 2
			//{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
			//{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 4
			//{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 5
			//{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 6
			//{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		},	 // 7

			//앞면
			{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
			{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
			{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1
			{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
			{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1
			{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}		}, // 2

			//뒷면
			{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 6
			{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 5
			{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 4
			{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 6
			{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 4
			{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		},	 // 7

			//오른쪽면
			{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,0.f}		}, // 2
			{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 1
			{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
			{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,0.f}		}, // 2
			{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
			{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		},	 // 6

			//왼쪽면
			{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
			{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 4
			{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{1.f,1.f}		}, // 0
			{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
			{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{1.f,1.f}		}, // 0
			{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 3

			//윗면
			{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
			{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 4
			{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
			{{ -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
			{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
			{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1

			//아랫면
			{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
			{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
			{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}	 	}, // 2
			{ {-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
			{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}		}, // 2
			{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		}	 // 6
		};

		m_IndexBuffer = {
			{0,4,5 },{0,5,1}, //윗면
			{7,3,2},{7,2,6 },//아랫면
			{3,0,1 },{3,1,2 }, //앞면
			{6,5,4 },{6,4,7 }, //뒷면
			{2,1,5 },{2,5,6}, //오른쪽
			{7,4,0 },{7,0,3 },//왼쪽
		};
		TextureKey = "crate";
		break;
	case Engine::CMesh::MeshType::SPHERE:
		Generate_Sphere(1, 15, 15);
		break;
	case Engine::CMesh::MeshType::SKYBOX:
		Generate_Sky();
		TextureKey = "SkyBox";
		break;
	default:
		break;
	}
	Compute_Normal();
}


void CMesh::Compute_Normal()
{
	//법선
	for (VTXLIGHTTEX& vtx : m_VtxBuffer) {
		vtx.vNorm = { 0.f,0.f,0.f };
	}

	for (INDEX16 idx : m_IndexBuffer) {
		_vec3 v1 = m_VtxBuffer[idx._0].vPosition;
		_vec3 v2 = m_VtxBuffer[idx._1].vPosition;
		_vec3 v3 = m_VtxBuffer[idx._2].vPosition;

		_vec3 edge1 = v2 - v1;
		_vec3 edge2 = v3 - v1;
		_vec3 crResult;
		D3DXVec3Cross(&crResult, &edge1, &edge2);

		m_VtxBuffer[idx._0].vNorm += crResult;
		m_VtxBuffer[idx._1].vNorm += crResult;
		m_VtxBuffer[idx._2].vNorm += crResult;
	}

	for (VTXLIGHTTEX& vtx : m_VtxBuffer) {
		D3DXVec3Normalize(&vtx.vNorm, &vtx.vNorm);
	}
}

void CMesh::Generate_Sphere(float radius, int stacks, int slices)
{
	m_VtxBuffer.clear();
	m_IndexBuffer.clear();

	// 정점 생성
	for (int i = 0; i <= stacks; ++i)
	{
		// 위도 ( 북~남)
		float theta = D3DX_PI * i / stacks;
		//n개의 위도로 나누고

		for (int j = 0; j <= slices; ++j)
		{
			// 경도 ( 동~서)
			float phi = 2.0f * D3DX_PI * j / slices;
			//k개의 경도로 나눈다.

			//해당 사항은 공식이란다.
			float x = radius * sinf(theta) * cosf(phi);
			float y = radius * cosf(theta);
			float z = radius * sinf(theta) * sinf(phi);

			_vec3 pos = { x, y, z };
			_vec3 norm = pos;
			//구체 메시는 정점의 위치 벡터를 반지름으로 나누면 법선과 같다.
			//왜냐면, 원점 기준으로 같은 거리에 있는 정점들의 집합이기 때문에. 

			D3DXVec3Normalize(&norm, &norm);

			m_VtxBuffer.push_back({ pos, norm, {0.f,0.f} });
		}
	}

	// 인덱스 생성
	for (int i = 0; i < stacks; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int a = i * (slices + 1) + j;
			int b = (i + 1) * (slices + 1) + j;

			m_IndexBuffer.push_back({ (unsigned short)a, (unsigned short)b, (unsigned short)(a + 1) });
			m_IndexBuffer.push_back({ (unsigned short)b, (unsigned short)(b + 1), (unsigned short)(a + 1) });
		}
	}
}

void CMesh::Generate_Sky()
{
	m_bSky = true;
	m_VtxBuffer = {
		//앞면
		{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
		{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
		{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1
		{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
		{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1
		{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}		}, // 2

		//뒷면
		{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 6
		{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 5
		{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 4
		{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 6
		{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 4
		{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		},	 // 7

		//오른쪽면
		{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,0.f}		}, // 2
		{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 1
		{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
		{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,0.f}		}, // 2
		{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
		{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		},	 // 6

		//왼쪽면
		{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
		{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 4
		{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{1.f,1.f}		}, // 0
		{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
		{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{1.f,1.f}		}, // 0
		{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 3

		//윗면
		{ { -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
		{{-1.f,1.f,1.f},				{0.f,0.f,0.f},			{0.f,1.f}		},	 // 4
		{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
		{{ -1.f, 1.f, -1.f },		{ 0.f,0.f,0.f},		{0.f,0.f}		}, // 0
		{{1.f,1.f,1.f},				{0.f,0.f,0.f},			{1.f,1.f}		},	 // 5
		{ { 1.f,  1.f, -1.f},		{0.f,0.f,0.f},			{1.f,0.f}		}, // 1

		//아랫면
		{{-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
		{ {-1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{0.f,1.f}		}, // 3
		{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}	 	}, // 2
		{ {-1.f,-1.f,1.f},			{0.f,0.f,0.f},			{0.f,0.f}		},	 // 7
		{ { 1.f, -1.f, -1.f},		{0.f,0.f,0.f},			{1.f,1.f}		}, // 2
		{ {1.f,-1.f,1.f},			{0.f,0.f,0.f},			{1.f,0.f}		}	 // 6
	};

}

#pragma region 다른 메쉬 생성
//void CMesh::Generate_Cylinder(float radius, float height, int slices, D3DCOLOR color)
//{
//	m_VtxBuffer.clear();
//	m_IndexBuffer.clear();
//
//	for (int i = 0; i <= (int)height; ++i) {
//
//		for (int j = 0; j <= slices; ++j)
//		{
//			// 경도 ( 동~서)
//			float phi = 2.0f * D3DX_PI * j / slices;
//			//k개의 경도로 나눈다.
//
//			//해당 사항은 공식이란다.
//			float x = radius * cosf(phi);
//			float y = i;
//			float z = radius * sinf(phi);
//
//			_vec3 pos = { x, y, z };
//			_vec3 norm = pos;
//			//구체 메시는 정점의 위치 벡터를 반지름으로 나누면 법선과 같다.
//			//왜냐면, 원점 기준으로 같은 거리에 있는 정점들의 집합이기 때문에. 
//
//			D3DXVec3Normalize(&norm, &norm);
//
//			m_VtxBuffer.push_back({ pos, norm, color });
//		}
//	}
//	//뚜껑
//	for (int i = 0; i <= (int)height;i += height) {
//		for (int j = 0; j <= slices; j++) {
//			float phi = 2.0f * D3DX_PI * j / slices;
//
//			float x = radius * cosf(phi);
//			float z = radius * sinf(phi);
//			float y = i;
//			_vec3 pos = { x, y, z };
//			_vec3 norm = pos;
//
//			D3DXVec3Normalize(&norm, &norm);
//			m_VtxBuffer.push_back({ pos, norm, color });
//		}
//	}
//
//	// 인덱스 생성
//	for (int i = 0; i < height; ++i)
//	{
//		for (int j = 0; j < slices; ++j)
//		{
//			int a = i * (slices + 1) + j;
//			int b = (i + 1) * (slices + 1) + j;
//
//			m_IndexBuffer.push_back({ (unsigned short)a, (unsigned short)b, (unsigned short)(a + 1) });
//			m_IndexBuffer.push_back({ (unsigned short)b, (unsigned short)(b + 1), (unsigned short)(a + 1) });
//		}
//	}
//}
//
//void CMesh::Generate_Capsule(float radius, float height, int stacks, int slices, D3DCOLOR color)
//{
//	m_VtxBuffer.clear();
//	m_IndexBuffer.clear();
//
//	int hemiStacks = stacks / 2;
//	float bodyHeight = height - 2 * radius; // 원기둥 길이
//	float yOffset = bodyHeight / 2.f;
//
//	// ===== 상단 반구 =====
//	for (int i = 0; i <= hemiStacks; ++i)
//	{
//		float theta = D3DX_PI / 2.f * i / hemiStacks;  // 0 ~ π/2
//		float y = radius * cosf(theta) + yOffset;
//		float r = radius * sinf(theta);
//
//		for (int j = 0; j <= slices; ++j)
//		{
//			float phi = 2.f * D3DX_PI * j / slices;
//			float x = r * cosf(phi);
//			float z = r * sinf(phi);
//
//			_vec3 pos = { x, y, z };
//			_vec3 norm = pos;
//			D3DXVec3Normalize(&norm, &norm);
//
//			m_VtxBuffer.push_back({ pos, norm, color });
//		}
//	}
//
//	// ===== 원기둥 바디 =====
//	for (int i = 1; i < stacks - 1; ++i)
//	{
//		float y = yOffset - (bodyHeight * i / (stacks - 1));
//
//		for (int j = 0; j <= slices; ++j)
//		{
//			float phi = 2.f * D3DX_PI * j / slices;
//			float x = radius * cosf(phi);
//			float z = radius * sinf(phi);
//
//			_vec3 pos = { x, y, z };
//			_vec3 norm = { x, 0.f, z };
//			D3DXVec3Normalize(&norm, &norm);
//
//			m_VtxBuffer.push_back({ pos, norm, color });
//		}
//	}
//
//	// ===== 하단 반구 =====
//	for (int i = hemiStacks; i <= stacks; ++i)
//	{
//		float theta = D3DX_PI / 2.f * i / hemiStacks;
//		float y = -radius * cosf(theta) - yOffset;
//		float r = radius * sinf(theta);
//
//		for (int j = 0; j <= slices; ++j)
//		{
//			float phi = 2.f * D3DX_PI * j / slices;
//			float x = r * cosf(phi);
//			float z = r * sinf(phi);
//
//			_vec3 pos = { x, y, z };
//			_vec3 norm = pos;
//			D3DXVec3Normalize(&norm, &norm);
//
//			m_VtxBuffer.push_back({ pos, norm, color });
//		}
//	}
//
//	// ===== 인덱스 생성 =====
//	int ringCount = stacks + 1; // 위도 분할 + 양 끝 반구
//	int ringVertexCount = slices + 1;
//
//	for (int i = 0; i < ringCount; ++i)
//	{
//		for (int j = 0; j < slices; ++j)
//		{
//			int curr = i * ringVertexCount + j;
//			int next = (i + 1) * ringVertexCount + j;
//
//			m_IndexBuffer.push_back({ (WORD)curr, (WORD)next, (WORD)(curr + 1) });
//			m_IndexBuffer.push_back({ (WORD)next, (WORD)(next + 1), (WORD)(curr + 1) });
//		}
//	}
//}
//

#pragma endregion

void CMesh::Free()
{
}