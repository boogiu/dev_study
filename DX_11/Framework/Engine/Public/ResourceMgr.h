#pragma once
#include "IResourceService.h"

NS_BEGIN(Engine)

class CResourceMgr final :
	public IResourceService
{
	struct RS_Pool {
		unordered_map<string, class CShader*>m_Shaders;
		unordered_map<string, class CVIBuffer*>m_Buffers;
		unordered_map<string, class CModelData*>m_ModelDatas;
		unordered_map<string, vector<class CMaterialData*>> m_MaterialInstances;
		unordered_map<string, class CTexture*>m_Textures;
		unordered_map<string, class CSoundData*>m_Sounds;
		unordered_map<string, class CAnimationClip*>m_Animations;
	};

private:
	CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CResourceMgr() DEFAULT;

public:
	HRESULT Initiallize();
	virtual void Clear_Resource(const string& levelTag)override;
	virtual HRESULT Sync_To_Level()override;

public:
	virtual class CSoundData* Load_Sound(const string& levelTag, const string& soundKey) override;

	virtual class CModelData* Load_ModelData(const string& levelTag, const string& ModelKey) override;
	virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& bufferKey, BUFFER_TYPE eType) override;
	virtual vector<class CMaterialInstance*> Load_MaterialFromFile(const string& levelTag, const string& fileKey) override;

	virtual class CShader* Load_Shader(const string& levelTag, const string& shaderKey) override;
	virtual class CTexture* Load_Texture(const string& levelTag, const string& textureKey) override;
	virtual class CAnimationClip* Load_AnimClip(const string& levelTag, const string& Subject, const string& AnimClipKey) override;

	virtual string Get_ResourcePath(const string& resourceKey) override;
	virtual HRESULT Add_ResourcePath(const string& resourceKey, const string& resourcePath) override;

	/*머티리얼의 문제*/
	/*1.		다른 모델/메쉬 임에도 머티리얼 데이터 키가 겹치는 경우가 많을 것으로 예상됨*/
	/*2.		머티리얼 데이터 안에 있는 텍스처(디퓨즈,노멀)들이 여러개씩 있어서 각각의 인덱스를 설정해주어야 함*/
	/*3.		지형처럼 텍스처가 매핑되어 있지 않은 머티리얼의 경우, 직접 그 데이터 안에 머티리얼 텍스처를 매핑해주어야 함.*/


	/*해결 방법 고안*/
	/*1.		머티리얼은 그냥 파일 단위 로드로 진행 한다. 해당 파일에 속한 데이터들을 벡터로 관리하다가 넘겨준다*/
	/*2.		머티리얼 데이터를 반환할 때 오버라이드 구조체를 함께 넘겨서 텍스처의 인덱스를 관리해준다. */
	/*3.		지형처럼 특정 텍스처가 매핑되는 것은 맵툴과 같은 사항을 활용해 완전한 파일로 만들어서 로딩한다...?*/

private:
	_int ValidLevel(const string& levelKey);
	void Load_InitialResource();
private:
	string MakePath(const string& pathKey);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class CGameInstance* m_pInstance = { nullptr };

	vector<RS_Pool >m_Resources;
	unordered_map<string, _uint> m_LevelIndex;
	unordered_map<string, string>m_KeyPath;
	unordered_map<string, vector<string>>m_MateriaFileKeys;

public:
	static CResourceMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


NS_END

