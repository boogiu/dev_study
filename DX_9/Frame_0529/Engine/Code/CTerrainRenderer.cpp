#include "Engine_Define.h"
#include "CTerrainRenderer.h"
#include "CResourceMgr.h"
#include "CGraphicDev.h"
#include "CTerrain.h"
#include "CGameObject.h"
#include "CTransform.h"

CTerrainRenderer::CTerrainRenderer()
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

void CTerrainRenderer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pDevice || !m_pTransform || !m_pMesh) return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pDevice->SetStreamSource(0, m_pMesh->GetVertexBuffer(), 0, sizeof(VTXTEX));
	m_pDevice->SetFVF(m_pMesh->GetFVF());
	m_pDevice->SetIndices(m_pMesh->GetIndexBuffer());

	LPDIRECT3DTEXTURE9 _tex =	CResourceMgr::GetInstance()->Find_Texture("grass.bmp");

	if (_tex)
		m_pDevice->SetTexture(0, _tex);
	else
		m_pDevice->SetTexture(0, nullptr);


	int vtxNum = m_pMesh->Get_Col() * m_pMesh->Get_Row();
	int numTri = (m_pMesh->Get_Col() - 1) * (m_pMesh->Get_Row()-1)*2;

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
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

void CTerrainRenderer::Set_Mesh(const string& key)
{
	m_pMesh = dynamic_cast<CTerrain*>(CResourceMgr::GetInstance()->Find_Mesh(key))->Clone();
}


void CTerrainRenderer::Set_Terrain(int xSize, int zSize, int CellSize, float scale)
{
	if (!m_pMesh) return;

	int startX = -xSize / 2;
	int endX = +xSize / 2;

	int startZ = +zSize / 2;
	int endZ = -zSize / 2;
	//가로 세로
	int _numVertsPerRow = m_pMesh->Get_Row(); 
	int _numVertsPerCol = m_pMesh->Get_Col();

	vector<int>& _heightmap = m_pMesh->GetHeightMap();

	//버텍스 개수 비율, 즉 UV좌표에서 몇분의 몇만큼 가져갈건가?
	float uCoordIncrementSize = 1.0f / (float)_numVertsPerRow; //버텍스 개수
	float vCoordIncrementSize = 1.0f / (float)_numVertsPerCol;

	VTXTEX* vtxArr = nullptr;
	auto& _vb = m_pMesh->GetVertexBuffer();

	_vb->Lock(0, 0, (void**)&vtxArr, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= CellSize)
	{
		int j = 0;
		for (int x = startX; x <= endX; x += CellSize)
		{
			int index = i * _numVertsPerRow + j;
			VTXTEX vtx;
			//z행 x열
			vtx.vPosition = { (float)x, (float)_heightmap[index] * scale, (float)z };
			vtx.vTexUV = { (float)j * uCoordIncrementSize, (float)i * vCoordIncrementSize };

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
			indices[baseIndex] = i * _numVertsPerRow + j;
			//오른쪽위(1)
			indices[baseIndex + 1] = i * _numVertsPerRow + (j + 1);
			//왼쪽아래(3)
			indices[baseIndex + 2] = (i + 1) * _numVertsPerRow + j;

			//왼쪽 아래(3)
			indices[baseIndex + 3] = (i + 1) * _numVertsPerRow + j;
			//오른쪽 위(1)
			indices[baseIndex + 4] = i * _numVertsPerRow + (j + 1);
			//오른쪽 아래(2)
			indices[baseIndex + 5] = (i + 1) * _numVertsPerRow + (j + 1);
			// 다음 셀
			baseIndex += 6;
		}
	}

	pIB->Unlock();

	m_pMesh->SetIDX_Buffer(pIB);
	pIB->Release();


	//----------------------------------//
	hr = 0;
	LPDIRECT3DTEXTURE9 _tex = CResourceMgr::GetInstance()->Find_Texture("grass.bmp");

	D3DSURFACE_DESC textureDesc;
	_tex->GetLevelDesc(0 /*level*/, &textureDesc);

	// make sure we got the requested format because our code that fills the
	// texture is hard coded to a 32 bit pixel depth.
	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return;

	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(
		0,          // lock top surface level in mipmap chain
		&lockedRect,// pointer to receive locked data
		0,          // lock entire texture image
		0);         // no lock flags specified

	DWORD* imageData = (DWORD*)lockedRect.pBits;

	for (int i = 0; i < textureDesc.Height; i++)
	{
		for (int j = 0; j < textureDesc.Width; j++)
		{
			// index into texture, note we use the pitch and divide by 
			// four since the pitch is given in bytes and there are 
			// 4 bytes per DWORD.
			int index = i * lockedRect.Pitch / 4 + j;

			// get current color of quad
			D3DXCOLOR c(imageData[index]);

			// shade current quad
			//c *= computeShade(i, j, directionToLight);;

			// save shaded color
			imageData[index] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

}

void CTerrainRenderer::Free()
{
	m_pDevice->Release();

	Safe_Release(m_pMesh);
	Safe_Release(m_pTransform);
}
