#pragma once
#include "IObjectService.h"

NS_BEGIN(Engine)
class CObjectMgr final : public IObjectService 
{
private:
	CObjectMgr();
	~CObjectMgr() DEFAULT;

public:

private:
	unordered_map<string, class CLayer*> m_Layers;

public:
	static CObjectMgr* Create();
	virtual void Free() override;
};
NS_END
