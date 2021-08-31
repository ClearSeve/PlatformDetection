#ifndef UTILS_FILE_FIND_DATA_H_
#define UTILS_FILE_FIND_DATA_H_


namespace utils {

class CFileFindData
{
public:
    CFileFindData();
    ~CFileFindData();

public:
    CAtlString GetName();
    CAtlString GetPath();
    CAtlString GetFilePath();
    CAtlString GetFileShortPath();
    BOOL IsDirectory();
    BOOL IsFile();

public:
    int GetCurDepth();

protected:
    CAtlString m_strFindPath;
    WIN32_FIND_DATA m_findData = { 0 };
    UINT m_nCurrentDepth = 0;
    BOOL m_nMaxDepth = -1;
};


} // namespace utils

#endif // UTILS_FILE_FIND_DATA_H_