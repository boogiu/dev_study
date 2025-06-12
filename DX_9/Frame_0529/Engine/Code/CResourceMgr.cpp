#include"Engine_Define.h"
#include "CResourceMgr.h"
#include "CGraphicDev.h"
#include "IMesh.h"
#include "CMesh.h"
#include "CCubeMesh.h"
#include "CMaterial.h"
#include "CTerrain.h"
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

IMesh* CResourceMgr::Find_Mesh(const string& key)
{
	auto iter = find_if(m_MeshContainer.begin(), m_MeshContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_MeshContainer.end())
		return nullptr;
	else
		return iter->second;

}

CMaterial* CResourceMgr::Find_Material(const string& key)
{
	auto iter = find_if(m_MatContainer.begin(), m_MatContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_MatContainer.end())
		return nullptr;
	else
		return iter->second;
}

const LPDIRECT3DTEXTURE9& CResourceMgr::Find_Texture(const string& key)
{
	auto iter = find_if(m_TexContainer.begin(), m_TexContainer.end(),
		[&key](auto& pair) ->bool {return key == pair.first;});

	if (iter == m_TexContainer.end())
		return nullptr;
	else
		return iter->second;
}

HRESULT CResourceMgr::Load_All()
{
	LoadAllTexture();
	LoadDeafultMesh();
	LoadStaticMeshByX("tiger");
	LoadTerrainMeshByRaw("coastMountain64", 64, 64);
	LoadTerrainMeshByRaw("terrain", 512, 512);

	return S_OK;
}

void CResourceMgr::LoadDeafultMesh()
{
	CCubeMesh* mesh = CCubeMesh::Create();
	mesh->SetKey("Deafult_Cube");
	m_MeshContainer.insert({ "Deafult_Cube" ,mesh });
}

void CResourceMgr::LoadStaticMeshByX(const string& key)
{
	const string xFilePath = "../Bin/Resource/xFiles/" + key + ".x";

	//인접 버퍼, 재질 버퍼, 재질 개수
	ID3DXBuffer* adjBuffer = nullptr;
	ID3DXBuffer* mtrlBuffer = nullptr;
	DWORD        numMtrls = 0;

	//메시 구조체 생성
	LPD3DXMESH newMesh = nullptr;
	HRESULT hr = D3DXLoadMeshFromX(
		xFilePath.c_str(),
		D3DXMESH_MANAGED,
		m_pDevice,
		&adjBuffer, //인접 정보 반환
		&mtrlBuffer, //재질 정보 반환
		nullptr,
		&numMtrls, //재질 개수 반환
		&newMesh); //메쉬 정보 반환

	if (FAILED(hr))
	{
		::MessageBoxA(0, "D3DXLoadMeshFromX() - FAILED", "Error", MB_OK);
		return;
	}

	//정적 오브젝트 메시 클래스 생성
	CMesh* meshClass = CMesh::Create();
	if (!meshClass) return;
	// 메시 등록
	meshClass->SetMesh(newMesh);//->구조체 등록
	meshClass->SetKey(key); //->키 등록
	m_MeshContainer.insert({ key, meshClass }); //->데이터 컨테이너에 삽입

	// 머티리얼 및 텍스처 처리
	if (mtrlBuffer && numMtrls > 0)///없지 않다면
	{
		//재질 구조체 배열(포인터) 받아오기
		D3DXMATERIAL* mtrls = reinterpret_cast<D3DXMATERIAL*>(mtrlBuffer->GetBufferPointer());
		//재질 클래스 생성
		CMaterial* material = CMaterial::Create();
		material->SetSize(numMtrls); //미리 안에 컨테이너 개수 할당
		material->SetKey(key); //-> 키 할당
		vector<MATTEX>& matList = material->Get_Material(); //할당한 벡터 가져오기->구조체{재질/텍스쳐}

		for (DWORD i = 0; i < numMtrls; ++i)
		{
			MATTEX tmp; //임시 구조체 생성
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse; //앰비언트가 기본 0일 때가 있어서 초기화
			tmp.material = mtrls[i].MatD3D;

			if (mtrls[i].pTextureFilename)
			{
				tmp.texture = Find_Texture(mtrls[i].pTextureFilename); //미리 불러온 텍스쳐 파일에서 가져오기
				if (tmp.texture) {
					tmp.texture->AddRef();
				}
			}
			else {
				tmp.texture = nullptr; //없으면 문제 없도록 널포인터
			}

			matList.push_back(tmp); //가져와서 넣기
		}

		m_MatContainer.insert({ key, material });
	}

	// 자원 정리
	Safe_Release(newMesh);
	Safe_Release(mtrlBuffer);
	Safe_Release(adjBuffer);
}

void CResourceMgr::LoadTerrainMeshByRaw(const string& key, int x, int z)
{
	int vertexNum= x * z; //사진 상의 가로/세로 사이즈

	std::vector<BYTE> in(vertexNum); //우선 바이트 단위로 읽기

	string fileName = "../Bin/Resource/Terrain/" + key + ".raw";

	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);

	if (!inFile.is_open()) {
		::MessageBoxA(0, "LoadTerrainMeshByRaw() - FAILED", "Error", MB_OK);
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
		_heightmap[i] = in[i];

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
		::MessageBoxA(0, "LoadTerrainMeshByRaw() -buffer Create FAILED", "Error", MB_OK);
		Safe_Release(instance);
		return;
	}

	instance->SetVTX_Buffer(pVB);
	m_MeshContainer.insert({ key, instance });
	pVB->Release();
}

void CResourceMgr::LoadAllTexture()
{
	_finddata_t fd;

	const string texBasePath = "../Bin/Resource/Textures/*.*";
	intptr_t handle = _findfirst(texBasePath.c_str(), &fd); // _findfirst 사용

	if (handle == -1)
	{
		//::MessageBoxA(0, "LoadAllTexture() - FAILED", "Error", MB_OK);
		return;
	}

	do
	{
		if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
			continue;

		LPDIRECT3DTEXTURE9 _texture;
		string fileName = "../Bin/Resource/Textures/" + string(fd.name);

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fileName.c_str(), &_texture)))
		{
			::MessageBoxA(0, "CreateTexture() - FAILED", "Error", MB_OK);
			_texture->Release();
			return;
		}

		string rawName = fd.name;
		m_TexContainer.insert({ rawName , _texture });

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
	return;
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

	m_pDevice->Release();
}
