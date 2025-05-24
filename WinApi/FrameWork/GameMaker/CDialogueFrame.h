#pragma once
#include "CBase_UI.h"
class CIcon;
class CDialogueFrame:public CBase_UI
{
public:
	CDialogueFrame();
	~CDialogueFrame();
public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

private:
	void MoveTo(const TCHAR* Key, float Dest);
	void RenderName(HDC _hDC);
	void RenderScript(HDC _hDC);
private:
	unordered_map < const TCHAR*, CIcon* > m_IconContainer;
	float DialCount;
	int SoundID = 0;
};

