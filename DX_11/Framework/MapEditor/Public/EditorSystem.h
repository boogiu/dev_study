#pragma once
#include "Base.h"
#include "GUI_Context.h"
NS_BEGIN(Engine)
class IRayService;
class CGameObject;
class IInputService;
class IObjectService;
class ITileService;
NS_END

NS_BEGIN(MapEditor)
class CEditorSystem :
	public CBase
{
	DECLARE_SINGLETON(CEditorSystem);

public:
	struct Editor_Context {
		CGameObject* pEditingObject = {nullptr};
		TILESYSTEM_INFO ContextTileInfo = {};
		vector<CGameObject*> vecEditingObjects;
	};

private:
	explicit CEditorSystem();
	virtual ~CEditorSystem();

public:
	HRESULT Initialize();
	void Update(_float dt);

public:
	Editor_Context* Get_Context() { return &m_EditorContext; }
	HRESULT Create_Object(const string& folderName);
	HRESULT Create_ObjectArea(_fvector vEdgeMin, _cvector vEdgeMax);

private:
	void Execute_TileSystem();
	void Create_Ray();
	void DragDrop_Object();
	void Select_Object();

private:
	POINT m_MousePt = {};
	RAY m_tRay = {};

	Editor_Context m_EditorContext = {};
	GUI_CONTEXT* m_pUIContext = { nullptr };

	IRayService* m_pRayManager = { nullptr };
	IInputService* m_pInputService = { nullptr };
	IObjectService* m_pObjMgr = { nullptr };
	ITileService* m_pTileSystem = { nullptr };

	const _float4x4* m_pViewMat = { nullptr };
	const _float4x4* m_pProjMat = { nullptr };

	class CGridObject* m_pGrid = { nullptr };
	class CEditorPanel* m_pPanel = { nullptr };
public:
	virtual void Free() override;
};

NS_END