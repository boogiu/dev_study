#pragma once
#include "BasePanel.h"
NS_BEGIN(Engine)

class CHierarchyPanel final: 
	public CBasePanel
{
private:
	CHierarchyPanel(SIZE vp);
	virtual ~CHierarchyPanel();
	 HRESULT Initialize() ;

public:
	virtual void Render_GUI() override;

private:
	class IObjectService* m_pObjectManager = { nullptr };

public:
	static CHierarchyPanel* Create(SIZE vp);
	virtual void Free();

};

NS_END
