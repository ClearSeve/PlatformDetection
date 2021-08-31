#include "stdafx.h"
#include "FileFindData.h"
#include <string>

namespace utils {

CFileFindData::CFileFindData()
{
}


CFileFindData::~CFileFindData()
{
}

CAtlString CFileFindData::GetName()
{
    return CAtlString(m_findData.cFileName);
}

CAtlString CFileFindData::GetPath()
{
    CAtlString strPath;
    int nPos = m_strFindPath.ReverseFind(_T('\\'));
    if (nPos > -1) {
        strPath = m_strFindPath.Left(nPos + 1);
        if (strPath.GetLength() < 3) {
            strPath += _T("\\");
        }
    }
    return strPath;
}

CAtlString CFileFindData::GetFilePath()
{
    return GetPath() + GetName();
}

BOOL CFileFindData::IsFile()
{
    if (FILE_ATTRIBUTE_DIRECTORY & m_findData.dwFileAttributes) {
        return FALSE;
    }
    if (0 == GetName().Compare(_T(".")) || 0 == GetName().Compare(_T(".."))) {
        return FALSE;
    }
    return TRUE;
}

int CFileFindData::GetCurDepth()
{
    return m_nCurrentDepth;
}

} // namespace utils
