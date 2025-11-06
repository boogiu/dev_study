#include "ClientHelper.h"
#include "GameInstance.h"
#include "IResourceService.h"

HRESULT ClientHelper::Add_ModelPathFromDirectory(string directoryPath)
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	for (const auto& entry : filesystem::recursive_directory_iterator(filesystem::path(directoryPath))) {
		if (!entry.is_regular_file())
			continue;

		string ext = entry.path().extension().string();

		if (ext == ".model") {
			string fileName = entry.path().filename().string();
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName, entry.path().string());
		}
	}

	return S_OK;
}

HRESULT ClientHelper::Add_MaterialPathFromDirectory(string directoryPath)
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	for (const auto& entry : filesystem::recursive_directory_iterator(filesystem::path(directoryPath))) {
		if (!entry.is_regular_file())
			continue;

		string ext = entry.path().extension().string();

		if (ext == ".mat") {
			string fileName = entry.path().filename().string();
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName, entry.path().string());
		}
	}

	return S_OK;
}

HRESULT ClientHelper::Add_AnimPathFromDirectory(string directoryPath, string Subject)
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	for (const auto& entry : filesystem::recursive_directory_iterator(filesystem::path(directoryPath))) {
		if (!entry.is_regular_file())
			continue;

		string ext = entry.path().extension().string();

		if (ext == ".anim") {
			string fileName = entry.path().filename().string();;
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName+"_"+ Subject, entry.path().string());
		}
	}

	return S_OK;
}

HRESULT ClientHelper::Add_TexturePathFromDirectory(string directoryPath)
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();

	for (const auto& entry : filesystem::directory_iterator(filesystem::path(directoryPath))) {
		if (!entry.is_regular_file())
			continue;

		string ext = entry.path().extension().string();

		if (ext != ".png" && ext != ".jpg" && ext != ".jpeg" && ext != ".bmp" && ext != ".dds")
			continue;

		string fileName = entry.path().filename().string();
		string parentFolder = entry.path().parent_path().filename().string();
		string combinedKey = parentFolder + "_" + fileName;
		CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(combinedKey, entry.path().string());
		}
	return S_OK;
}

