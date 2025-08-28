#include "Helper_Func.h"

_bool Helper::ContainsNonAscii(const string& str)
{
	for (unsigned char c : str) {
		if (c >= 0x80) {
			return true;
		}
	}
	return false;
}

_bool Helper::IsPathInProjectFolder(const string& path)
{
    //현재 솔루션 Path
    filesystem::path projectPath = SOLUTION_DIR; //전처리기로 빌드 시에 솔루션 디렉토리 가져옴
    //선택된 프로젝트 Path
    filesystem::path selectedPath = filesystem::absolute(path);

    string projectStr = projectPath.string();
    string selectedStr = selectedPath.string();

    if (selectedStr.find(projectStr) == 0) // selectedStr이 projectStr로 시작하면
    {
        return true;
    }

    return false;
}


string Helper::OpenFile_Dialogue()
{
	string filePath = "";
	nfdchar_t* outPath = nullptr;
	filesystem::path defaultPath = SOLUTION_DIR;
	nfdresult_t result = NFD_OpenDialog(nullptr, defaultPath.string().c_str(), &outPath);
	if (result == NFD_OKAY)
	{
		filePath = outPath;
		if (Helper::ContainsNonAscii(filePath)) {
			MSG_BOX("File Path Must Be English");
			filePath.clear();
		}
		else if (!Helper::IsPathInProjectFolder(filePath)) {
			MSG_BOX("Files outside the project folder cannot be selected");
			filePath.clear();
		}
	}
	else if (result == NFD_ERROR)
	{
		MSG_BOX("File Open Error : CGUISystem");
	}

	if (outPath)
		free(outPath);

	return filePath;
}

string Helper::OpenFolder_Dialogue()
{
	string folderPath = "";
	nfdchar_t* outPath = nullptr;
	filesystem::path defaultPath = SOLUTION_DIR;

	nfdresult_t result = NFD_PickFolder(defaultPath.string().c_str(), &outPath);
	if (result == NFD_OKAY)
	{
		folderPath = outPath;
		if (Helper::ContainsNonAscii(folderPath)) {
			MSG_BOX("Folder Can't Be Open : Not English Path");
			folderPath.clear();
		}
		else if (!Helper::IsPathInProjectFolder(folderPath)) {
			MSG_BOX("Folders outside the project folder cannot be selected");
			folderPath.clear();
		}
	}
	else if (result == NFD_ERROR)
	{
		MSG_BOX("Folder Open Error : CGUISystem");
	}

	if (outPath)
		free(outPath);

	return folderPath;
}

string Helper::SaveFileDialog()
{
	string savePath = "";
	filesystem::path defaultPath = SOLUTION_DIR;
	string filter = "json;txt";

	nfdchar_t* outPath = nullptr;
	nfdresult_t result = NFD_SaveDialog(filter.c_str(), defaultPath.string().c_str(), &outPath);

	if (result == NFD_OKAY)
	{
		savePath = outPath;
		if (Helper::ContainsNonAscii(savePath)) {
			MSG_BOX("Folder Can't Be Open : Not English Path");
			savePath.clear();
		}
		else if (!Helper::IsPathInProjectFolder(filesystem::path(savePath).parent_path().string())) { //파일 부모 폴더 경로까지만
			MSG_BOX("Folders outside the project folder cannot be selected");
			savePath.clear();
		}
	}
	else if (result == NFD_ERROR)
	{
		MSG_BOX("Folder Open Error : CGUISystem");
	}

	if (outPath)
		free(outPath);

	return savePath;
}

wstring Helper::ConvertToWideString(const string& str)
{
	if (str.empty()) return wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

string Helper::ConvertToString(const wstring& wstr)
{
	if (wstr.empty()) return string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
