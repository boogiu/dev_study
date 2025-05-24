#include "pch.h"
#include "CFunction.h"

HFONT CFunction::CreateFontW(const std::wstring& fontName, int size, bool bold, bool italic)
{
    return ::CreateFontW(
        size, 0, 0, 0,
        bold ? FW_BOLD : FW_NORMAL,
        italic ? TRUE : FALSE,
        FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_DONTCARE,
        fontName.c_str()
    );
}

bool CFunction::LoadExternalFont(const std::wstring& fontPath)
{
    return AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, 0);
}
