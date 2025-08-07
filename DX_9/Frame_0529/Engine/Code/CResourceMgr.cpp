#include"Engine_Define.h"
#include "CResourceMgr.h"
#include "CGraphicDev.h"

#include "IMesh.h"
#include "CMesh.h"
#include "CCubeMesh.h"
#include "CMaterial.h"
#include "CTerrain.h"
#include "CFont.h"
#include "CThreadMgr.h"

#include <fstream>
#include <io.h>

IMPLEMENT_SINGLETON(CResourceMgr)

CResourceMgr::CResourceMgr()
{
	}

CResourceMgr::~CResourceMgr()
{
	Free();
}

HRESULT CResourceMgr::Ready_Resoource()
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();
	if (!m_pDevice)
		return E_FAIL;

	m_pDevice->AddRef();

	if (FAILED(Load_All())) {
		return E_FAIL;
	}

}

IMesh* CResourceMgr::Find_Mesh(const wstring& key)
{
	auto iter = find_if(m_MeshContainer.begin(), m_MeshContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_MeshContainer.end())
		return nullptr;
	else
		return iter->second;

}

CMaterial* CResourceMgr::Find_Material(const wstring& key)
{
	auto iter = find_if(m_MatContainer.begin(), m_MatContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_MatContainer.end())
		return nullptr;
	else
		return iter->second;
}

const LPDIRECT3DTEXTURE9& CResourceMgr::Find_Texture(const wstring& key)
{
	auto iter = find_if(m_TexContainer.begin(), m_TexContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_TexContainer.end())
		return nullptr;
	else
		return iter->second;
}

CFont* CResourceMgr::Find_Font(const wstring& key)
{
	auto iter = find_if(m_FontContainer.begin(), m_FontContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_FontContainer.end())
		return nullptr;
	else
		return iter->second;
}

HRESULT CResourceMgr::Load_All()
{
	LoadAllTexture();
	LoadDeafultMesh();
	LoadStaticMeshByX(L"tiger");
	LoadStaticMeshByX(L"MirrorCube");

	LoadTerrainMeshByRaw(L"coastMountain64", 64, 64);
	LoadTerrainMeshByBMP(L"Height1", 129, 129);
	LoadTerrainMeshByRaw(L"terrain", 512, 512);

	LoadFonts(L"궁서");

	return S_OK;
}

void CResourceMgr::LoadDeafultMesh()
{
	CCubeMesh* mesh = CCubeMesh::Create();
	mesh->SetKey(L"Deafult_Cube");
	m_MeshContainer.insert({ L"Deafult_Cube" ,mesh });
}

void CResourceMgr::LoadStaticMeshByX(const wstring& key)
{
	const wstring xFilePath = L"../Bin/Resource/xFiles/" + key + L".x";

	// 인접 버퍼, 재질 버퍼, 재질 개수
	ID3DXBuffer* adjBuffer = nullptr;
	ID3DXBuffer* mtrlBuffer = nullptr;
	DWORD        numMtrls = 0;

	LPD3DXMESH newMesh = nullptr;

	HRESULT hr = D3DXLoadMeshFromXW(
		xFilePath.c_str(),
		D3DXMESH_MANAGED,
		m_pDevice,
		&adjBuffer,
		&mtrlBuffer,
		nullptr,
		&numMtrls,
		&newMesh);

	if (FAILED(hr))
	{
		wstring Err = L"D3DXLoadMeshFromX() - FAILED: " + key;
		::MessageBoxW(nullptr, Err.c_str(), L"Error", MB_OK);
		return;
	}

	// 메시 클래스 생성 및 등록
	CMesh* meshClass = CMesh::Create();
	if (!meshClass) return;
	meshClass->SetMesh(newMesh);
	meshClass->SetKey(key);
	m_MeshContainer.insert({ key, meshClass });

	// 머티리얼 + 텍스처 처리
	if (mtrlBuffer && numMtrls > 0)
	{
		D3DXMATERIAL* mtrls = reinterpret_cast<D3DXMATERIAL*>(mtrlBuffer->GetBufferPointer());
		CMaterial* material = CMaterial::Create();
		material->SetSize(numMtrls);
		material->SetKey(key);
		vector<MATTEX>& matList = material->Get_Material();

		for (DWORD i = 0; i < numMtrls; ++i)
		{
			MATTEX tmp;
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			tmp.material = mtrls[i].MatD3D;

			if (mtrls[i].pTextureFilename)
			{
				 //ANSI 문자열을 유니코드 문자열로 변환
				string ansiTexName(mtrls[i].pTextureFilename);
				wstring wTexName(ansiTexName.begin(), ansiTexName.end());

				tmp.texture = Find_Texture(wTexName);
				if (tmp.texture)
					tmp.texture->AddRef();
			}
			else
			{
				tmp.texture = nullptr;
			}

			matList.push_back(tmp);
		}

		m_MatContainer.insert({ key, material });
	}

	Safe_Release(newMesh);
	Safe_Release(mtrlBuffer);
	Safe_Release(adjBuffer);
}

