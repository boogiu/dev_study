#pragma once
class CMapEditor
{
public:
	CMapEditor();
	~CMapEditor();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;
};

