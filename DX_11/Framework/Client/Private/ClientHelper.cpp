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

HRESULT ClientHelper::Add_SoundClips(string directoryPath)
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

		if (ext == ".wav") {
			string fileName = entry.path().filename().string();;
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName, entry.path().string());
		}
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

wstring ClientHelper::ExtractVowels(const std::wstring& src)
{
	std::wstring result;

	for (wchar_t ch : src)
	{
		if (ch >= 0xAC00 && ch <= 0xD7A3)
		{
			int SIndex = ch - 0xAC00;
			int jung = (SIndex / 28) % 21;

			wchar_t aeiou = MapJungToAEIOU(jung);
			result.push_back(aeiou);
		}
	}

	return result;
}

wchar_t ClientHelper::MapJungToAEIOU(int jungIndex)
{
	switch (jungIndex)
	{
		// 아 계열
	case 0:  // ㅏ
	case 2:  // ㅑ
		return L'아';

		// 에 계열
	case 1:  // ㅐ
	case 3:  // ㅒ
	case 5:  // ㅔ
	case 7:  // ㅖ
	case 10: // ㅙ
		return L'에';

		// 이 계열
	case 16: // ㅟ
	case 20: // ㅣ
		return L'이';

		// 오 계열
	case 8:  // ㅗ
	case 11: // ㅚ
	case 12: // ㅛ
	case 9:  // ㅘ
		return L'오';

		// 우 계열
	case 13: // ㅜ
	case 14: // ㅝ
	case 15: // ㅞ
	case 17: // ㅠ
		return L'우';

		// 나머지 (ㅡ, ㅢ, 기타) → 적당히 처리
	case 18: // ㅡ
	case 19: // ㅢ
	default:
		return L'이'; // 혹은 L'아', 혹은 무시 등 취향대로
	}
}

itemType ClientHelper::Get_Type(const string& typeString)
{
	if (typeString == "Fruit")
		return itemType::Fruit;
	if (typeString == "Ore")
		return itemType::Ore;
	if (typeString == "Plant")
		return itemType::Plant;
	if (typeString == "Insect")
		return itemType::Insect;
	if (typeString == "Fish")
		return itemType::Fish;
	if (typeString == "Axe")
		return itemType::Axe;
	if (typeString == "Net")
		return itemType::Net;
	if (typeString == "Scoop")
		return itemType::Scoop;
	if (typeString == "FishingRod")
		return itemType::FishingRod;
	if (typeString == "Represent")
		return itemType::Represent;
	if (typeString == "Furniture")
		return itemType::Furniture;

	return itemType::None;
}
