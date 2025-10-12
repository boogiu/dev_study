#pragma once
#include "Base.h"
#include "GUI_Context.h"
NS_BEGIN(Engine)
class IRayService;
class CGameObject;
class IInputService;
class IObjectService;
class ITileService;
class IRenderService;
NS_END

NS_BEGIN(MapEditor)
class CEditorSystem :
	public CBase
{
	DECLARE_SINGLETON(CEditorSystem);
	enum ObjType {TILE, FIELDOUT};

public:
	struct Editor_Context {
		CGameObject* pEditingObject = {nullptr};
		ObjType eType = {};
		TILESYSTEM_INFO ContextTileInfo = {};
	};

private:
	explicit CEditorSystem();
	virtual ~CEditorSystem() DEFAULT;

public:
	HRESULT Initialize();
	void Update(_float dt);

public:
	Editor_Context* Get_Context() { return &m_EditorContext; }
	HRESULT Delete_Object(class CGameObject* pObject);

	HRESULT Create_Tile(const string& folderName);
	HRESULT Create_FieldOut(const string& folderName);

	HRESULT Save_Blocks();
	void Create_Base();

private:
	void Execute_TileSystem();
	void Create_GUIPanels();
	void Create_Ray();

	void DragDrop_Object();

private:
	POINT m_MousePt = {};

	/*Context*/
	Editor_Context m_EditorContext = {};
	GUI_CONTEXT* m_pUIContext = { nullptr };

	/*Managers*/
	IInputService* m_pInputService = { nullptr };
	IObjectService* m_pObjMgr = { nullptr };
	ITileService* m_pTileSystem = { nullptr };
	IRenderService* m_pRenderSystem = { nullptr };

	/*Ray*/
	RAY m_tRay = {};
	IRayService* m_pRayManager = { nullptr };
	const _float4x4* m_pViewMat = { nullptr };
	const _float4x4* m_pProjMat = { nullptr };

	/*BaseObject*/
	class CGridObject* m_pGrid = { nullptr };

	/*GUI Panel*/
	class CDirectoryPanel* m_pDirectoryPanel = { nullptr };
	class CControlPanel* m_pControlPanel = { nullptr };

	_uint m_BaseTileID = {};
public:
	virtual void Free() override;
};

NS_END