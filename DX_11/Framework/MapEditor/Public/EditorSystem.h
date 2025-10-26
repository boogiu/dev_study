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
public:
	struct DebugIndex {
		TILE_INDEX index;
		_float4 saveMtl = {};
	};
public:
	struct Editor_Context {
		Mode eMode = { EditObj };
		_uint eAnchor = {static_cast<_uint>(ANCHOR::Center) };
		_uint tileFlag =	{static_cast<_uint>(ANCHOR::Center) };
		CGameObject* pEditingObject = {nullptr};
		string baseType = {};
		TILESYSTEM_INFO ContextTileInfo = {};
		_float4 m_fConerHeight = {};
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
	HRESULT Create_MapObject(const string& folderName);
public:
	void Clear_Index();
	void Adjust_Height();
	void Adjust_Flag();
public:
	HRESULT Load_MapData();
	HRESULT Save_MapData();

private:
	void Execute_TileSystem();
	void Create_GUIPanels();
	void Create_Ray();

private:
	void DragDrop_Object();
	void Brushing_Tiles();
	void ConvertMaterial(string brushType, TILE_INDEX Index);
	void Editing_TileInfo();

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

	/*GridObject*/
	class CGridObject* m_pGrid = { nullptr };
	_float3 m_GridMinEdge = {};
	_float3 m_GridMaxEdge = {};

	vector<DebugIndex> m_selectedIndex;

	/*GUI Panel*/
	class CDirectoryPanel* m_pDirectoryPanel = { nullptr };
	class CControlPanel* m_pControlPanel = { nullptr };
	
public:
	virtual void Free() override;
};

NS_END