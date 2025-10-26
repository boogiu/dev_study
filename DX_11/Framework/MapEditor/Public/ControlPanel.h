#pragma once
#include "BasePanel.h"
NS_BEGIN(Engine)
	class IResourceService;
	class CTexture;
NS_END

NS_BEGIN(MapEditor)
class CControlPanel :
	public CBasePanel
{
private:
	CControlPanel(GUI_CONTEXT* context);
	virtual ~CControlPanel();
	HRESULT Initialize();
public:
		virtual void Set_Grid(class CGridObject* pGrid);
public:
	virtual void Render_GUI() override;
private:
	void Render_BrushTab();
	void Render_EditTab();
	void Render_TileEdit();
	void Render_SaveTab();
private:
	class CGridObject* m_pGrid = { nullptr };
	Mode m_eMode = {};
	_bool SaveTab = {};
	_uint nowFlag = {};
public:
	static CControlPanel* Create(GUI_CONTEXT* context);
	virtual void Free();
};

NS_END