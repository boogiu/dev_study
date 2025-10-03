#pragma once
#include "BasePanel.h"
NS_BEGIN(Engine)
class IResourceService;
class CTexture;
NS_END

NS_BEGIN(MapEditor)
class CEditorPanel :
    public CBasePanel
{
private:
	CEditorPanel(GUI_CONTEXT* context);
	virtual ~CEditorPanel();
	HRESULT Initialize();

public:
	virtual void Render_GUI() override;
	virtual void Set_Grid(class CGridObject* pGrid);
	void Notify_CreateComplete() {m_bItemCreated = false;}

private:

private:
	 IResourceService* m_pResource = { nullptr };
	 CTexture* m_pFolderIcon= { nullptr };
	 _float m_fIconSize = {48 };
	 string payload = {};
	 _bool m_bItemCreated = { false };
	vector<string> m_FolderNames;
	class CGridObject* m_pGrid = { nullptr };

public:
	static CEditorPanel* Create(GUI_CONTEXT* context);
	virtual void Free();
};
NS_END
