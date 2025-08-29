#pragma once
#include "BasePanel.h"
NS_BEGIN(Engine)

class CHierarchyPanel final: 
	public CBasePanel
{
private:
	CHierarchyPanel();
	virtual ~CHierarchyPanel();
	 HRESULT Initialize() ;

public:
	virtual void Render_GUI() override;

private:

public:
	static CHierarchyPanel* Create();
	virtual void Free();

};

NS_END
