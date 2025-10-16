#pragma once
#include "BasePanel.h"
NS_BEGIN(Engine)
class IResourceService;
class CTexture;
NS_END

NS_BEGIN(MapEditor)
class CDirectoryPanel :
    public CBasePanel
{
	enum FolderType {PARENT,LEAF};

	struct FolderInfo{
		FolderType eType;
		string strName;
	};
	enum  Mode
	{
		FIELDOUT,
		STRUCTURE
	};

private:
	CDirectoryPanel(GUI_CONTEXT* context);
	virtual ~CDirectoryPanel();
	HRESULT Initialize();

public:
	virtual void Render_GUI() override;
	void Notify_CreateComplete() {m_bItemCreated = false;}

private:
	void Render_Folders();
private:
	void Create_MapObject(const string& name);

private:
	void LookUp_Directory(const string& path);
	void GoTo_Parent();
	string TruncateText(const string& text, _uint Length);

private:
	 IResourceService* m_pResource = { nullptr };

	 _float m_fIconSize = { 48 };
	 CTexture* m_pFolderIcon= { nullptr };
	 CTexture* m_pModelIcon= { nullptr };

	 _bool m_bItemCreated = { false };

	 string m_strDirectory = {};
	vector<FolderInfo> m_ContentInfos;
	 _bool m_bLeafFolder = { false };
	 
	 Mode m_eMode = FIELDOUT;
public:
	static CDirectoryPanel* Create(GUI_CONTEXT* context);
	virtual void Free();
};
NS_END
