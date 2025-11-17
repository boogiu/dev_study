#include "ClientHelper.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Animator3D.h"

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
		if (!filesystem::exists(directoryPath)) {
			string msg = "No such directory: " + directoryPath + "\n";
			OutputDebugStringA(msg.c_str());
		}
		string ext = entry.path().extension().string();

		if (ext == ".anim") {
			string fileName = entry.path().filename().string();;
			CGameInstance::GetInstance()->Get_ResourceMgr()->
				Add_ResourcePath(fileName + "_" + Subject, entry.path().string());
			
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

HRESULT ClientHelper::Add_AllClipsByFile(string filePath, string Level, string Subject, CAnimator3D* pAnimator)
{
	ifstream ifs(filePath);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"Clips 파일을 찾을 수 없습니다.", L"Error", MB_OK);
		return E_FAIL;
	}

	json jScene;

	try {
		ifs >> jScene;
	}
	catch (const json::parse_error& e) {
		MessageBoxA(nullptr, e.what(), "JSON Parse Error", MB_OK);
		return E_FAIL;
	}
	ifs.close();

	for (auto& item : jScene)
	{
		try
		{

			string animName = item.value("FileName", "");
			_bool loopFlag = false;

			if (item.contains("Loop"))
			{
				if (item["Loop"].is_boolean())
				{
					loopFlag = item["Loop"].get<bool>();
				}
				else if (item["Loop"].is_string())
				{
					string value = item["Loop"].get<std::string>();
					transform(value.begin(), value.end(), value.begin(), ::tolower); // 전부 소문자로 변환

					loopFlag = (value == "true" || value == "1" || value == "yes" || value == "on");
				}
			}

			HRESULT hr = pAnimator->Add_AnimClips(Level, animName, Subject, loopFlag);
			
		}
		catch (...)
		{
			continue;
		}
	}


	return S_OK;
}
