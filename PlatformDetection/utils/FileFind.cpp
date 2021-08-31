#include "stdafx.h"
#include "FileFind.h"

namespace utils {

CFileFind::CFileFind(LPCTSTR lpFindPath)
{
    m_strFindPath = lpFindPath;
}


CFileFind::~CFileFind()
{
    if (INVALID_HANDLE_VALUE != m_hFindFile) {
        FindClose(m_hFindFile);
    }
}

BOOL CFileFind::FindFile()
{
    m_hFindFile = ::FindFirstFile(m_strFindPath, &m_findData);
    if (INVALID_HANDLE_VALUE == m_hFindFile) {
        return FALSE;
    }
    return TRUE;
}

BOOL CFileFind::FindNext()
{
    return ::FindNextFile(m_hFindFile, &m_findData);
}

} // namespace utils

