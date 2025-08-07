#pragma once
#include "IProtoService.h"

NS_BEGIN(Engine)
class CPrototypeMgr final: public IProtoService
{
private:
	CPrototypeMgr();
	virtual ~CPrototypeMgr() override DEFAULT;

public:
	HRESULT Initialize();

private:

private:
	static CPrototypeMgr* Create();
	virtual void Free() override;
};
NS_END
