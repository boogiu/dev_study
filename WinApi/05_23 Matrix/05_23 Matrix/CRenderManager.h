#pragma once
class CRenderer;

class CRenderManager
{
private:
	CRenderManager();
	~CRenderManager();

public:
	static CRenderManager& Get_Instance() {
		static CRenderManager instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void RegisterRenderer(CRenderer* comp);
	void ReleaseRenderer(CRenderer* comp);

private:
	std::unordered_map<RenderLayer, std::vector<CRenderer*>> m_RenderContainer;
};

