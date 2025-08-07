#include "Engine_Define.h"
#include "CTerrainRenderer.h"
#include "CResourceMgr.h"
#include "CGraphicDev.h"
#include "CTerrain.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CStateCache.h"

CTerrainRenderer::CTerrainRenderer()
	:m_pTex(nullptr)
{
}

CTerrainRenderer::~CTerrainRenderer()
{
}

CTerrainRenderer* CTerrainRenderer::Create()
{
	CTerrainRenderer* instance = new CTerrainRenderer;

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CTerrainRenderer::Ready_Component()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (m_pDevice) {
		m_pDevice->AddRef();
		return S_OK;
	}

	return E_FAIL;
}

void CTerrainRenderer::Update_Component(float& dt)
{
}

void CTerrainRenderer::LateUpdate_Component(float& dt)
{
	if (m_bActive)
		CRenderMgr::GetInstance()->Add_Renderer(this);
}

void CTerrainRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pDevice || !m_pTransform || !m_pMesh) return;

	m_pCache->SetFVF(m_pMesh->GetFVF());

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pDevice->SetStreamSource(0, m_pMesh->GetVertexBuffer(), 0, sizeof(VTXTEX));
	m_pDevice->SetIndices(m_pMesh->GetIndexBuffer());


	m_pDevice->SetTexture(0, m_pTex);

	int vtxNum = m_pMesh->Get_Col() * m_pMesh->Get_Row();
	int numTri = (m_pMesh->Get_Col() - 1) * (m_pMesh->Get_Row() - 1) * 2;

	HRESULT hr;
	hr = m_pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		vtxNum,
		0,
		numTri);

	if (FAILED(hr))
		return;
}

CComponent* CTerrainRenderer::Clone() const
{
	return nullptr;
}

void CTerrainRenderer::Set_Mesh(const wstring& key)
{
	//m_pMesh = dynamic_cast<CTerrain*>(CResourceMgr::GetInstance()->Find_Mesh(key))->Clone();
	m_pMesh = dynamic_cast<CTerrain*>(CResourceMgr::GetInstance()->Find_Mesh(key));
	if (m_pMesh)
		m_pMesh->AddRef();
}

void CTerrainRenderer::Set_Texture(const wstring& key)
{
	m_pTex = CResourceMgr::GetInstance()->Find_Texture(key);
}


void CTerrainRenderer::Set_Terrain(int CellSize, float scale)
{
	if (!m_pMesh) return;
	//가로 세로
	int _numVertsPerRow = m_pMesh->Get_Row();
	int _numVertsPerCol = m_pMesh->Get_Col();

	int startX = -(_numVertsPerCol - 1) / 2;
	int endX = +(_numVertsPerCol - 1) / 2;

	int startZ = -(_numVertsPerRow - 1) / 2;
	int endZ = +(_numVertsPerRow - 1) / 2;

	vector<float>& _heightmap = m_pMesh->GetHeightMap();

	VTXTEX* vtxArr = nullptr;
	auto& _vb = m_pMesh->GetVertexBuffer();

	_vb->Lock(0, 0, (void**)&vtxArr, 0);

	int i = 0;
	for (int z = startZ; z <= endZ; z += 1) //g행
	{
		int j = 0;
		for (int x = startX; x <= endX; x += 1) //열
		{
			int index = i * _numVertsPerRow + j;//0행 1열 등
			VTXTEX vtx;
			//z행 x열
			vtx.vPosition = { (float)x * CellSize, (float)_heightmap[index] * scale, (float)z * CellSize };
			vtx.vTexUV = {
				(float)(z % 2),(float)(x % 2),
			};

			vtxArr[index] = vtx;
			j++; // next column
		}
		i++; // next row
	}

	_vb->Unlock();

	LPDIRECT3DINDEXBUFFER9 pIB = nullptr;

	//면 개수
	int _numCellPerCol = _numVertsPerCol - 1;
	int _numCellPerRow = _numVertsPerRow - 1;
	int _numTriangles = (_numCellPerCol) * (_numCellPerRow) * 2;

	HRESULT hr = m_pDevice->CreateIndexBuffer(
		_numTriangles * 3 * sizeof(DWORD), // 3 indices per triangle
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&pIB,
		0);

	if (FAILED(hr))
		return;

	DWORD* indices = 0;
	pIB->Lock(0, 0, (void**)&indices, 0);
	int baseIndex = 0;

	for (int i = 0; i < _numCellPerCol; i++)
	{
		for (int j = 0; j < _numCellPerRow; j++)
		{
			//왼쪽위(0)
			indices[baseIndex] = (i + 1) * _numVertsPerRow + j;
			//오른쪽위(1)
			indices[baseIndex + 1] = (i + 1) * _numVertsPerRow + (j + 1);
			//왼쪽아래(3)
			indices[baseIndex + 2] = (i)*_numVertsPerRow + j;

			//왼쪽 아래(3)
			indices[baseIndex + 3] = (i)*_numVertsPerRow + j;
			//오른쪽 위(1)
			indices[baseIndex + 4] = (i + 1) * _numVertsPerRow + (j + 1);
			//오른쪽 아래(2)
			indices[baseIndex + 5] = (i)*_numVertsPerRow + (j + 1);
			// 다음 셀
			baseIndex += 6;
		}
	}
	pIB->Unlock();
	m_pMesh->SetIDX_Buffer(pIB);
	pIB->Release();
}

void CTerrainRenderer::Free()
{
	m_pDevice->Release();

	Safe_Release(m_pMesh);
	Safe_Release(m_pTransform);
}
