#include "Engine_Define.h"
#include "CTextureMgr.h"
#include <io.h>

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
}

HRESULT CTextureMgr::Ready_Texture(LPDIRECT3DDEVICE9 pDevice, string folderPath)
{
	_finddata_t fd;
	
	string searchPath = folderPath + "/*.*";
	intptr_t handle = _findfirst(searchPath.c_str(), &fd); // _findfirst 사용

	if (handle == -1) return E_FAIL;

	do
	{
		if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
			continue;

		LPDIRECT3DTEXTURE9 _texture;
		string fileName = folderPath + '/' + fd.name;
		if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName.c_str(), &_texture)))
		{
			//MessageBoxA(nullptr, FMOD_ErrorString(result), "Sound Load Failed", MB_OK);
			return E_FAIL;
		}

		// 확장자 없는 키 생성
		string rawName = fd.name;
		size_t dotPos = rawName.find_last_of('.'); //이름에서 . 위치

		if (dotPos != string::npos)
			rawName = rawName.substr(0, dotPos); //. 직전까지만

		string Key(rawName.begin(), rawName.end()); //키로 만들어주기

		// 등록
		m_textureContainer.insert({Key.c_str(), _texture });
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
	return S_OK;
}

LPDIRECT3DTEXTURE9 CTextureMgr::Get_Texture(const string& key)
{
	auto iter = find_if(m_textureContainer.begin(), m_textureContainer.end(),
		[&key](auto& cmp)->bool {
			return cmp.first == key;
		});

	if (iter != m_textureContainer.end()) {
		return m_textureContainer[key];
	}

	return nullptr;
}


void CTextureMgr::Free()
{
}