void CResourceMgr::LoadTerrainMeshByRaw(const wstring& key, int x, int z)
{
	int vertexNum= x * z; //사진 상의 가로/세로 사이즈

	std::vector<BYTE> in(vertexNum); //우선 바이트 단위로 읽기

	wstring fileName = L"../Bin/Resource/Terrain/" + key + L".raw";

	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);

	if (!inFile.is_open()) {
		::MessageBoxW(0, L"LoadTerrainMeshByRaw() - FAILED", L"Error", MB_OK);
		return;
	}

	inFile.read(
		(char*)&in[0], // buffer에 채워넣기
		in.size());// number of bytes to read into buffer

	inFile.close();

	CTerrain* instance =  CTerrain::Create();

	instance->SetKey(key);
	instance->SetFVF(FVF_TEX);
	instance->SetVTX_Num(x, z);

	auto & _heightmap = instance->GetHeightMap();
	_heightmap.resize(vertexNum);

	for (int i = 0; i < in.size(); i++)
		_heightmap[i] =(float) in[i];

	//버텍스 버퍼만 생성
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	 HRESULT hr = m_pDevice->CreateVertexBuffer(
		vertexNum * sizeof(VTXTEX),
		D3DUSAGE_WRITEONLY,
		FVF_TEX,
		D3DPOOL_MANAGED,
		&pVB,
		0);

	if (FAILED(hr)) {
		::MessageBoxW(0, L"LoadTerrainMeshByRaw() -buffer Create FAILED", L"Error", MB_OK);
		Safe_Release(instance);
		return;
	}

	instance->SetVTX_Buffer(pVB);
	m_MeshContainer.insert({ key, instance });
	//pVB->Release();
}

void CResourceMgr::LoadTerrainMeshByBMP(const wstring& key, int x, int z)
{
	int vertexNum = x * z;
	wstring fileName = L"../Bin/Resource/Terrain/" + key + L".bmp";

	HANDLE m_hFile = CreateFileW(fileName.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (INVALID_HANDLE_VALUE == m_hFile) {
		::MessageBoxW(0, L"CreateTerrain_BMP() - FAILED", L"Error", MB_OK);
		return;
	}

	_ulong	dwByte(0);

	BITMAPFILEHEADER m_fH;
	BITMAPINFOHEADER m_iH;

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	int width = x > m_iH.biWidth? m_iH.biWidth : x;
	int height=  z > m_iH.biHeight ? m_iH.biHeight : z;

	_ulong m_dwTriCnt = (x - 1) * (z - 1) * 2;
	_ulong m_dwVtxCnt = x * z;

	vector<float> height_map;
	height_map.reserve(width * height); // 최적화

	_ulong* pPixel = new _ulong[width * height];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * width * height, &dwByte, NULL);
	CloseHandle(m_hFile);

	for (_ulong i = 0; i < z; ++i)
	{
		for (_ulong j = 0; j < x; ++j)
		{
			_ulong pixel = pPixel[i * x + j];
			BYTE R = (pixel >> 16) & 0xFF;
			BYTE G = (pixel >> 8) & 0xFF;
			BYTE B = (pixel >> 0) & 0xFF;
			float gray = 0.299f * R + 0.587f * G + 0.114f * B;

			height_map.push_back(gray);
		}
	}

	Safe_Delete_Array(pPixel);

	// Terrain 객체 생성
	CTerrain* instance = CTerrain::Create();
	instance->SetKey(key);
	instance->SetFVF(FVF_TEX);
	instance->SetVTX_Num(x, z);
	instance->GetHeightMap() = height_map;

	// 버텍스 버퍼 생성
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	HRESULT hr = m_pDevice->CreateVertexBuffer(
		vertexNum * sizeof(VTXTEX),
		D3DUSAGE_WRITEONLY,
		FVF_TEX,
		D3DPOOL_MANAGED,
		&pVB,
		0);

	if (FAILED(hr)) {
		::MessageBoxW(0, L"LoadTerrainMeshByBMP() -buffer Create FAILED", L"Error", MB_OK);
		Safe_Release(instance);
		return;
	}

	instance->SetVTX_Buffer(pVB);
	m_MeshContainer.insert({ key, instance });
}


void CResourceMgr::LoadAllTexture()
{
	_finddata_t fd;

	const string texBasePath = "../Bin/Resource/Textures/*.*";
	intptr_t handle = _findfirst(texBasePath.c_str(), &fd); // _findfirst 사용

	if (handle == -1)
	{
		::MessageBoxW(0, L"LoadAllTexture() - FAILED", L"Error", MB_OK);
		return;
	}

	do
	{
		if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
			continue;

		LPDIRECT3DTEXTURE9 _texture;
		wstring fileName = L"../Bin/Resource/Textures/" + wstring(fd.name, fd.name + strlen(fd.name));

		if (FAILED(D3DXCreateTextureFromFileW(m_pDevice, fileName.c_str(), &_texture)))
		{
			::MessageBoxW(0, L"CreateTexture() - FAILED", L"Error", MB_OK);
			_texture->Release();
			return;
		}

		wstring rawName = wstring(fd.name, fd.name + strlen(fd.name));
		m_TexContainer.insert({ rawName , _texture });

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
	return;
}

void CResourceMgr::LoadFonts(const wstring& key)
{
	if (Find_Font(key)) return;

	CFont* instance = CFont::Create(
		key.c_str(), 5, 15, FW_HEAVY
	);
	m_FontContainer.insert({ key ,instance });
}


void CResourceMgr::Free()
{
	for_each(m_MeshContainer.begin(), m_MeshContainer.end(),
		[](auto& pair) {
			Safe_Release(pair.second);
		});
	m_MeshContainer.clear();

	for_each(m_MatContainer.begin(), m_MatContainer.end(),
		[](auto& pair) {
			Safe_Release(pair.second);
		});
	m_MatContainer.clear();

	for (auto& pair : m_TexContainer)
	{
		pair.second->Release();
	}
	m_TexContainer.clear();

	for_each(m_FontContainer.begin(), m_FontContainer.end(),
		[](auto& pair) {
			Safe_Release(pair.second);
		});
	m_FontContainer.clear();

	m_pDevice->Release();
}
