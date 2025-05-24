#pragma once
class CBmpMgr;
class CSoundMgr;

class CResourceMgr
{
private:
	CResourceMgr();
	~CResourceMgr();
	CResourceMgr(CResourceMgr& rhs) =delete;
	CResourceMgr operator=(CResourceMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CResourceMgr* Get_Instance() {
		if (!m_pInstance) {
			m_pInstance = new CResourceMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	HDC Find_Image(const TCHAR* pImageKey);
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey);

	void PlaySound(const wstring& pSoundKey, float fVolume, int* ID, bool Loop =false);
	void PlaySound(const wstring& pSoundKey, float fVolume, bool Loop =false);
	void PlayBGM(const wstring& pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopSound(int eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
private:
	void Load_Resource();
private:
	static CResourceMgr* m_pInstance;
	CBmpMgr* m_BmpMgr;
	CSoundMgr* m_SoundMgr;
};

